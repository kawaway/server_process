#include <stdlib.h>
#include "uv.h"
#include "connection.h"
#include "http_parser.h"
#include "request.h"


int on_url(http_parser * parser, const char *at, size_t lenght);
void on_write(uv_write_t * req, int status);
void on_close(uv_handle_t * handle);


static http_parser_settings g_settings = {
	.on_url = on_url
};

http_parser_settings *request_init(void)
{
	return &g_settings;
}


void on_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf)
{
	if(nread < 0) {
		/*
		if(nread  == UV_EOF) {

		}
		*/
		if(buf->base) free(buf->base);
	} else if (nread == 0) {
		free(buf->base);
	}
	struct Connection *con = (struct Connection *)stream;
	struct Server *srv = con->srv;
	/*
	Request_t *req = req->parser = calloc(1, sizeof(*parser));
	if (req->parser == NULL) {

		//free ?
	}
	*/
	http_parser *parser = calloc(1, sizeof(*parser));
	if (parser == NULL) {
		//free ?
	}
	
	http_parser_init(parser, HTTP_REQUEST);
	parser->data = con;
	http_parser_execute(parser, srv->http_settings, buf->base ,buf->len);
	

}

struct Http_status_code{
	int code;
	char *str;
};

	
static const struct Http_status_code g_status_code2status_str[] = {
	{ 200, "OK" },
	{ 404, "Not Found" },
	{ 500, "Internal Server Error" }
};

static const char *get_status_str(const int status_code)
{
	for(int i = 0; ;i++) {
		if (g_status_code2status_str[i].code == status_code) {
			return g_status_code2status_str[i].str;

		}
		if (g_status_code2status_str[i].code == 500) {
			return g_status_code2status_str[i].str;
		}
	}
}

int header_text(char *buf, int buf_len, const int status_code, const int content_length)
{
	const char *status = get_status_str(status_code);
	
	int nwrite = snprintf(buf, buf_len,
			      "HTTP/1.1 %d %s\r\n"
			      "Content-Length: %d\r\n"
			      "Content-type: text/plain; charset=UTF-8;\r\n"
			      "Connection: close\r\n"
			      "\r\n",
			      status_code,
			      status,
			      content_length
		);
	return nwrite;
}

_Bool response_hello(Connection_t *con, http_parser * parser)
{
	uv_buf_t buffs[2];
	char header_buf[256];
	char *body = "hello world!\r\n";
	int content_length = strnlen(body, sizeof(header_buf));
	int header_length = 0;
	
	Response_t *resp = calloc(1, sizeof(*resp));
	if( resp == NULL ) {
		return 0;
	}
	resp->parser = parser;
	
	header_length = header_text(header_buf, sizeof(header_buf)/sizeof(header_buf[0]),
				    200, content_length);
	buffs[0] = uv_buf_init(header_buf, header_length);
	buffs[1] = uv_buf_init(body, content_length);
	
	uv_write(&resp->req, &con->stream, buffs, 2, on_write);
	return 1;
}

_Bool response_404(Connection_t *con, http_parser * parser)
{
	uv_buf_t buffs[2];
	char header_buf[256];
	char *body = "Not Found!\r\n";
	int content_length = strnlen(body, sizeof(header_buf));
	int header_length = 0;
	
	Response_t *resp = calloc(1, sizeof(*resp));
	if( resp == NULL ) {
		return 0;
	}
	resp->parser = parser;
	
	header_length = header_text(header_buf, sizeof(header_buf)/sizeof(header_buf[0]),
				    404, content_length);
	buffs[0] = uv_buf_init(header_buf, header_length);
	buffs[1] = uv_buf_init(body, content_length);
	
	uv_write(&resp->req, &con->stream, buffs, 2, on_write);
	return 1;
}

/*
_Bool proc_random(Connection_t *con, http_parser * parser)
{
	Response_t *resp = calloc(1, sizeof(*resp));
	if( resp == NULL ) {
		return 0;
	}
	resp->parser = parser;
	
	uv_fs_event_init(con->srv->loop, &resp->fs_event);
	uv_fs_event_start(&resp->fs_event, cb, "/dev/random", );

	return 1;
}
*/

int on_url(http_parser * parser, const char *at, size_t length)
{
	Connection_t *con = (Connection_t *)parser->data;
//	Server_t *srv = con->srv;
	
	if (strncmp("/", at, length) == 0) {
		response_hello(con, parser);
	} else if  (strncmp("/hello", at, length) == 0) {
		response_hello(con, parser);
	} else if  (strncmp("/random", at, length) == 0) {
		/* not implemented yet */
		response_404(con, parser);
	} else {
		response_404(con, parser);
	}
	
	return 1;
}

void on_write(uv_write_t *req, int status)
{
	if (status < 0 ) {
		//eroor
	}
	Response_t *resp = (Response_t *)req;
	
	uv_close((uv_handle_t *)resp->parser->data, on_close);
}

void on_close(uv_handle_t *handle)
{
	free(handle);
}
