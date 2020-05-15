#include "../Utils/log.h"

t_log* completar_logger(char* mensaje,char* programa, t_log_level log_level)
{
	t_log* logger;
	logger = log_create("Delibird.log",programa, 1, log_level);
	log_info(logger,mensaje);
	return logger;
}


