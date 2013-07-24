/*
 *	Copyright (c) 2013, Synapticon GmbH
 *	All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or without
 *	modification, are permitted provided that the following conditions are met: 
 *
 *	1. Redistributions of source code must retain the above copyright notice, this
 *	   list of conditions and the following disclaimer. 
 *	2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution. 
 *
 *	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 *	ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *	The views and conclusions contained in the software and documentation are those
 *	of the authors and should not be interpreted as representing official policies, 
 *	either expressed or implied, of the FreeBSD Project.
 *
 *  sebs_parse_ros.c created by Christian Holl
 */


extern void mycallback(const void* const __msg);


#include <rosc/sebs_parse_fw/adv_modules/sebs_parse_ros.h>
#include <rosc/debug/debug_out.h>
#include <rosc/system/endian.h>
#include <rosc/system/types.h>


sebs_parse_return_t sebs_parse_ros(sebs_parser_data_t* pdata)
{
	sebs_parse_ros_data_t *fdata=(sebs_parse_ros_data_t *)pdata->current_parser.parser_data;
	if(pdata->function_init)
	{
		pdata->function_init=false;

		switch(fdata->mode)
		{
			case SEBS_PARSE_ROS_MODE_ROSRPC:
				fdata->state=SEBS_PARSE_ROSPRC_MESSAGE_LENGTH;
			break;

			case SEBS_PARSE_ROS_MODE_BINARY:
				fdata->state=SEBS_PARSE_ROSBINARY_MESSAGE_LENGTH;
			break;
		}

		fdata->current_array_depth=0;
		fdata->current_array_length=0;
		fdata->current_buildup_field=0;
		fdata->current_memory_offset=0;
		fdata->current_submessage_size=0;
		fdata->builtin_array=false;
		fdata->builtin_array_size=0;

		DEBUG_PRINT_STR("ROS PARSER INIT");
	}

	while (*pdata->len > 0
			&& (fdata->message_length>0 ||
					fdata->state == SEBS_PARSE_ROSPRC_MESSAGE_LENGTH
					||fdata->state ==SEBS_PARSE_ROSBINARY_MESSAGE_LENGTH
			))
	{
		bool skipchar=false;

		switch(fdata->state)
		{
			/* ******
			 *ROSPRC*
			 ********/
			case SEBS_PARSE_ROSPRC_MESSAGE_LENGTH:
				fdata->state=SEBS_PARSE_ROSRPC_FIELD_LENGTH;
				SEBS_PARSE_COPY2BUFFER_INIT(pdata,fdata->copy2buffer,&fdata->message_length,4,0,g_byte_order_correction_to_system->SIZE_4_B,0,0);
				break;

			case SEBS_PARSE_ROSRPC_FIELD_LENGTH:
				DEBUG_PRINT(INT,"MSG LEN",fdata->message_length);
				fdata->state=SEBS_PARSE_ROSRPC_FIELD_ID;
				SEBS_PARSE_COPY2BUFFER_INIT(pdata,fdata->copy2buffer,&fdata->field_length,4,0,g_byte_order_correction_to_system->SIZE_4_B,0,0);
				break;

			case SEBS_PARSE_ROSRPC_FIELD_ID:
				fdata->message_length-=4;
				fdata->state=SEBS_PARSE_ROSRPC_FIELD_EQUAL;
				if(fdata->field_length > fdata->message_length)
				{
					DEBUG_PRINT_STR("ERROR FIELD LENGTH BIGGER THAN MESSAGE LENGTH!");
					//TODO ERROR
				}
				DEBUG_PRINT(INT,"Field LEN",fdata->field_length);
				SEBS_PARSE_SEEKSTRING_INIT(pdata,fdata->seekstring,ros_field_strings, ROS_FIELD_STRINGS_LEN, "=",true, fdata->field_length);
				break;

			case SEBS_PARSE_ROSRPC_FIELD_EQUAL:
				DEBUG_PRINT(INT,"Current Pos",fdata->seekstring.curChrPos);
				fdata->state=SEBS_PARSE_ROSRPC_SKIP_FIELD_CONTENT;
				//Subtract length

				fdata->field_length-=fdata->seekstring.curChrPos;
				fdata->message_length-=fdata->seekstring.curChrPos;
				if(**pdata->buf=='=')
				{
					skipchar=true;
					if(fdata->seekstring.result>=0)
					{
						fdata->rpc_field_id=fdata->seekstring.result;
						pdata->event=SEBS_PARSE_ROS_EVENT_RPC_FIELD_START;
					}
				}
				else
				{
					//TODO ERROR -> NO '='
				}
				break;

			case SEBS_PARSE_ROSRPC_SKIP_FIELD_CONTENT:

				if(fdata->field_length>0)
				{
					skipchar=true;
				}
				else
				{
					fdata->state=SEBS_PARSE_ROSRPC_FIELD_LENGTH;
				}
				break;

				/* *****************
				 *ROSMSG and ROSSRV*
				 *******************/

			case SEBS_PARSE_ROSBINARY_MESSAGE_LENGTH:
				fdata->state=SEBS_PARSE_ROSBINARY_MESSAGE_FIELD;
				SEBS_PARSE_COPY2BUFFER_INIT(pdata,fdata->copy2buffer,&fdata->message_length,4,0,g_byte_order_correction_to_system->SIZE_4_B,0,0);
				break;

			case SEBS_PARSE_ROSBINARY_MESSAGE_FIELD:
			{
				mycallback(fdata->msg_storage);

				int8_t basic_size=0;
				const int8_t* basic_byteorder=0;
				int8_t basic_repeat=0;

				switch(fdata->buildup[fdata->current_buildup_field])
				{
					case ROS_MSG_BUILDUP_TYPE_BOOL:
					case ROS_MSG_BUILDUP_TYPE_CHAR:
					case ROS_MSG_BUILDUP_TYPE_INT8:
					case ROS_MSG_BUILDUP_TYPE_UINT8:
						basic_size=1;
						basic_byteorder=0;
					break;

					case ROS_MSG_BUILDUP_TYPE_INT16:
					case ROS_MSG_BUILDUP_TYPE_UINT16:
						basic_size=2;
						basic_byteorder=g_byte_order_correction_to_system->SIZE_2_B;
					break;

					case ROS_MSG_BUILDUP_TYPE_INT32:
					case ROS_MSG_BUILDUP_TYPE_UINT32:
					case ROS_MSG_BUILDUP_TYPE_FLOAT32:
						basic_size=4;
						basic_byteorder=g_byte_order_correction_to_system->SIZE_4_B;
					break;

					case ROS_MSG_BUILDUP_TYPE_INT64:
					case ROS_MSG_BUILDUP_TYPE_UINT64:
					case ROS_MSG_BUILDUP_TYPE_FLOAT64:
						basic_size=8;
						basic_byteorder=g_byte_order_correction_to_system->SIZE_8_B;
					break;

					case ROS_MSG_BUILDUP_TYPE_STRING:
						fdata->state=SEBS_PARSE_ROSBINARY_STRING;
						SEBS_PARSE_COPY2BUFFER_INIT(pdata,fdata->copy2buffer,(int8_t*)&fdata->string_size,4,0,g_byte_order_correction_to_system->SIZE_4_B,0,0);
						break;

					case ROS_MSG_BUILDUP_TYPE_SUBMESSAGE:
						++fdata->current_buildup_field;
						++fdata->current_submessage_depth;
						break;

					case ROS_MSG_BUILDUP_TYPE_DURATION:
					case ROS_MSG_BUILDUP_TYPE_TIME:
						basic_size=4;
						basic_byteorder=g_byte_order_correction_to_system->SIZE_4_B;
						basic_repeat=1;
						break;

					case ROS_MSG_BUILDUP_TYPE_ARRAY:
					case ROS_MSG_BUILDUP_TYPE_ARRAY_UL:
						fdata->builtin_array=true;
						SEBS_PARSE_COPY2BUFFER_INIT(pdata,fdata->copy2buffer,(int8_t*)&fdata->builtin_array_size,4,0,g_byte_order_correction_to_system->SIZE_4_B,0,0);
						break;

					case ROS_MSG_BUILDUP_TYPE_SUBMESSAGEARRAY:
					case ROS_MSG_BUILDUP_TYPE_SUBMESSAGEARRAY_UL:
						++fdata->current_submessage_depth;
						break;


					case ROS_MSG_BUILDUP_TYPE_MESSAGE_END:

						break;
				}





				if(basic_size!=0)
				{
					++fdata->current_memory_offset;
					++fdata->current_buildup_field;
					//reset array
					fdata->builtin_array=false;
					SEBS_PARSE_COPY2BUFFER_INIT(pdata,fdata->copy2buffer,fdata->msg_storage+fdata->memory_offsets[fdata->current_memory_offset],basic_size,0,basic_byteorder,0,basic_repeat);
				}




			}
			break;


			case SEBS_PARSE_ROSBINARY_STRING:
			{
				uint32_t repeat;
				if(fdata->string_size>(fdata->array_lengths[fdata->current_array_length]-1)) //Check if the reserved size is too small (-1 cause of terminator '\0')
				{
					fdata->state=SEBS_PARSE_ROSBINARY_SKIP_BYTES;
					//Size = maximum length -1 (terminator)
					*((uint32_t*)(fdata->memory_offsets[fdata->current_memory_offset+1]))=(fdata->array_lengths[fdata->current_array_length]-1);
					repeat=(fdata->array_lengths[fdata->current_array_length]-1);
					//Oversize -> True
					*((bool*)(fdata->memory_offsets[fdata->current_memory_offset+2]))=true;
					fdata->skip_bytes=fdata->string_size-(fdata->array_lengths[fdata->current_array_length]-1);
				}
				else
				{
					fdata->state=SEBS_PARSE_ROSBINARY_MESSAGE_FIELD;
					//Size = maximum length
					*((uint32_t*)(fdata->memory_offsets[fdata->current_memory_offset+1]))=fdata->string_size;
					repeat=fdata->string_size;
					*((bool*)(fdata->memory_offsets[fdata->current_memory_offset+2]))=false;
				}
					fdata->current_memory_offset+=3;
				SEBS_PARSE_COPY2BUFFER_INIT(pdata,fdata->copy2buffer,fdata->msg_storage+fdata->memory_offsets[fdata->current_memory_offset],1,0,0,0,repeat);
			}
				break;

			case SEBS_PARSE_ROSBINARY_SKIP_BYTES:
				fdata->state=SEBS_PARSE_ROSBINARY_MESSAGE_FIELD;
				SEBS_PARSE_SKIP_INIT(pdata,fdata->skip,fdata->skip_bytes);
				break;








			case SEBS_PARSE_ROSBINARY_MESSAGE_BUILTIN_ARRAY:

				break;

			default: //TODO check
				break;
		}





		if(skipchar)
		{
			--fdata->field_length;
			--fdata->message_length;
			++*pdata->buf;
			--*pdata->len;

			if(fdata->message_length==0 && fdata->state != SEBS_PARSE_ROSPRC_MESSAGE_LENGTH)
			{
				pdata->event=SEBS_PARSE_ROS_EVENT_MESSAGE_END;
				DEBUG_PRINT_STR("MESSAGE END!");
			}
		}

		if(pdata->event!=SEBS_PARSE_EVENT_NONE)
			break;
	}
	return (SEBS_PARSE_RETURN_GO_AHEAD);
}
