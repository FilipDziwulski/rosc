#ifndef HTTP_HEADER_PARSING_H_
#define HTTP_HEADER_PARSING_H_

#include <rosc/string_res/msg_strings.h>

typedef enum
{
	__HTTP_HEADER_DESCRIPTORS(PARSE_HTTP)
}parse_http_desc_t;


//"POST / HTTP/1.1\n"
typedef enum
{
	PARSE_HTTP_STATE_METHSTR_BEGIN,
	PARSE_HTTP_STATE_METHSTR_METHODSTR,
	PARSE_HTTP_STATE_METHSTR_SKPSPC0,
	PARSE_HTTP_STATE_METHSTR_BCKSLSH0,
	PARSE_HTTP_STATE_METHSTR_SKPSPC1,
	PARSE_HTTP_STATE_METHSTR_HTTP,
	PARSE_HTTP_STATE_METHSTR_SKPSPC1,
	PARSE_HTTP_STATE_METHSTR_BCKSLSH1,
	PARSE_HTTP_STATE_METHSTR_SKPSPC2,
	PARSE_HTTP_STATE_METHSTR_MAIN_VER,
	PARSE_HTTP_STATE_METHSTR_POINT,
	PARSE_HTTP_STATE_METHSTR_SUB_VER,
	PARSE_HTTP_STATE_METHSTR_LF,
	PARSE_HTTP_STATE_DESCRIPTOR,
	PARSE_HTTTP_STATE_FIELD,
}parse_http_state_t;

#endif /* HTTP_HEADER_PARSING_H_ */
