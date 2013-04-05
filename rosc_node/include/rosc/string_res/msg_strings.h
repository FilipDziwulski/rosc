/*
 *  WARNING WARNING WARNING WARNING
 *    FILE GENERATED AUTOMATICALLY
 *  WARNING WARNING WARNING WARNING
 */
#ifndef MSG_STRINGS_H_
#define MSG_STRINGS_H_ 
 
 
/**
 * Length of the string array rpc_xml_attribute_strings
 */
#define RPC_XML_ATTRIBUTE_STRINGS_LEN 	1

/**
 * Macro for inserting standard attributes for XMLRPC (None available just a "dummy" for the parser) string numbers into any enum
 */
#define __RPC_XML_ATTRIBUTE_STRINGS(ENUM) \
	 ENUM ## _ATTRIBUTE_DUMMY

/**
 * This array contains the strings for standard attributes for XMLRPC (None available just a "dummy" for the parser)
 */
extern const char *rpc_xml_attribute_strings[];


/**
 * Length of the string array http_header_stdtext
 */
#define HTTP_HEADER_STDTEXT_LEN 	8

/**
 * Macro for inserting some standard values for http headers string numbers into any enum
 */
#define __HTTP_HEADER_STDTEXT(ENUM) \
	 ENUM ## _VAL_BASEHTTP_ROSC_NODELIB, \
	 ENUM ## _VAL_HTTP1_0, \
	 ENUM ## _VAL_HTTP1_1, \
	 ENUM ## _VAL_OK, \
	 ENUM ## _VAL_METHOD_POST, \
	 ENUM ## _VAL_XMLRPC_ROSC_NODELIB, \
	 ENUM ## _VAL_HTTP_URL_HEAD, \
	 ENUM ## _VAL_TEXT_XML

/**
 * This array contains the strings for some standard values for http headers
 */
extern const char *http_header_stdtext[];


/**
 * Length of the string array http_header_descriptors
 */
#define HTTP_HEADER_DESCRIPTORS_LEN 	7

/**
 * Macro for inserting standard http header descriptors string numbers into any enum
 */
#define __HTTP_HEADER_DESCRIPTORS(ENUM) \
	 ENUM ## _DESC_ACCEPTED_ENCODING, \
	 ENUM ## _DESC_CONTENT_TYPE, \
	 ENUM ## _DESC_CONTENT_LENGTH, \
	 ENUM ## _DESC_DATE, \
	 ENUM ## _DESC_HOST, \
	 ENUM ## _DESC_SERVER, \
	 ENUM ## _DESC_USER_AGENT

/**
 * This array contains the strings for standard http header descriptors
 */
extern const char *http_header_descriptors[];


/**
 * Length of the string array http_header_content_types
 */
#define HTTP_HEADER_CONTENT_TYPES_LEN 	2

/**
 * Macro for inserting contains known content types string numbers into any enum
 */
#define __HTTP_HEADER_CONTENT_TYPES(ENUM) \
	 ENUM ## _CONTENT_GZIP, \
	 ENUM ## _CONTENT_TEXT_HTML

/**
 * This array contains the strings for contains known content types
 */
extern const char *http_header_content_types[];


/**
 * Length of the string array rpc_xml_stdtext
 */
#define RPC_XML_STDTEXT_LEN 	10

/**
 * Macro for inserting standard XMLRPC text pieces string numbers into any enum
 */
#define __RPC_XML_STDTEXT(ENUM) \
	 ENUM ## _STDTXT_TCP_KEEPALIVE, \
	 ENUM ## _STDTXT_XML_DEF, \
	 ENUM ## _STDTXT_ERROR_CODE, \
	 ENUM ## _STDTXT_HASPARAM, \
	 ENUM ## _STDTXT_HTTP_URL_HEAD, \
	 ENUM ## _STDTXT_REGISTERPUBLISHER, \
	 ENUM ## _STDTXT_REGISTERSUBSCRIBER, \
	 ENUM ## _STDTXT_REQUESTTOPIC, \
	 ENUM ## _STDTXT_UNREGISTERPUBLISHER, \
	 ENUM ## _STDTXT_UNREGISTERSUBSCRIBER

/**
 * This array contains the strings for standard XMLRPC text pieces
 */
extern const char *rpc_xml_stdtext[];


/**
 * Length of the string array parse_separators
 */
#define PARSE_SEPARATORS_LEN 	3

/**
 * Macro for inserting seperators for parsing different pieces of a XMLRPC message string numbers into any enum
 */
#define __PARSE_SEPARATORS(ENUM) \
	 ENUM ## _SEP_METHOD, \
	 ENUM ## _SEP_XML, \
	 ENUM ## _SEP_HTTP_HEADER

/**
 * This array contains the strings for seperators for parsing different pieces of a XMLRPC message
 */
extern const char *parse_separators[];


/**
 * Length of the string array http_available_targets
 */
#define HTTP_AVAILABLE_TARGETS_LEN 	2

/**
 * Macro for inserting contains available targets for http requests string numbers into any enum
 */
#define __HTTP_AVAILABLE_TARGETS(ENUM) \
	 ENUM ## _TARGET_ROOT, \
	 ENUM ## _TARGET_TEST

/**
 * This array contains the strings for contains available targets for http requests
 */
extern const char *http_available_targets[];


/**
 * Length of the string array rpc_xml_tag_strings
 */
#define RPC_XML_TAG_STRINGS_LEN 	17

/**
 * Macro for inserting standard XMLRPC tags string numbers into any enum
 */
#define __RPC_XML_TAG_STRINGS(ENUM) \
	 ENUM ## _TAG_COMMENT, \
	 ENUM ## _TAG_CDDATA, \
	 ENUM ## _TAG_ARRAY, \
	 ENUM ## _TAG_BOOLEAN, \
	 ENUM ## _TAG_DATA, \
	 ENUM ## _TAG_FAULT, \
	 ENUM ## _TAG_INT, \
	 ENUM ## _TAG_MEMBER, \
	 ENUM ## _TAG_METHODCALL, \
	 ENUM ## _TAG_METHODNAME, \
	 ENUM ## _TAG_METHODRESPONSE, \
	 ENUM ## _TAG_PARAM, \
	 ENUM ## _TAG_PARAMS, \
	 ENUM ## _TAG_STRING, \
	 ENUM ## _TAG_STRUCT, \
	 ENUM ## _TAG_VALUE, \
	 ENUM ## _TAG_XMLDEF

/**
 * This array contains the strings for standard XMLRPC tags
 */
extern const char *rpc_xml_tag_strings[];


/**
 * Length of the string array http_methods
 */
#define HTTP_METHODS_LEN 	2

/**
 * Macro for inserting method strings string numbers into any enum
 */
#define __HTTP_METHODS(ENUM) \
	 ENUM ## _METHOD_GET, \
	 ENUM ## _METHOD_POST

/**
 * This array contains the strings for method strings
 */
extern const char *http_methods[];


#endif /* MSG_STRINGS_H_ */