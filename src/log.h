#ifndef _LOG_H_
#define _LOG_H_

#include <stdio.h>
#include <stdarg.h>

typedef enum {
	LOG_T_STDOUT = 0,
	LOG_T_SYSLOG,
	LOG_T_MEM,
} log_type_t;

typedef enum {
	LOG_L_DEBUG = 0,
	LOG_L_INFO,
	LOG_L_WARN,
	LOG_L_ERR,

	LOG_L_MAX,
} log_level_t;

typedef enum {
	LOG_K_NONE = 0x00000000,
	LOG_K_UV   = 0x00000001,
	LOG_K_ANY  = 0xffffffff,
} log_kind_t;


struct Log {
	log_type_t  type;
  	log_level_t level;
	log_kind_t  kind;
};


/* public function */
struct Log *log_init(const char *name, log_type_t type, log_level_t level, log_kind_t kind);
#define ERROR(log, kind, fmt, ...) log_write(log, LOG_L_ERR,   kind, "%s:%d "fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define WARN(log, kind, fmt, ...)  log_write(log, LOG_L_WARN,  kind, "%s:%d "fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define INFO(log, kind, fmt, ...)  log_write(log, LOG_L_INFO,  kind, "%s:%d "fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define DEBUG(log, kind, fmt, ...) log_write(log, LOG_L_DEBUG, kind, "%s:%d "fmt, __FILE__, __LINE__, ##__VA_ARGS__)
void log_set_option(struct Log *log, log_level_t level, log_kind_t kind);
void log_end(struct Log *log);


/* internal function */
void log_write(struct Log *log, log_level_t level, log_kind_t kind, char *fmt, ...);

#endif
