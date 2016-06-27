#ifndef _LISTENER_H_
#define _LISTENER_H_

#include <sys/types.h>
#include <sys/socket.h>
#include "ngx_queue.h"
#include "uv.h"
#include "log.h"

typedef struct Resolver Resolver_t;

struct Resolver {
	uv_getaddrinfo_t uv;
	struct Server *srv;
};

struct Listener {
	uv_tcp_t tcp;
	ngx_queue_t queue;
	struct Server *srv;
};

struct Resolver **resolver_init(struct Server *srv);
void on_resolve(uv_getaddrinfo_t *uv_res, int status, struct addrinfo *addr);
void resolver_end(struct Server *srv, struct Resolver **resolver);

struct Listener *listener_create(void);
_Bool listener_add(struct Server *srv, struct Listener *tail, struct addrinfo *addr);
void listener_destory(struct Listener *res);

#endif
