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
 *  skipuntilchar.c created by Christian Holl on 20.03.2013
 */


#ifndef FORCE_INLINE
	#ifndef ENABLE_C
		#define ENABLE_C
	#endif
	#include <rosc/com_xml/parse/sub/skipuntilchar.h>
#endif


#ifndef FORCE_INLINE
	void skipuntilchar(char **buf_ptr, uint32_t *len_ptr, parse_act_t *pact)
	 //work around for inlining the function
#endif
#ifdef ENABLE_C
{
	#ifndef FORCE_INLINE
		uint32_t len=*len_ptr;
		char *buf=*buf_ptr;
	#endif

		pact->submode_state=PARSE_SUBMODE_RUNNING;
		while(len > 0)
		{
			bool isSpecChar=false;
			const char *sep=pact->submode_data.skipUntilChr.chrs;
			while(*sep!='\0')
			{
				if(*buf==*sep)
				{
					isSpecChar=true;
					pact->submode_data.skipUntilChr.fndchr=*sep;
					break;
				}
				++sep;
			}

			if((isSpecChar && pact->submode_data.skipUntilChr.negative) ||
					(!isSpecChar && !pact->submode_data.skipUntilChr.negative))
			{
				pact->submode_state=PARSE_SUBMODE_FINISHED;
				pact->submode=PARSE_SUBMODE_NONE;
				if(pact->submode_data.skipUntilChr.negative)
				{
					buf++;
					len--;
				}
				break;
			}
			else
			{
				buf++;
				len--;
			}
		}
	#ifndef FORCE_INLINE
		*len_ptr=len;
		*buf_ptr=buf;
	#endif
}
#endif

