/*
 *  Copyright by Synapticon GmbH (www.synapticon.com)  ©2012
 *
 *  msg_gen.c
 *
 *  This file is part of ROScNode Library.
 *
 *  ROScNode Library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ROScNode Library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with ROScNode Library. If not, see <http://www.gnu.org/licenses/>.
 *
 *	File created by Christian Holl on 11.09.2012
 */

#include "msg_gen.h"
#include "msg_strings.h"

void str2buf(unsigned int *index, char* buffer, const char* str, str2buf_modes mode)
{
	if(mode==S2B_CTAG || mode == S2B_TAG)//If tag
	{
		//Add first bracket
		buffer[*index]='<';
		(*index)++;

		if(mode==S2B_CTAG)
		{
			buffer[*index]='/';
			(*index)++;
		}
	}
	//process string
    while (*str != '\0')
    {
    	buffer[*index]=*str;
    	(*index)++;
        str++;
    }

    if(mode==S2B_TAG || mode==S2B_CTAG)
    {
    	//Add second bracket
    	buffer[*index]='>';
		(*index)++;
    }
    else if(mode==S2B_HTTP_HEAD_FIELD_DESC)
    {
    	buffer[*index]=':';
		(*index)++;
		buffer[*index]=' ';
		(*index)++;
    }
    else if(mode==S2B_HTTP_HEAD_FIELD)
	{
		buffer[*index]='\n';
		(*index)++;
	}
}


void int2buf(char* message_buffer, unsigned int *buf_index, unsigned int number)
{
	//count the places
	unsigned int a=10;
	while(number/a)a*=10;
	a/=10;
	while(a>0)
	{
		message_buffer[*buf_index]=(char)(number/a)+48;
		number%=a;
		a/=10;
		(*buf_index)++;
	}
}


unsigned int generateHTTPHeader(char* message_buffer, const http_head_gen_command* gen_array, const char **custom_string_array)
{
	unsigned int buf_index=0;
	while(*gen_array != HTTP_HEADER_GEN_END)
	{
		if((*gen_array) == HTTP_HEADER_GEN_CUSTOM_TEXT_END)
		{
			message_buffer[buf_index]='\n';
			buf_index++;
		}
		else if((*gen_array)>=HTTP_HEADER_GEN_VAL_UINT_NUMBER)
		{
			unsigned int number=(*gen_array)-HTTP_HEADER_GEN_VAL_UINT_NUMBER;
			int2buf(message_buffer,&buf_index,number);
		}
		else if((*gen_array)>=HTTP_HEADER_GEN_VAL_CUSTOM) //Print custom value, no newline...
		{
			str2buf(&buf_index,message_buffer,custom_string_array[(*gen_array)-HTTP_HEADER_GEN_VAL_CUSTOM],S2B_NORMAL);
		}
		else if((*gen_array)>=HTTP_HEADER_VALUE_BEGIN) //Print std value
		{
			str2buf(&buf_index,message_buffer,http_header_stdtext[(*gen_array)-HTTP_HEADER_VALUE_BEGIN],S2B_HTTP_HEAD_FIELD);
		}
		else if((*gen_array)>=HTTP_HEADER_GEN_DESC_CUSTOM) //Print custom descriptor
		{
			str2buf(&buf_index,message_buffer,custom_string_array[(*gen_array)-HTTP_HEADER_GEN_DESC_CUSTOM],S2B_HTTP_HEAD_FIELD_DESC);
		}
		else if((*gen_array)>=HTTP_HEADER_DESC_BEGIN) //Print std descriptor
		{
			str2buf(&buf_index,message_buffer,http_header_descriptors[(*gen_array)-HTTP_HEADER_DESC_BEGIN],S2B_HTTP_HEAD_FIELD_DESC);
		}
		gen_array++;
	}
	//Finish Header with empty line
	message_buffer[buf_index]='\n';
	buf_index++;
	return buf_index;
}



unsigned int generateXML(char* message_buffer, const ros_rpc_gen_command* gen_array, const char **custom_string_array)
{
	unsigned int buf_index=0;
	const char *outstring;
	int outmode;
	unsigned int number;
	int command=*gen_array;

	while(command != RPC_GENERATOR_FINISH)
	{
			/*
			 * UNSIGNED INTEGER
			 */
			if(command>=RPC_UINT_NUMBER)
			{
				number=command-RPC_UINT_NUMBER;
				outmode=-1;
			}
			/*
			 * TEXT FIELD
			 */
			else if(command>=RPC_STDTEXT_START-1) //Add text
			{
				if(command>=RPC_CUSTOM_TEXT)
				{
					outstring=custom_string_array[command-RPC_CUSTOM_TEXT];
					outmode=S2B_NORMAL;
				}
				else
				{
					outstring=ros_rpc_stdtext[command-RPC_STDTEXT_START-1];
					outmode=S2B_NORMAL;
				}
			}
			/*
			 * CLOSING TAG
			 */
			else if(command>=RPC_CLOSE_TAG)
			{
				if(command>=RPC_CUSTOM_TAG+RPC_CLOSE_TAG)
				{
					outstring=custom_string_array[command-RPC_CUSTOM_TAG-RPC_CLOSE_TAG];
					outmode=S2B_CTAG;
				}
				else
				{
					outstring=ros_rpc_tag_strings[command-RPC_TAGS_START-1-RPC_CLOSE_TAG];
					outmode=S2B_CTAG;
				}
			}
			/*
			 * OPENING TAG
			 */
			else if(command>=RPC_TAGS_START-1)
			{
				if(command>=RPC_CUSTOM_TAG)
				{
					outstring=custom_string_array[command-RPC_CUSTOM_TAG];
					outmode=S2B_TAG;
				}
				else
				{
					outstring=ros_rpc_tag_strings[command-RPC_TAGS_START-1];
					outmode=S2B_TAG;
				}
			}

			if(outmode<0)
			{
				int2buf(message_buffer,&buf_index,number);
			}
			else
			{
				str2buf(&buf_index, message_buffer,outstring,outmode);
			}

			gen_array++;
			command=*gen_array;
	}
	return buf_index;
}
