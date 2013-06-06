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
 *  sebs_parse_ros.h created by Christian Holl
 */

#ifndef SEBS_PARSE_ROS_H_
#define SEBS_PARSE_ROS_H_

#include <endian.h>
#include <rosc/sebs_parse_fw/sebs_parser_frame.h>

#define SEBS_PARSE_ROS_INIT(PARSER_DATA, DATA_STORAGE)\
		PARSER_DATA->next_parser.parser_function=(sebs_parse_function_t) &sebs_parse_ros;\
		PARSER_DATA->next_parser.parser_data=(void *)(&DATA_STORAGE);\
		return (SEBS_PARSE_RETURN_INIT_ADV)

typedef enum
{
	SEBS_PARSE_ROS_MODE_CONNECTION_ROSRPC,
	SEBS_PARSE_ROS_MODE_CONNECTION_TOPIC_HEADER,
	SEBS_PARSE_ROS_MODE_CONNECTION_TOPIC_BINARY,
}sebs_parse_ros_mode_t;

typedef enum
{
	SEBS_PARSE_ROSPRC_MESSAGE_LENGTH,
	SEBS_PARSE_ROSRPC_FIELD_LENGTH,
	SEBS_PARSE_ROSRPC_FIELD_ID,
	SEBS_PARSE_ROSRPC_FIELD_VALUE,

	//Topic
	SEBS_PARSE_ROSTOPIC_SUBMESSAGE_LENGTH,
	SEBS_PARSE_ROSTOPIC_ARRAY_LENGTH,
	SEBS_PARSE_ROSTOPIC_VALUE,

}sebs_parse_ros_state_t;


typedef enum
{

	SEBS_PARSE_ROS_EVENT_RPC_FIELD_ID,
	SEBS_PARSE_ROS_EVENT_RPC_FIELD_START,

}sebs_parse_ros_event_t;

typedef struct
{
	sebs_parse_ros_state_t state;
	sebs_parse_ros_mode_t mode;

	union
	{
		bool	boolean;

		uint8_t uint8;
		uint16_t uint16;
		uint32_t uint32;
		uint64_t uint64;

		int8_t int8;
		int16_t int16;
		int32_t int32;
		int64_t int64;

		float64_t float64;
		float32_t float32;
	}parsed_value;

}sebs_parse_ros_data_t;



sebs_parse_return_t sebs_parse_ros(sebs_parser_data_t* pdata);

#endif /* SEBS_PARSE_ROS_H_ */
