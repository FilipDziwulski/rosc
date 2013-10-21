
#include <rosc/sebs_parse_fw/send_modules/msggen.h>

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
		fdata->cmds.type = fdata->cmds.header;
		fdata->cmds.data = fdata->cmds.header_data;
		fdata->cmds.size.payload_size=0;
		fdata->cmds.size.payload_size_available=false;
		fdata->cmds.out.curPos=0;
		fdata->cmds.size.mode=MSG_GEN_SIZE_MODE_NONE;

	}

	if(*pdata->len==SOCKET_SIG_DATA_SENT || fdata->first_run)
	{
		fdata->first_run=false;
		XMLRPC_REQ_RESET();
		XMLRPC_RESP_RESET();


//		extern msg_gen_type_t rosrpc_hd[];
//		extern msg_gen_type_t rosrpc_msg_topic_init[];
//		extern msg_gen_type_t xmlrpc_hd_request[];
//		extern msg_gen_type_t xmlrpc_hd_response[];
//		extern msg_gen_type_t xmlrpc_msg_request[];
//		extern msg_gen_type_t xmlrpc_msg_response[];
//		extern msg_gen_type_t xmlrpc_msg_error[];

		switch (fdata->type) {
			case MSGGEN_TYPE_XMLRPC_REQ_REGISTER_PUBLISHER_TOPIC:
				fdata->cmds.header=xmlrpc_hd_request;
				fdata->cmds.payload=xmlrpc_msg_request;
				break;

			case MSGGEN_TYPE_XMLRPC_ERROR:
				fdata->cmds.header=xmlrpc_hd_response;
				fdata->cmds.payload=xmlrpc_msg_error;
				break;

			case MSGGEN_TYPE_XMLRPC_REQUEST_TOPIC:

				break;
			default:
				break;
		}

		pdata->out_len=fdata->buffer_size;
		if(!msg_gen(fdata->buffer,&pdata->out_len,&fdata->cmds))
		{
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
