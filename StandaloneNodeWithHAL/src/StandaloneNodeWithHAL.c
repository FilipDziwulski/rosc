/*
 ============================================================================
 Name        : StandaloneNodeWithHAL.c
 Author      : Christian Holl
 Version     :
 Copyright   : SNCN_C
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "rosCnode/rosCnode.h"


int main(void)
{
	const char *custom_str[] =
	{
		 "custom_desc",
		 "custom_val",
		 "custom_tag",
		 "custom_text",
	};
	char buf_header[150];
	char buf_xml[850];
	unsigned int buf_header_index=0;
	unsigned int buf_xml_index=0;
	unsigned int xml_gen_index=0;


	ros_rpc_gen_command xml_gen_array[]=
	{
	RPC_XML_DECLARATION,
		RPC_TAG_METHODCALL,
			RPC_TAG_METHODNAME,
				RPC_STDTEXT_HASPARAM,
			RPC_CT RPC_TAG_METHODNAME,
			RPC_TAG_PARAMS,
				RPC_TAG_PARAM,
					RPC_CUSTOM_TAG+2,
						RPC_CUSTOM_TEXT+3,
					RPC_CT RPC_CUSTOM_TAG+2,
				RPC_CT RPC_TAG_PARAM ,
			RPC_CT RPC_TAG_PARAMS,
		RPC_CT RPC_TAG_METHODCALL,
		RPC_GENERATOR_FINISH
	};
	generateXML(buf_xml, xml_gen_array, custom_str, &xml_gen_index, &buf_xml_index);




	printf("%s\n",buf_xml);

	return 0;




}
