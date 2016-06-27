#include <stdlib.h>
#include "request.h"
#include "http_parser.h"
#include "core.h"
#include "connection.h"
#include "listener.h"

void on_connect(uv_stream_t *uv_stream, int status)
{
//	struct Listener *listener = _CONTAINER_OF(struct Listener, uv, res);
	struct Listener *lis = (struct Listener *)uv_stream;
	struct Server *srv = lis->srv;

	_Bool ret = 0;
	
	/* canceled */
	if (status < 0) {
		INFO(srv->log, LOG_K_UV, "failed to connect. %s %s\n",
		     uv_err_name(status), uv_strerror(status));
		return;
	}

	ret = connection_add(srv->con, lis);
	if (ret == 0) {
		INFO(srv->log, LOG_K_UV, "failed to connect\n");
	}
}

void on_alloc(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
	buf->base = calloc(1, suggested_size);
	buf->len  = suggested_size;
}

struct Connection *connection_create(void)
{
	struct Connection *con = NULL;
	con = calloc(1, sizeof(*con));
	if (con == NULL) return NULL;

	ngx_queue_init(&con->queue);

	return con;
}

_Bool connection_add(struct Connection *tail, struct Listener *lis)
{
	struct Connection *con = NULL;
	struct Server *srv = lis->srv;
	int res = 0;
	
	con = calloc(1, sizeof(*lis));
	if (con == NULL) {
		ERROR(srv->log, LOG_K_UV, "failed to calloc\n");
		goto error;
	}
	res = uv_tcp_init(srv->loop, (uv_tcp_t *)&con->stream);
	if (res < 0) {
		ERROR(srv->log, LOG_K_UV, "failed to tcp init\n");
		goto error;
	}
	res = uv_tcp_simultaneous_accepts((uv_tcp_t *)&con->stream, 1);
	if (res < 0) {
		ERROR(srv->log, LOG_K_UV, "failed to tcp simultaneous accept¥n");
		goto error;
	}
	res = uv_accept((uv_stream_t *)&lis->tcp, &con->stream);
	if( res < 0 ) {
		ERROR(srv->log, LOG_K_UV, "failed to accept ¥n");
		goto error;
	}
	res = uv_tcp_nodelay((uv_tcp_t *)&con->stream, 1);
	if( res < 0 ) {
		ERROR(srv->log, LOG_K_UV, "failed to tcp nodelay ¥n");
		goto error;
	}
	res = uv_read_start(&con->stream, on_alloc, on_read);
		if( res < 0 ) {
		ERROR(srv->log, LOG_K_UV, "failed to start read ¥n");
		goto error;
	}
	ngx_queue_insert_tail(&tail->queue, &lis->queue);
	con->srv = srv;
	
	return 1;
error:
	// tcp cancel
	if (con) free(con);
	return 0;
	
}

void connection_destory(struct Connection *con)
{
	//for
	free(con);
}
