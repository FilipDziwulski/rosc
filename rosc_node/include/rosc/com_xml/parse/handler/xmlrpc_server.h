#ifndef XMLRPC_SERVER_H_
#define XMLRPC_SERVER_H_

#include <rosc/com_xml/parse/parser_structure.h>
#include <rosc/com_xml/parse/handler/xmlrpc_string_id.h>
#include <rosc/string_res/msg_strings.h>


typedef struct xmlrpc_server_data_t
{
	xmlrpc_server_method_t method;
	xmlrpc_server_target_t target;
	xmlrpc_slave_api_method_t rpcmethod;
	parse_xml_tags_t last_open_tag;
	union rpcmethodData
	{
		struct
		{
			char caller_id[__NODENAME_MAX_LEN__];
		}getBusStats;

		struct
		{
			char caller_id[__NODENAME_MAX_LEN__];
		}getBusInfo;

		struct
		{
			char caller_id[__NODENAME_MAX_LEN__];
		}getMasterUri;

		struct
		{
			char caller_id[__NODENAME_MAX_LEN__];
		}shutdown;

		struct
		{
			char caller_id[__NODENAME_MAX_LEN__];
		}getPid;

		struct
		{
			char caller_id[__NODENAME_MAX_LEN__];
		}getSubscriptions;

		struct
		{
			char caller_id[__NODENAME_MAX_LEN__];
		}getPublications;

		struct
		{
			char caller_id[__NODENAME_MAX_LEN__];
			char parameter_key[__ROS_PARAMETER_MAX_LEN__];
		}paramUpdate;

		struct
		{
			char caller_id[__NODENAME_MAX_LEN__];
			char parameter_key[__ROS_PARAMETER_MAX_LEN__];
		}publisherUpdate;

		struct
		{
			char caller_id[__NODENAME_MAX_LEN__];
			char parameter_key[__ROS_PARAMETER_MAX_LEN__];
		}requestTopic;
	};
}xmlrpc_server_data_t;


void xmlrpc_server_handler(parse_act_t * pact);

#endif /* XMLRPC_SERVER_H_ */
