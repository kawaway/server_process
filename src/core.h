#ifndef _CORE_H_
#define _CORE_H_

#include <sys/types.h>
#include <sys/socket.h>
#include "uv.h"
#include "log.h"
#include "request.h"

typedef struct Server Server_t;


struct Resolve_conf {
//	int  type;
	char *host;
	char *service;
};

struct Conf {
	int resolve_num;
	struct Resolve_conf *resolve;
	int listen_max;
};

struct Server {
	uv_loop_t* loop;
	struct Conf *conf;

	int resolver_num;
	struct Resolver **resolver;
	int listener_num;
	struct Listener *listener;
	 struct Connection *con;
	struct Log *log;
	http_parser_settings *http_settings;
};

#define _CONTAINER_OF(type, member, ptr) ((type *) ((char *)ptr - offsetof(type, member)))


#endif
