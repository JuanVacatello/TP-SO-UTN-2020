#include "logTeam.h"



t_log* iniciar_logger(){
	t_log* logger;
	char** file = obtener_log_file();
	if((logger =! log_create("Team.log","TEAM",0,LOG_LEVEL_INFO )) == NULL){
		printf("No se pudo crear el log");
		exit(1);
	}
	else
		return logger;
}

t_log* completar_logger(char* mensaje, char* programa, t_log_level log_level)
{
	t_log* logger;

	logger = log_create("archivo", programa, 1, log_level);
	log_info(logger,mensaje);
	return logger;
}

