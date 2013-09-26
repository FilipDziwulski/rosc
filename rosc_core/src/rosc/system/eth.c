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
 *  eth.c created by Christian Holl
 */


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
 *  ports.c created by Christian Holl
 */

#include <rosc/system/eth.h>
#include <rosc/system/status.h>


#ifndef __SYSTEM_HAS_MALLOC__
	#ifndef PORTS_STATIC_MAX_NUMBER
		#error No port setting macro defined, define PORTS_DYNAMIC or PORTS_STATIC_MAX_NUMBER <maximal ports>
	#endif

	//Memory for the port structs itself
	static socket_t __port_mem_reservation[PORTS_STATIC_MAX_NUMBER];

	//point the hub pointer to the first array entry
	static socket_t port_list_hub;

	//external memory (defined by STATIC_SYSTEM_MESSAGE_TYPE_LIST in rosc_init.h)
	extern void* rosc_static_port_mem;
	extern const size_t rosc_static_port_mem_size;
	extern const size_t rosc_static_port_mem_message_offset;
	extern const size_t rosc_static_port_mem_hdata_offset;

#else
	socket_t __port_list_hub;
	const socket_t* port_list_hub=__port_list_hub;
#endif


void rosc_sockets_init()
{
	//Init list hub
	port_list_hub.data=0;
	port_list_hub.interface=0;
	port_list_hub.socket_id=0;
	port_list_hub.port_number=0;
	port_list_hub.type=PORT_TYPE_HUB;
	port_list_hub.state=SOCKET_STATE_UNUSABLE;
	port_list_hub.next=0;

	//Init for static systems on
	#ifndef __SYSTEM_HAS_MALLOC__
		port_list_hub.next=(socket_t*)__port_mem_reservation;
		int i;
		for(i=0;i<PORTS_STATIC_MAX_NUMBER;++i)
		{
			__port_mem_reservation[i].next=(socket_t *)__port_mem_reservation+sizeof(socket_t)*(i+1);
			__port_mem_reservation[i].data=(void*)(rosc_static_port_mem+rosc_static_port_mem_size*i);
			__port_mem_reservation[i].interface=0;
			__port_mem_reservation[i].socket_id=0;
			__port_mem_reservation[i].port_number=0;
			__port_mem_reservation[i].type=PORT_TYPE_UNUSED;
			__port_mem_reservation[i].state=SOCKET_STATE_CLOSED;
		}
		__port_mem_reservation[i-1].next=0; //Set last items next address to zero
	#endif
}

bool rosc_use_socket( iface_t *iface, uint16_t port_number)
{
	socket_t *cur=port_list_hub.next;
	while(1)
	{
		if(cur->next==0)break;
		if(cur->state!=SOCKET_STATE_CLOSED ||
			cur->state!=SOCKET_STATE_UNUSABLE) break;
		cur=cur->next;
	}
#ifndef __SYSTEM_HAS_MALLOC__
	if(cur->state==SOCKET_STATE_CLOSED)
	{
		cur->interface=iface;
		cur->pdata.handler_init=true;
		cur->pdata.init_data=iface->init_data;
		cur->pdata.handler_data=cur->data+rosc_static_port_mem_hdata_offset;
		cur->pdata.additional_storage=cur->data+rosc_static_port_mem_message_offset;
		cur->pdata.handler_function=iface->handler_function;
		cur->pdata.communication_port=cur;
	}
	else
	{
		return (false);
	}
#else
	//TODO allocate new port memory and stuff here

	//Needs additions for type sizes of ros message...
#endif


	cur->port_number=port_number;
	cur->socket_id=1;  //TODO REMOVE BEFORE DOING ANY SERIOUS!!!
	return (true);
}



void rosc_receive_by_socketid(uint32_t socket_id, uint8_t *buffer, uint32_t len)
{
	socket_t *cur=port_list_hub.next;
	while(1)
	{
		if(cur->next==0)break;
		if(cur->socket_id==socket_id) break;
		cur=cur->next;
	}
	sebs_parser_frame(buffer,len,&cur->pdata);
}


static iface_t interface_list_hub;


void rosc_init_interface_list()
{
	interface_list_hub.isListHub=true;
	interface_list_hub.next=0;
}


void register_interface(iface_t *interface)
{
	iface_t* cur=&interface_list_hub;
	//Go to the end of the list
	while(cur->next != 0 && cur->next != interface) cur=cur->next;

	if(cur->next != interface && cur->next == 0)
	{
		cur->next=interface;
		cur->next->next=0;
		cur->state=IFACE_STATE_DO_REGISTER;
	}
}


void unregister_interface(iface_t *interface)
{
	iface_t* cur=&interface_list_hub;
	while(cur->next != 0 && cur->next != interface) cur=cur->next;

	//TODO I guess some additional stuff (states) must be done here ... but currently I leave it like that.
	if(cur == interface)
	{
		cur->state=IFACE_STATE_DO_UNREGISTER;
	}
}

void remove_interface(iface_t *interface)
{
	iface_t* cur=&interface_list_hub;
	iface_t* last;
	//Go to the entry of the list
	while(cur && cur != interface)
	{
		last=cur;
		cur=cur->next;
	}

	if(cur==interface)
	{
		last->next=cur->next;
	}
}


