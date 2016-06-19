#include <stdio.h>
#include "log.h"
#include "uv.h"
#include "http_parser.h"

int main(void)
{
	struct Log *log = NULL;

	log = log_init("server", LOG_T_STDOUT, LOG_L_DEBUG, LOG_K_ANY);
	if (log == NULL) {
		return -1;
	}

	printf("hello ");

	ERROR(log, LOG_K_ANY, "world");
	log_end(log);
	return 0;
}
