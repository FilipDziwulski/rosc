/*
 *  Copyright by Synapticon GmbH (www.synapticon.com)  ©2012
 *
 *  msg_strings.c
 *
 *  This file is part of ROScNode Library.
 *
 *  ROScNode Library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ROScNode Library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with ROScNode Library. If not, see <http://www.gnu.org/licenses/>.
 *
 *	File created by Christian Holl on 11.09.2012
 */



/*
 *
 */
const char *separators[]=
{
	"\\ .",		    //!< Message type separators
	" :\n",		    //!< Header separators
	" <>=\"\'"		//!< Full XML separators
};





/*
 * Note to editors: If you add something here make sure its also available (in the SAME ORDER)
 * in the ros_rpc_gen_command enum command block (alphabetically ordered!)
 */
const char *ros_xml_tag_strings[] =
{
	"array",
	"body",
	"boolean",
	"data",
	"fault",
	"h1",
	"head",
	"int",
	"member",
	"methodCall",
	"methodName",
	"methodResponse",
	"p",
	"param",
	"params",
	"string",
	"struct",
	"title",
	"value"
};

/*
 * Note to editors: If you add something here make sure its also available (in the SAME ORDER)
 * in the ros_rpc_gen_command enum command block
 */
const char *ros_xml_stdtext[] =
{
	"<?xml version=\"1.0\"?>",
	//"Error code "
	"hasParam",
	"registerPublisher",
	"registerSubscriber",
	"requestTopic",
	"/tcp_keepalive",
	"unregisterPublisher",
	"unregisterSubscriber"
};

/*
 * Note to editors: If you add something here make sure its also available (in the SAME ORDER)
 * in the http_head_gen_command enum command block (for numbers>=HTTP_HEADER_DESC_BEGIN)
 */
const char *http_header_descriptors[] =
{
		"Server",
		"User-Agent",
		"Date",
		"Host",
		"Content-Type",
		"Content-length",
		"Accepted-Encoding"
};

/*
 * Note to editors: If you add something here make sure its also available (in the SAME ORDER)
 * in the http_head_gen_command enum command block (for numbers>=HTTP_HEADER_VALUE_BEGIN)
 */
const char *http_header_stdtext[] =
{
		"POST/HTTP/1.1",
		"HTTP/1.0",
		"XMLRPC ROSc-NodeLib",
		"BaseHTTP/ROSc-NodeLib",
		"text/xml",
		"OK"
};

