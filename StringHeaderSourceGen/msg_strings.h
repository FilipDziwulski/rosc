/*
 *  WARNING WARNING WARNING WARNING
 *    FILE GENERATED AUTOMATICALLY
 *  WARNING WARNING WARNING WARNING
 *
 *  Copyright by Synapticon GmbH (www.synapticon.com)  ©2012
 *
 *  msg_strings.h
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
 *    File Creator by Christian Holl
 *
 */
#ifndef MSG_STRINGS_H_
#define MSG_STRINGS_H_ 
 
 
/**
 * Macro for inserting standard XMLRPC tags string numbers into any enum
 */
#define __RPC_XML_TAG_STRINGS(ENUM) \
	 ENUM ## _TAG_ARRAY, \
	 ENUM ## _TAG_BODY, \
	 ENUM ## _TAG_BOOLEAN, \
	 ENUM ## _TAG_DATA, \
	 ENUM ## _TAG_FAULT, \
	 ENUM ## _TAG_H1, \
	 ENUM ## _TAG_HEAD, \
	 ENUM ## _TAG_INT, \
	 ENUM ## _TAG_MEMBER, \
	 ENUM ## _TAG_METHODCALL, \
	 ENUM ## _TAG_METHODNAME, \
	 ENUM ## _TAG_METHODRESPONSE, \
	 ENUM ## _TAG_P, \
	 ENUM ## _TAG_PARAM, \
	 ENUM ## _TAG_PARAMS, \
	 ENUM ## _TAG_STRING, \
	 ENUM ## _TAG_STRUCT, \
	 ENUM ## _TAG_TITLE, \
	 ENUM ## _TAG_VALUE

/**
 * This array contains the strings for standard XMLRPC tags
 */
extern const char *rpc_xml_tag_strings[];


/**
 * Macro for inserting standard XMLRPC text pieces string numbers into any enum
 */
#define __RPC_XML_STDTEXT(ENUM) \
	 ENUM ## _STDTXT_TCP_KEEPALIVE, \
	 ENUM ## _STDTXT_XML_DEF, \
	 ENUM ## _STDTXT_ERROR CODE, \
	 ENUM ## _STDTXT_HASPARAM, \
	 ENUM ## _STDTXT_REGISTERPUBLISHER, \
	 ENUM ## _STDTXT_REGISTERSUBSCRIBER, \
	 ENUM ## _STDTXT_REQUESTTOPIC, \
	 ENUM ## _STDTXT_UNREGISTERPUBLISHER, \
	 ENUM ## _STDTXT_UNREGISTERSUBSCRIBER

/**
 * This array contains the strings for standard XMLRPC text pieces
 */
extern const char *rpc_xml_stdtext[];


#endif /* MSG_STRINGS_H_ */