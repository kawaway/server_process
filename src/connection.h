#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <sys/types.h>
#include <sys/socket.h>
#include "ngx_queue.h"
#include "uv.h"
#include "log.h"

typedef struct Connection Connection_t;
struct Connection {
	uv_stream_t stream;
	ngx_queue_t queue;
	struct Server *srv;
};

struct Connection *connection_create(void);
_Bool connection_add(struct Connection *tail, struct Listener *lis);
void on_connect(uv_stream_t *uv_stream, int status);
void connection_destory(struct Connection *res);



#endif
