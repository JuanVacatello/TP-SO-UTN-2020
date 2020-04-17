#include "log.h"

t_log* completar_logger(char* mensaje,char* programa)
{
	t_log* logger;
	logger = log_create("tp0.log",programa, 1, LOG_LEVEL_INFO);
	log_info(logger,mensaje);
	return logger;
}
