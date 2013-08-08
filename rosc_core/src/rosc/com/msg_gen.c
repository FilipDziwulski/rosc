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
 *  msg_gen.c created by Christian Holl
 */

#include <rosc/com/msg_gen.h>
#include <rosc/system/endian.h>
#include <rosc/system/types.h>

#define OUT_BUFFER_SIZE 1000



uint32_t gen_msg(port_t *port,	msg_gen_command_t* message_def)
{
	bool dryrun=true;
	uint32_t current_out_buffer_size=0;
	char outbuffer[OUT_BUFFER_SIZE];
	uint32_t def = 0;
	uint32_t size = 0;
	uint32_t subsize=0;
	uint32_t sub_size_storage;
	do
	{
		bool part_finished=false;
		char *str;
		uint32_t outstr_size;

		if(dryrun ==true)
		{
			str=0;
			subsize=0;
			message_def[def].size=0;

			switch(message_def[def].type)
			{
				case MSG_GEN_TYPE_WHOLE_LEN_BIN:
				case MSG_GEN_TYPE_WHOLE_LEN_STRING:
				case MSG_GEN_TYPE_END:
					break;


				case MSG_GEN_TYPE_CHAR:
					subsize+=1;
					break;

				case MSG_GEN_TYPE_STRING:
					str=(char *)message_def[def].data;
					break;

				case MSG_GEN_TYPE_TAG:
					subsize+=2; // < + >
					str=(char *)message_def[def].data;
					break;

				case MSG_GEN_TYPE_CLOSE_TAG:
					subsize+=3; //</ >
					str=(char *)message_def[def].data;
					break;

				case MSG_GEN_TYPE_ROSRPC_FIELD:
					subsize+=1; //=
					str=(char *)message_def[def].data;
					break;

				case MSG_GEN_TYPE_FLOAT32:
					subsize+=4;
					break;

				case MSG_GEN_TYPE_FLOAT64:
					subsize+=8;
					break;

				case MSG_GEN_TYPE_BOOL:
				case MSG_GEN_TYPE_INT8:
				case MSG_GEN_TYPE_UINT8:
					subsize+=1;
					break;

				case MSG_GEN_TYPE_INT16:
				case MSG_GEN_TYPE_UINT16:
					subsize+=2;
					break;

				case MSG_GEN_TYPE_INT32:
				case MSG_GEN_TYPE_UINT32:
					subsize+=4;
					break;

				case MSG_GEN_TYPE_INT64:
				case MSG_GEN_TYPE_UINT64:
					subsize+=8;
					break;

			}

			//get string length
			if(str!=0)
				while(*str!='\0')
				{
					subsize++;
					++str;
				}

			size+=subsize;
			for(sub_size_storage=def; message_def[sub_size_storage].belongs_to_previous && sub_size_storage != 0; --sub_size_storage)
			{}
			message_def[sub_size_storage].size+=subsize;

			if(message_def[sub_size_storage].belongs_to_previous)
			printf("current_subsize: %i\n", subsize);
			printf("whole_subsize: %i\n", message_def[sub_size_storage].size);

		}
		else
		{





				switch(message_def[def].type)
				{
					case MSG_GEN_TYPE_WHOLE_LEN_BIN:

						break;

					case MSG_GEN_TYPE_WHOLE_LEN_STRING:

						break;

					case MSG_GEN_TYPE_BOOL:

						break;

					case MSG_GEN_TYPE_CHAR:


					case MSG_GEN_TYPE_STRING:

						break;



					case MSG_GEN_TYPE_TAG:

						break;


					case MSG_GEN_TYPE_CLOSE_TAG:

						break;


					case MSG_GEN_TYPE_ROSRPC_FIELD:

						break;

					case MSG_GEN_TYPE_INT8:
					case MSG_GEN_TYPE_UINT8:
						//bytes=1;
						break;


					case MSG_GEN_TYPE_INT16:
					case MSG_GEN_TYPE_UINT16:
						//bytes=2;
						//g_byte_order_correction_to_network->SIZE_2_B;
						break;


					case MSG_GEN_TYPE_INT32:
					case MSG_GEN_TYPE_UINT32:
					case MSG_GEN_TYPE_FLOAT32:
						//bytes=4;
						//g_byte_order_correction_to_network->SIZE_4_B;
						break;


					case MSG_GEN_TYPE_INT64:
					case MSG_GEN_TYPE_UINT64:
					case MSG_GEN_TYPE_FLOAT64:
						//bytes=8;
						//g_byte_order_correction_to_network->SIZE_8_B;
						break;


					case MSG_GEN_TYPE_END:

						break;
				}


		}





		if(part_finished || dryrun==true)
		{
			++def;
		}
		if(message_def[def].type == MSG_GEN_TYPE_END && dryrun==true)
		{
			printf("WHOLE MESSAGE: %i <--- ",size);
			def=1; //Message size calculated, now sending it ...
			dryrun=false;
		}

	}while (message_def[def].type != MSG_GEN_TYPE_END);
}
