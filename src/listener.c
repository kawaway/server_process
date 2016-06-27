#include <stdlib.h>
#include "core.h"
#include "listener.h"
#include "connection.h"

void on_resolve(uv_getaddrinfo_t *res, int status, struct addrinfo *addr)
{
//	struct Resolver *resolver = _CONTAINER_OF(Resolver_t, uv, res);
	struct Resolver *resolver = (struct Resolver *)res;
	struct Server *srv = resolver->srv;
	_Bool ret = 0;
	
	/* canceled */
	if (status == UV_ECANCELED) {
		return;
	}
	
	ret = listener_add(srv, srv->listener, addr);
	if (ret == 0) {
		INFO(srv->log, LOG_K_UV, "listenning\n");
	}
}

struct Resolver **resolver_init(struct Server *srv)
{
	struct Resolver **resolver = NULL;
//	uv_getaddrinfo_t **resolver = NULL;
	struct Conf *conf = srv->conf;
	struct addrinfo hints;
	int resolver_num = 0;
	int i = 0;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family   = PF_UNSPEC;
	hints.ai_flags = AI_ADDRCONFIG | AI_NUMERICSERV | AI_PASSIVE;
	
	resolver = calloc(conf->resolve_num, sizeof(*resolver));
	if (resolver == NULL) goto error;

	for(i = 0; i < conf->resolve_num; ) {
		resolver[i] = calloc(1, sizeof(*resolver[i]));
		if (resolver[i] == NULL) continue;
		i++;
	}
	resolver_num = i;
	
	for(int i = 0; i < resolver_num; ) {
		int ret = uv_getaddrinfo(srv->loop, &resolver[i]->uv, on_resolve,
					 conf->resolve[i].host, conf->resolve[i].service, &hints);
		if (ret < 0 ) {
			free(resolver[i]);
			resolver[i] = NULL;
			ERROR(srv->log, LOG_K_ANY, "failed to getaddrinfo\n");
		}
		resolver[i]->srv = srv;
		i++;
	}
	if( i > resolver_num) {
		struct Resolver **swap = calloc(i, sizeof(*swap));
		int swap_i = 0;
		int res_i = swap_i;
		if (swap == NULL) goto error;
		for(; swap_i < i; ) {
			if( resolver[res_i]) {
				swap[swap_i] = resolver[res_i];
				swap_i++;
			}
			res_i++;
		}
		free(resolver);
		resolver = swap;
	}
	
	srv->resolver_num = i;
	return resolver;
error:
	// uv_getaddrinfo cancel
	for(int i = 0; i< resolver_num; i++) {
		if(resolver[i]) free(resolver[i]);
	}
	if(resolver) free(resolver);
	return NULL;
}

void resolver_end(struct Server *srv, struct Resolver **resolver)
{
	// uv_getaddrinfo cancel
	for(int i = 0; i< srv->resolver_num; i++) {
		if(resolver[i]) free(resolver[i]);
	}
	if(resolver) free(resolver);
}


struct Listener *listener_create(void)
{
	struct Listener *res = NULL;
	res = calloc(1, sizeof(*res));
	if (res == NULL) return NULL;

	ngx_queue_init(&res->queue);

	return res;
}

_Bool listener_add(struct Server *srv, struct Listener *tail, struct addrinfo *addr)
{
	struct Listener *lis = NULL;
	int res = 0;
	
	lis = calloc(1, sizeof(*lis));
	if (lis == NULL) {
		ERROR(srv->log, LOG_K_UV, "failed to calloc\n");
		goto error;
	}
	lis->srv = srv;
	res = uv_tcp_init(srv->loop, &lis->tcp);
	if (res < 0) {
		ERROR(srv->log, LOG_K_UV, "failed to tcp init\n");
		goto error;
	}
	res = uv_tcp_bind(&lis->tcp, addr->ai_addr, 0);
	if (res < 0) {
		ERROR(srv->log, LOG_K_UV, "failed to tcp bind\n");
		goto error;
	}
	res = uv_tcp_simultaneous_accepts(&lis->tcp, 1);
	if (res < 0) {
		ERROR(srv->log, LOG_K_UV, "failed to tcp simultaneous accept\n");
		goto error;
	}
	res = uv_listen((uv_stream_t*)&lis->tcp, srv->conf->listen_max, on_connect);
	if(res < 0) {
		ERROR(srv->log, LOG_K_UV, "failed to tcp listen \n");
		goto error;
	}
	
	ngx_queue_insert_tail(&tail->queue, &lis->queue);
	return 1;
error:
	// tcp cancel
	if (lis) free(lis);
	return 0;
	
}

void listener_destory(struct Listener *res)
{
	//for
	free(res);
}
