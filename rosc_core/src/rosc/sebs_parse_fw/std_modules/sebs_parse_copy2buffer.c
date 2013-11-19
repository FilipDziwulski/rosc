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
 *  copy2buffer.c created by Christian Holl
 */


#include <rosc/sebs_parse_fw/std_modules/sebs_parse_copy2buffer.h>
#include <rosc/debug/debug_out.h>

sebs_parse_return_t sebs_parse_copy2buffer(sebs_parser_data_t* pdata)
{
	sebs_parse_copy2buffer_data_t *fdata=(sebs_parse_copy2buffer_data_t *)pdata->current_parser.parser_data;
	if(pdata->function_init)
	{
		pdata->function_init=false;
		fdata->cur_pos=0;
		fdata->byteorder_pos=0;
	}


	int8_t *correct=fdata->byteOrderCorrection;
	char *buffer_c=fdata->buffer;

#ifdef __DEBUG__PRINTS__
	if(fdata->endChrs==0 && fdata->max_len==0)
	{
		DEBUG_PRINT_STR("ERROR copy2buf -> Will copy an endless amount of chars!!");
		while(1);
	}
#endif

	while(*pdata->len > 0)
	{
		const char *sep=fdata->endChrs;
		bool isEndChar=false;
		if(sep!=0)
		{
			while(*sep!='\0')
			{
				if(**pdata->buf==*sep)
				{
					isEndChar=true;
					break;
				}
				++sep;
			}
		}

		if((fdata->cur_pos<(fdata->max_len)) && !isEndChar )
		{
			if(correct==0)
				buffer_c[fdata->cur_pos]=**pdata->buf;
			else //Byte order correction
			{
				buffer_c[fdata->cur_pos+correct[fdata->byteorder_pos]]=**pdata->buf;
				if(fdata->byteorder_pos < fdata->repeats && fdata->repeats!=0)
					++fdata->byteorder_pos;
				else
					fdata->byteorder_pos=0;
			}

			++fdata->cur_pos;
			++*pdata->buf;
			--*pdata->len;
		}
		else
		{
			if(isEndChar)
			{
				fdata->result=COPY2BUFFER_ENDCHR;
			}
			else
			{
				fdata->result=COPY2BUFFER_MAXLEN;
			}
			if(fdata->is_string)buffer_c[fdata->cur_pos]='\0';

			return (SEBS_PARSE_RETURN_FINISHED); //Finished!
		}
	}
	if((fdata->cur_pos>=(fdata->max_len)))
	{
		return (SEBS_PARSE_RETURN_FINISHED);
	}


	return (SEBS_PARSE_RETURN_GO_AHEAD); //Not finished yet
}


