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
 *  msggen.c created by Christian Holl
 */

#include <rosc/sebs_parse_fw/send_modules/msggen.h>
#include <rosc/debug/debug_out.h>

sebs_parse_return_t sebs_msggen(sebs_parser_data_t* pdata)
{
	sebs_msggen_t *fdata=(sebs_msggen_t *)pdata->current_parser.parser_data;
	if(pdata->function_init)
	{
		pdata->sending=true;
		pdata->function_init=false;
		fdata->first_run=true;
		fdata->cmds.submode =MSG_GEN_MODE_TYPE;
		fdata->cmds.def_state = 0;
		fdata->finished=false;

		fdata->cmds.header_data=fdata->header_data_ptr;
		fdata->cmds.payload_data=fdata->payload_data_ptr;

		switch (fdata->type) {
		case MSGGEN_TYPE_XMLRPC_REQ_REGISTER_PUBLISHER:
		case MSGGEN_TYPE_XMLRPC_REQ_REGISTER_SUBSCRIBER:
		case MSGGEN_TYPE_XMLRPC_REQUEST_TOPIC:
				fdata->cmds.header=xmlrpc_hd_request;
				fdata->cmds.payload=xmlrpc_msg_request;
				break;

		case MSGGEN_TYPE_XMLRPC_RESPOND_REQUEST_TOPIC:
		case MSGGEN_TYPE_XMLRPC_ACK:
			fdata->cmds.header=xmlrpc_hd_response;
			fdata->cmds.payload=xmlrpc_msg_response;
			break;


		case MSGGEN_TYPE_HTTP_ERROR:
			fdata->cmds.header=xmlrpc_hd_response;
			fdata->cmds.payload=xmlrpc_msg_http_error;
			break;

		case MSGGEN_TYPE_ROSMSG_HEADER:
		fdata->cmds.header=rosrpc_hd;
		fdata->cmds.payload=rosrpc_topic_init;
		break;


		}

		fdata->cmds.type = fdata->cmds.header;
		fdata->cmds.data = fdata->cmds.header_data;
		fdata->cmds.size.payload_size=0;
		fdata->cmds.size.payload_size_available=false;
		fdata->cmds.out.curPos=0;
		fdata->cmds.size.mode=MSG_GEN_SIZE_MODE_NONE;

	}

	if(*pdata->len==SOCKET_SIG_DATA_SENT || fdata->first_run)
	{
		if(!fdata->finished)
		{
			fdata->first_run=false;
			XMLRPC_RESP_HD_RESET();
			XMLRPC_REQ_RESET();
			XMLRPC_RESP_RESET();
			switch(fdata->type)
			{
				case MSGGEN_TYPE_XMLRPC_RESPOND_REQUEST_TOPIC:
					xmlrpc_msg_response[27]=MSG_TYPE_NONE;
					xmlrpc_msg_response[34]=MSG_TYPE_NONE;
					xmlrpc_msg_response[41]=MSG_TYPE_NONE;
					break;

				case MSGGEN_TYPE_XMLRPC_REQ_REGISTER_SUBSCRIBER:
					xmlrpc_msg_request[3]=MSG_TYPE_METHODNAME_REGISTERSUBSCRIBER;
					xmlrpc_msg_request[12]=MSG_TYPE_NONE;
					xmlrpc_msg_request[19]=MSG_TYPE_NONE;
					xmlrpc_msg_request[43]=MSG_TYPE_NONE;
					break;

				case MSGGEN_TYPE_XMLRPC_REQ_REGISTER_PUBLISHER:
					xmlrpc_msg_request[3]=MSG_TYPE_METHODNAME_REGISTERPUBLISHER;
					xmlrpc_msg_request[12]=MSG_TYPE_NONE;
					xmlrpc_msg_request[19]=MSG_TYPE_NONE;
					xmlrpc_msg_request[43]=MSG_TYPE_NONE;
					break;

				case MSGGEN_TYPE_XMLRPC_ACK:
					break;

				case MSGGEN_TYPE_XMLRPC_REQUEST_TOPIC:
					xmlrpc_msg_request[3]=MSG_TYPE_METHODNAME_REQUESTTOPIC;
					xmlrpc_msg_request[12]=MSG_TYPE_NONE;
					xmlrpc_msg_request[53]=MSG_TYPE_NONE;
					break;

				case MSGGEN_TYPE_XMLRPC_ERROR:

					break;

				case MSGGEN_TYPE_HTTP_ERROR:
					xmlrpc_hd_response[2]=MSG_TYPE_STRING;
					xmlrpc_hd_response[8]=MSG_TYPE_VAL_TEXT_HTML;
					break;

				default:
					break;
			}


			pdata->out_len=fdata->buffer_size;
			pdata->out_buf=fdata->buffer;
			if(!msg_gen((uint8_t*)pdata->out_buf,&pdata->out_len,&fdata->cmds))
			{
				fdata->finished=true;
			}
		}
		else
		{
			pdata->out_len=SOCKET_SIG_NO_DATA;
			pdata->sending=false;
			return (SEBS_PARSE_RETURN_FINISHED);
		}
	}
	else
	{
		if(*pdata->len!=SOCKET_SIG_NO_DATA)
		{
			pdata->event=SEBS_MSGGEN_EVENT_SOCKET_SIGNAL;
		}
	}
	return (SEBS_PARSE_RETURN_GO_AHEAD);
}
