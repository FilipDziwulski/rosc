#include <stdio.h>
#include <rosc/com_xml/parse/parser.h>
#include <rosc/com_xml/parse/sub/seekstring.h>
#include <rosc/com_xml/parse/sub/copy2buffer.h>
#include <rosc/com_xml/parse/sub/parseurl.h>
#include <rosc/com_xml/parse/sub/numberparse.h>
#include <rosc/com_xml/parse/sub/skipuntilchar.h>
#include <rosc/com_xml/parse/sub/skipwholemessage.h>

//TODO inline???
void xmlrpc_parse_act_init(parse_act_t *pact, xmlrpc_parser_type_t type, void * handler_data_storage)
{
	pact->handler_data_storage=handler_data_storage;
	pact->mode=PARSE_MODE_HEADER;
	pact->submode=PARSE_SUBMODE_NONE;
	switch (type) {
		case  XMLRPC_SERVER:
			pact->handler_fkt=&xmlrpc_server_handler;
			break;
		case XMLRPC_CLIENT:
			printf("TODO: IMPLEMENT!");
			break;
		default:
			break;
	}

}

void xmlrpc_parse(char *buf, uint32_t len, parse_act_t* pact)
{
	bool chunk_processed=false;
	while(!chunk_processed)
	{
		if(pact->submode==PARSE_SUBMODE_NONE || pact->submode_finished)
		{
			switch(pact->mode)
			{
			case PARSE_MODE_HEADER:

				break;
			case PARSE_MODE_XML:

				break;
			default:
				//TODO Error
				break;
			}
		}
		else
		{
			switch (pact->submode) {
				case PARSE_SUBMODE_COPY2BUFFER:

					break;

				case PARSE_SUBMODE_NUMBERPARSE:

					break;

				case PARSE_SUBMODE_SEEKSTRING:
						#ifdef FORCE_INLINE
							#define ENABLE_C
								#include "sub/seekstring.c"
							#undef ENABLE_C
						#else
							seekstring(buf, &len, pact);
						#endif
					break;

				case PARSE_SUBMODE_SKIPWHOLEMESSAGE:

					break;

				case PARSE_SUBMODE_SKIPUNTILCHAR:

					break;
				default:
					break;
			}
		}
	}
}



//	pact->handler_fkt(pact);
