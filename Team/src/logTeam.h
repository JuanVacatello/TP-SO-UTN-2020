#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>

t_log* logger;

t_log* iniciar_logger();
t_log* completar_logger(char* mensaje,char* programa, t_log_level log_level);
