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
 *  ros_handler.c created by Christian Holl
 */

#include <rosc/system/spec.h>
#include <rosc/com/ros_handler.h>
#include <rosc/debug/debug_out.h>
#include <rosc/system/status.h>


sebs_parse_return_t ros_handler(sebs_parser_data_t* pdata)
{
	ros_handler_data_t *hdata=pdata->handler_data;
	ros_iface_init_t *idata=(ros_iface_init_t*)pdata->init_data;
	sebs_parse_ros_data_t *fdata=(sebs_parse_ros_data_t *)pdata->current_parser.parser_data;


	if(pdata->handler_init)
	{
		DEBUG_PRINT_STR("ROS HANDLER INIT");
		pdata->handler_init=false;
		pdata->function_init=true;
		pdata->return_to_handler=false;
		pdata->overall_len=0;
		//pdata->security_len=1024;
		hdata->hstate=ROS_HANDLER_STATE_NONE;
		hdata->iface_ok=false;
		hdata->md5sum_ok=false;
		pdata->sending=false;




		if(idata->ros_type==ROS_HANDLER_TYPE_TOPIC_SUBSCRIBER)
		{
			hdata->genPayloadData[0]=idata->message_definition;
			hdata->genPayloadData[1]=idata->md5sum;
			hdata->genPayloadData[2]=idata->iface_name;
			hdata->genPayloadData[3]=idata->type_name;
			hdata->hstate=ROS_HANDLER_STATE_SUBSCRIBER_HEADER_SEND;
			SEBS_PARSE_MSG_GEN(pdata, hdata->gen, pdata->additional_storage, rosc_static_socket_additional_data_size, MSGGEN_TYPE_ROSMSG_HEADER, 0, hdata->genPayloadData);
		}
		else
		{
			SEBS_PARSE_ROS_INIT_RPC(pdata,hdata->ros);
		}

	}

	sebs_parse_ros_event_t *ros_event=(sebs_parse_ros_event_t *)&pdata->event;
	switch (pdata->event)
		{
		case SEBS_PARSE_EVENT_LEN_EQUAL_SMALLER_ZERO:
			switch (*pdata->len)
			{

			case SOCKET_SIG_CLOSE:
				DEBUG_PRINT_STR("ROSHANDLER Connection close");
					pdata->out_len = SOCKET_SIG_RELEASE;
					return (SEBS_PARSE_RETURN_GO_AHEAD);
				break;

			case SOCKET_SIG_TIMEOUT:
				DEBUG_PRINT_STR("ROSHANDLER Timeout");
				pdata->out_len=SOCKET_SIG_RELEASE;
				return(SEBS_PARSE_RETURN_GO_AHEAD);
				break;

			case SOCKET_SIG_NO_CONNECTION:
				DEBUG_PRINT_STR("ROSHANDLER No Connection");
				pdata->out_len=SOCKET_SIG_RELEASE;
				return(SEBS_PARSE_RETURN_GO_AHEAD);
				break;

	        /* ***********/
			/*Only Client*/
			/* ***********/

			case SOCKET_SIG_COULD_NOT_CONNECT:
				DEBUG_PRINT_STR("ROSHANDLER could not connect");
				pdata->out_len=SOCKET_SIG_RELEASE;
				return(SEBS_PARSE_RETURN_GO_AHEAD);
				break;

			case SOCKET_SIG_COULD_NOT_RESOLVE_HOST:
				DEBUG_PRINT_STR("ROSHANDLER Resolve Host");
				pdata->out_len=SOCKET_SIG_RELEASE;
				return(SEBS_PARSE_RETURN_GO_AHEAD);
				break;

			case SOCKET_SIG_CONNECTED:
				DEBUG_PRINT_STR("CONNECTED");
				break;


			/* SIGNALS which should not occur here: */

			case SOCKET_SIG_DATA_SENT: /*should be caught by sending function*/
				DEBUG_PRINT_STR("ROSHANDLER ignoring unexpected signal  Data Sent");
				return (SEBS_PARSE_RETURN_GO_AHEAD);
				break;
			case SOCKET_SIG_RELEASE: /*outgoing*/
				DEBUG_PRINT_STR("ROSHANDLER ignoring unexpected signal  Release");
				return (SEBS_PARSE_RETURN_GO_AHEAD);
				break;
			case SOCKET_SIG_CONNECT: /*outgoing*/
				DEBUG_PRINT_STR("ROSHANDLER ignoring unexpected signal  Connect");
				return (SEBS_PARSE_RETURN_GO_AHEAD);
				break;

			default:
				break;
			}
			break;
		}

	switch(hdata->hstate)
	{

		case ROS_HANDLER_STATE_SUBSCRIBER_HEADER_SEND:
			hdata->hstate=ROS_HANDLER_STATE_NONE;
			SEBS_PARSE_ROS_INIT_RPC(pdata,hdata->ros);
		break;
		case ROS_HANDLER_STATE_CHECK_MD5SUM:
			if (hdata->ros.seekstring.result==0)
			{
				DEBUG_PRINT_STR("MD5SUM OK!");
				hdata->md5sum_ok=false;
			}
			else
			{
				DEBUG_PRINT_STR("MD5SUM DOES NOT MATCH!!!!");
			}
			hdata->ros.field_length-=hdata->ros.seekstring.curChrPos;
			hdata->ros.message_length-=hdata->ros.seekstring.curChrPos;
			break;

		case ROS_HANDLER_STATE_CHECK_IFACE_NAME:
			if (hdata->ros.seekstring.result==0)
			{
				DEBUG_PRINT_STR("IFACE NAME OK!");
				hdata->iface_ok=false;
			}
			else
			{
				DEBUG_PRINT_STR("IFACE NAME DOES NOT MATCH!!!!");
			}
			hdata->ros.field_length-=hdata->ros.seekstring.curChrPos;
			hdata->ros.message_length-=hdata->ros.seekstring.curChrPos;
			break;

		case ROS_HANDLER_STATE_NONE:
			switch(*ros_event)
			{
				case SEBS_PARSE_ROS_EVENT_RPC_FIELD_START:
					DEBUG_PRINT(STR,"Field",ros_field_strings[hdata->ros.rpc_field_id]);
					DEBUG_PRINT(INT,"Field Content Length", hdata->ros.field_length);


					/* ********************
						  ROS RPC INFO
					 **********************/

					switch(hdata->ros.rpc_field_id)
					{
						case SEBS_PARSE_ROS_FIELD_MD5SUM:
							switch(idata->ros_type)
							{
								case ROS_HANDLER_TYPE_TOPIC_SUBSCRIBER:
								case ROS_HANDLER_TYPE_TOPIC_PUBLISHER:
								case ROS_HANDLER_TYPE_SERVICE_CLIENT:
								case ROS_HANDLER_TYPE_SERVICE_SERVER:
									hdata->hstate=ROS_HANDLER_STATE_CHECK_MD5SUM;
									SEBS_PARSE_SEEKSTRING_INIT(pdata,hdata->ros.seekstring,(const char**)&idata->md5sum, 1, "",true, hdata->ros.field_length);
								break;

								default:
									break;
							}
							break;

						case SEBS_PARSE_ROS_FIELD_SERVICE:
						case SEBS_PARSE_ROS_FIELD_TOPIC:
							switch(idata->ros_type)
							{
								case ROS_HANDLER_TYPE_TOPIC_SUBSCRIBER:
								case ROS_HANDLER_TYPE_TOPIC_PUBLISHER:
								case ROS_HANDLER_TYPE_SERVICE_CLIENT:
								case ROS_HANDLER_TYPE_SERVICE_SERVER:
									hdata->hstate=ROS_HANDLER_STATE_CHECK_IFACE_NAME;
									SEBS_PARSE_SEEKSTRING_INIT(pdata,hdata->ros.seekstring,(const char**)&idata->iface_name, 1, "",true, hdata->ros.field_length);
								break;
							}
					}

					break;
				case SEBS_PARSE_ROS_EVENT_MESSAGE_END:
					DEBUG_PRINT_STR("HANDLER: MESSAGE END!")
					switch(idata->ros_type)
					{

						case ROS_HANDLER_TYPE_TOPIC_PUBLISHER:
							break;

						case ROS_HANDLER_TYPE_TOPIC_SUBSCRIBER:

							switch(fdata->mode)
							{
								case SEBS_PARSE_ROS_MODE_ROSRPC:
										DEBUG_PRINT_STR("ROSRPC END->BINARY PARSING...");
										SEBS_PARSE_ROS_INIT_MSG(pdata,hdata->ros,idata->buildup,idata->submessage_sizes,idata->array_lengths,idata->memory_offsets,idata->message_definition,pdata->additional_storage,pdata->additional_storage+idata->submessage_states_offset);
									break;
								case SEBS_PARSE_ROS_MODE_BINARY:
										DEBUG_PRINT_STR("BINARY END... CALLBACK!")
										idata->callback(fdata->msg_storage);
										SEBS_PARSE_ROS_INIT_MSG(pdata,hdata->ros,idata->buildup,idata->submessage_sizes,idata->array_lengths,idata->memory_offsets,idata->message_definition,pdata->additional_storage,pdata->additional_storage+idata->submessage_states_offset);
									break;
							}

							break;
						default:
							ROSC_FATAL("ros handler: Not a ros handler type!");
							break;
					}




					break;

				default: //TODO check
					break;
			}
			break;
	}
	hdata->hstate=ROS_HANDLER_STATE_NONE;

	return (SEBS_PARSE_RETURN_GO_AHEAD);
}
