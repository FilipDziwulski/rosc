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
 *  msg_gen_handler.c created by Christian Holl
 */

#include <rosc/sebs_parse_fw/gen_modules/msg_gen_handler.h>
#include <rosc/sebs_parse_fw/gen_modules/size_deter.h>
#include <rosc/sebs_parse_fw/gen_modules/buffer_fill.h>


sebs_parse_return_t msg_gen_handler(sebs_parser_data_t* pdata)
{
	msg_gen_handler_data_t *hdata=pdata->handler_data;
	msg_gen_handler_init_t *idata=pdata->init_data;
	if(pdata->handler_init)
	{
		DEBUG_PRINT_STR("MSG GEN HANDLER INIT");
		pdata->handler_init=false;
		pdata->function_init=true;
		pdata->return_to_handler=false;
		pdata->overall_len=0;
		pdata->security_len=0;
		hdata->handler_state=MSG_GEN_HANDLER_STATE_SIZE;
		hdata->buffer_size=pdata->len; //Get buffer len
	}


	switch (hdata->handler_state)
	{
		case MSG_GEN_HANDLER_STATE_SIZE:

			break;

		case MSG_GEN_HANDLER_STATE_OUTPUT:

			break;

		default:
			break;
	}



	return (SEBS_PARSE_RETURN_GO_AHEAD);
}

