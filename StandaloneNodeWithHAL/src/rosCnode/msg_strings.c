/*
 *  Copyright by Synapticon GmbH (www.synapticon.com)  ©2012
 *
 *  msg_strings.c
 *
 *  This file is part of StandaloneNodeWithHAL.
 *
 *  StandaloneNodeWithHAL is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  StandaloneNodeWithHAL is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with StandaloneNodeWithHAL. If not, see <http://www.gnu.org/licenses/>.
 *
 *	File created by choll on 11.09.2012
 */

/*
 * Note to editors: If you add something here make sure its also available (in the SAME ORDER)
 * in the ros_rpc_gen_command enum command block (for numbers>2000) (make it alphabetically ordered, maybe necessary!)
 */
const char *ros_rpc_tag_strings[] =
{
	"array",
	"boolean",
	"data",
	"int",
	"methodcall",
	"methodname",
	"methodresponse",
	"param",
	"params",
	"string",
	"value"
};

/*
 * Note to editors: If you add something here make sure its also available (in the SAME ORDER)
 * in the ros_rpc_gen_command enum command block (for numbers>4000) (make it alphabetically ordered, maybe necessary!)
 */
const char *ros_rpc_stdtext[] =
{
	"hasParam",
	"registerPublisher",
	"registerSubscriber",
	"requestTopic",
	"tcp_keepalive",
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
		"POST / HTTP/1.1",
		"XMLRPC ROSc-NodeLib",
		"BaseHTTP/ROSc-NodeLib",
		"text/xml",
		"HTTP/1.0",
		"OK"
};

