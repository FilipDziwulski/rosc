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
 *  numberparse.h created by Christian Holl
 */

#ifndef NUMBERPARSE_H_
#define NUMBERPARSE_H_

#include <rosc/msg_parsers/xml_parser_structure.h>



#define PARSE_SUBMODE_INIT_NUMBERPARSE(PARSE_STRUCT,MAX_FIGURES)\
				PARSE_STRUCT->submode=PARSE_SUBMODE_NUMBERPARSE;\
				PARSE_STRUCT->submode_state=PARSE_SUBMODE_INIT;\
				PARSE_STRUCT->submode_data.numberParse.figure_max=MAX_FIGURES


typedef enum
{
	NUMBERPARSE_MAX_FIGURES,
	NUMBERPARSE_ANOTHER_CHAR,
	NUMBERPARSE_ERROR_NONUMBER,
}numberparse_result_t;

#ifndef FORCE_INLINE
	void numberparse(char **buf_ptr, uint32_t *len_ptr, parse_act_t *pact);
#endif

#endif /* NUMBERPARSE_H_ */
