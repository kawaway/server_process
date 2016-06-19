#include <syslog.h>
#include <stdlib.h>
#include "log.h"

static const int g_log2syslog[LOG_L_MAX] = {
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERR,
};

struct Log *log_init(const char *name, log_type_t type, log_level_t level, log_kind_t kind)
{
	struct Log *new = NULL;

	new = calloc(1, sizeof(*new));
	if (new == NULL) {
		return NULL;
	}
	new->type  = type;
	new->level = level;
	new->kind  = kind;
	
	switch(type) {
		case LOG_T_SYSLOG:
			openlog(name, LOG_PID, LOG_USER);
			break;
		default:
			break;
	}
	return new;
}

void log_set_option(struct Log *log, log_level_t level, log_kind_t kind) {
	log->level = level;
	log->kind  |= kind;	
}

void log_write(struct Log *log, log_level_t level, log_kind_t kind, char *fmt, ...)
{
	va_list ap;
	
	if (log->level > level) return;
	if ((log->kind & kind) == LOG_K_NONE) return;
	
	switch(log->type) {
		case LOG_T_SYSLOG:
			va_start(ap, fmt);
			vsyslog(g_log2syslog[log->level], fmt, ap);
			va_end(ap);
			break;
		case LOG_T_STDOUT:
			va_start(ap, fmt);
			vprintf(fmt,ap);
			va_end(ap);
			break;
		default:
			break;
	}
}

void log_end(struct Log *log)
{
	switch(log->type) {
		case LOG_T_SYSLOG:
			closelog();
			break;
		default:
			break;
	}
	free(log);
}
