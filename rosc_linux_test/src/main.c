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
 *  main.c created by Christian Holl
 */

/* ******************/
/*     TESTING      */
/* ******************/
#include<rosc/sebs_parse_fw/send_modules/socket_connect.h>
#include<stdio.h>
#include<string.h>

/* ******************/


#include <rosc/rosc.h>

#include <rosc/msg/rosc_linux_test/simple1.h>
#include <rosc/msg/rosc_linux_test/simple2.h>

#include <rosc/com/ros_handler.h>

#include <rosc/com/msg_gen.h>
#include <rosc/system/eth.h>
#include <rosc/com/publish.h>


ROSC_STATIC_MSG_BUILDUP__rosc_linux_test__simple1();
ROSC_STATIC_MSG_BUILDUP__rosc_linux_test__simple2();

ROSC_STATIC_MSG_USER_DEF__rosc_linux_test__simple1(sim1, 3, 3, 2);
ROSC_STATIC_MSG_USER_DEF__rosc_linux_test__simple2(sim2);



ROSC_STATIC_SYSTEM_MESSAGE_TYPE_LIST_BEGIN
	ROSC_SIZE_LIST_ENTRY__rosc_linux_test__simple1(sim1);
	ROSC_SIZE_LIST_ENTRY__rosc_linux_test__simple2(sim2);
	ROSC_SIZE_LIST_ENTRY_MIN_XMLRPC_OUTPUT_BUFFER(100);
ROSC_STATIC_SYSTEM_MESSAGE_TYPE_LIST_END

ROSC_STATIC_CALLBACK_HEAD__rosc_linux_test__simple1(sim1,sub1)
	int i;
	printf("sub1\n");
	printf("s2 size: %i\n",msg->s2.size);
	printf("s2 oversize: %i\n",msg->s2.oversize);



	for(i=0;i<msg->s2.size;i++)
		printf("%i\n",msg->s2.data[i].ThirtyTwo);

}

ROSC_STATIC_CALLBACK_HEAD__rosc_linux_test__simple2(sim2,sub2)
	printf("simple2 callback\n");
}


ROSC_STATIC_SUBSCRIBER_INIT__rosc_linux_test__simple1(sim1, sub1,"/simple1")

ROSC_STATIC_SUBSCRIBER_INIT__rosc_linux_test__simple2(sim2, sub2,"/simple2")

ROSC_STATIC_PUBLISHER_INIT__rosc_linux_test__simple1(sim1, pub1, "/simple1Pub")


ROSC_STATIC_LOOKUP_TABLE_HEAD()
	ROSC_STATIC_LOOKUP_ENTRY(localhost,IP(127,0,0,1))
	ROSC_STATIC_LOOKUP_ENTRY(Computer,IP(192,168,0,3))
ROSC_STATIC_LOOKUP_TABLE_END

MASTER_URI_STATIC("http://localhost:11311");

NODE_NAME("roscnode");


extern void publisherfill(iface_t *interface, void *msg, socket_t* cur);

int main()
{
	printf("Socket Memory Statistics\n");
	printf("#############################################################\n");


	printf("Size of the ROS data struct in system part: %i bytes\n",(int) sizeof(ros_handler_data_t));
	printf("Size of the XMLRPC data struct in system part: %i bytes\n",(int) sizeof(xmlrpc_data_t));
	printf("------------------------------------------\n");
	printf("Overall size of the system part: %i bytes\n",(int) offsetof(rosc_socket_memory_size_def_t,message_data));
	printf("Size of the connect url length: %i bytes\n",  (int) rosc_static_url_max_size);
	printf("Size of the user defined part: %i bytes\n",(int) sizeof(rosc_socket_memory_size_def_t) - (int) offsetof(rosc_socket_memory_size_def_t,message_data));
	printf("------------------------------------------\n");
	printf("Overall size of a single socket: %i bytes\n",(int) sizeof(rosc_socket_memory_size_def_t));
	printf("\n");
	printf("Overall size selected %i sockets: %i bytes\n",(int) __SOCKET_MAXIMUM__,(int) sizeof(rosc_static_socket_mem));

	printf("#############################################################\n");
	printf("\n");

	printf("Lookup Table Memory \n");
	printf("#############################################################\n");
	printf("Maximum Hostname Size: %i bytes\n",(int) __HOSTNAME_BUFFER_LEN__);
	printf("Lookup Entry Size: %i bytes\n",(int) sizeof(lookup_table_entry_t));
	printf("------------------------------------------\n");
	printf("Lookup Table Size: (%i entries) %i bytes\n", (int)rosc_static_lookup_table_size , (int)rosc_static_lookup_table_size  * (int)sizeof(lookup_table_entry_t));
	printf("#############################################################\n");
	printf("\n");



	char mem[rosc_static_socket_mem_size];
	socket_t cur;

	sebs_parser_data_t pdata;

	rosc_static_msg_user_def__rosc_linux_test__simple1__sim1_t msg;



	cur.pdata.additional_storage=mem;
	memset(mem,0,rosc_static_socket_mem_size);



	msg.s2.size=2;
	msg.s2.oversize=1;
	msg.s2.data[0].ThirtyTwo=2222;
	printf("%x\n", (size_t)&msg.s2.data[0].ThirtyTwo);

	msg.s2.data[1].ThirtyTwo=3333;
	printf("%x\n", (size_t)&msg.s2.data[1].ThirtyTwo);


	msg.str.size=2;
	msg.str.data[0].size=2;
	msg.str.data[0].str_data[0]='a';
	msg.str.data[0].str_data[1]='b';

	msg.str.data[1].size=2;
	msg.str.data[1].str_data[0]='c';
	msg.str.data[1].str_data[1]='d';

	publisherfill(&pub1, &msg, &cur);
	printf("here!");




	//rosc_init();

	//register_interface(&sub1);
	//register_interface(&sub2);
	//register_interface(&pub1);

	//rosc_spin();
}
