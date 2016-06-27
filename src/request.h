#ifndef _REQUEST_H_
#define _REQUEST_H_
#include "uv.h"
#include "http_parser.h"

/*
struct Request 
{
	http_parser parser;
	Server_t *srv;
};
*/

typedef struct Response Response_t;

struct Response
{
	uv_write_t req;
	http_parser *parser;
	uv_fs_event_t fs_event;
};

	

http_parser_settings *request_init(void);
void on_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf);



#endif
