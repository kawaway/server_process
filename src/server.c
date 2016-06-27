#include <stdio.h>
#include <stdlib.h>
#include "core.h"
#include "listener.h"
#include "connection.h"
//#include "http_parser.h"


struct Resolve_conf g_resolve_conf[] = {
		{
			.host    = "127.0.0.1",
			.service = "8080",
		},
		{
			.host    = "::1",
			.service = "8080",
		},
};


struct Conf g_conf = {
	.resolve_num = 2,
	.resolve     = &g_resolve_conf[0],
	.listen_max = 65535,
};


struct Server * server_init(void)
{
	struct Server *srv = NULL;
	struct Log *log = NULL;
	uv_loop_t* loop = NULL;
	struct Resolver **resolver = NULL;
	struct Listener *listener = NULL;
	struct Connection *con = NULL;
	struct Conf *conf = &g_conf;
	http_parser_settings *http_settings;
	
	
	srv = calloc(1, sizeof(*srv));
	if (srv == NULL) goto error;

	srv->conf = conf;
	
	log = log_init("server", LOG_T_STDOUT, LOG_L_DEBUG, LOG_K_ANY);
	if (log == NULL) goto error;
	srv->log = log;
	
	loop = uv_default_loop();
	if (loop == NULL) goto error;
	srv->loop = loop;
	
	resolver = resolver_init(srv);
	if (resolver == NULL) goto error;
	srv->resolver = resolver;
	
	listener = listener_create();

	if (listener == NULL) goto error;
	srv->listener = listener;

	con = connection_create();
	if (con == NULL)  goto error;
	srv->con = con;

	http_settings = request_init();
	srv->http_settings = http_settings;
	
	INFO(srv->log, LOG_K_ANY, "server started\n");
	return srv;

error:
	if (con)  connection_destory(con);
	if (listener)  listener_destory(listener);
	if (srv && resolver)  resolver_end(srv, resolver);
	if (log)  log_end(log);
	if (loop) uv_loop_close(loop);
	if (srv)  free(srv);
	return NULL;
}

void server_end(struct Server *srv)
{
	INFO(srv->log, LOG_K_ANY, "server stopped\n");
	for (int i=0; i< srv->resolver_num; i++) {
		free(srv->resolver[i]);
	}
	free(srv->resolver);
	if (srv) free(srv);
}


int main(void)
{
	struct Server *srv = NULL;
	int ret = 0;
	
	srv = server_init();
	if (srv == NULL) {
		return -1;
	}
	
	ret = uv_run(srv->loop, UV_RUN_DEFAULT);


	server_end(srv);
	return 0;
}
