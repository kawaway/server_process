#include <stdio.h>
#include <assert.h>
#include "log.h"


int main(void)
{
	struct Log *log = log_init("driver", LOG_T_STDOUT, LOG_L_ERR, LOG_K_ANY);
	int i = 0;
	assert(log);
	ERROR(log, LOG_K_ANY, "seq OK 1¥n");
	WARN(log,  LOG_K_ANY, "seq OK 2¥n");
	INFO(log,  LOG_K_ANY, "seq OK 3¥n");
	DEBUG(log, LOG_K_ANY, "seq OK 4¥n");
	log_end(log);

	log = log_init("driver", LOG_T_SYSLOG, LOG_L_ERR, LOG_K_ANY);
	assert(log);
	ERROR(log, LOG_K_ANY, "seq OK 5¥n");
	WARN(log,  LOG_K_ANY, "seq OK 6¥n");
	INFO(log,  LOG_K_ANY, "seq OK 7¥n");
	DEBUG(log, LOG_K_ANY, "seq OK 8¥n");
	log_end(log);
	
	log = log_init("driver", LOG_T_STDOUT, LOG_L_ERR, LOG_K_ANY);
	assert(log);
	i = 4;
	ERROR(log, LOG_K_ANY, "seq OK %d¥n", i++);
	WARN(log,  LOG_K_ANY, "seq NG %d¥n", i++);
	INFO(log,  LOG_K_ANY, "seq NG %d¥n", i++);
	DEBUG(log, LOG_K_ANY, "seq NG %d¥n", i++);

	log_set_option(log, LOG_L_WARN, LOG_K_ANY);
	ERROR(log, LOG_K_ANY, "seq OK %d¥n", i++);
	WARN(log,  LOG_K_ANY, "seq OK %d¥n", i++);
	INFO(log,  LOG_K_ANY, "seq NG %d¥n", i++);
	DEBUG(log, LOG_K_ANY, "seq NG %d¥n", i++);

	log_set_option(log, LOG_L_INFO, LOG_K_ANY);
	ERROR(log, LOG_K_ANY, "seq OK %d¥n", i++);
	WARN(log,  LOG_K_ANY, "seq OK %d¥n", i++);
	INFO(log,  LOG_K_ANY, "seq OK %d¥n", i++);
	DEBUG(log, LOG_K_ANY, "seq NG %d¥n", i++);

	log_set_option(log, LOG_L_DEBUG, LOG_K_ANY);
	ERROR(log, LOG_K_ANY, "seq OK %d¥n", i++);
	WARN(log,  LOG_K_ANY, "seq OK %d¥n", i++);
	INFO(log,  LOG_K_ANY, "seq OK %d¥n", i++);
	DEBUG(log, LOG_K_ANY, "seq OK %d¥n", i++);
	log_end(log);
	
	log = log_init("driver", LOG_T_SYSLOG, LOG_L_ERR, LOG_K_ANY);
	assert(log);
	ERROR(log, LOG_K_ANY, "seq OK %d¥n", i++);
	WARN(log,  LOG_K_ANY, "seq NG %d¥n", i++);
	INFO(log,  LOG_K_ANY, "seq NG %d¥n", i++);
	DEBUG(log, LOG_K_ANY, "seq NG %d¥n", i++);

	log_set_option(log, LOG_L_WARN, LOG_K_ANY);
	ERROR(log, LOG_K_ANY, "seq OK %d¥n", i++);
	WARN(log,  LOG_K_ANY, "seq OK %d¥n", i++);
	INFO(log,  LOG_K_ANY, "seq NG %d¥n", i++);
	DEBUG(log, LOG_K_ANY, "seq NG %d¥n", i++);

	log_set_option(log, LOG_L_INFO, LOG_K_ANY);
	ERROR(log, LOG_K_ANY, "seq OK %d¥n", i++);
	WARN(log,  LOG_K_ANY, "seq OK %d¥n", i++);
	INFO(log,  LOG_K_ANY, "seq OK %d¥n", i++);
	DEBUG(log, LOG_K_ANY, "seq NG %d¥n", i++);

	log_set_option(log, LOG_L_DEBUG, LOG_K_ANY);
	ERROR(log, LOG_K_ANY, "seq OK %d¥n", i++);
	WARN(log,  LOG_K_ANY, "seq OK %d¥n", i++);
	INFO(log,  LOG_K_ANY, "seq OK %d¥n", i++);
	DEBUG(log, LOG_K_ANY, "seq OK %d¥n", i++);
	log_end(log);
	
	/*	*/
	return 0;
}
