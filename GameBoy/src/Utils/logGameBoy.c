#include "logGameBoy.h"


void iniciar_logger(){
	//char* file = obtener_log_file();
	//if((logger = log_create(file,"GAMEBOY",0,LOG_LEVEL_INFO )) == NULL){

	if((logger = log_create("GameBoy.log","GAMEBOY",1,LOG_LEVEL_INFO )) == NULL){
		printf("No se pudo crear el log del Gameboy.");
		exit(1);
	}
}

void completar_logger(char* mensaje, char* programa, t_log_level log_level)
{
	switch(log_level){
	case LOG_LEVEL_TRACE:
		log_trace(logger,mensaje);
		break;
	case LOG_LEVEL_DEBUG:
		log_debug(logger, mensaje);
		break;
	case LOG_LEVEL_INFO:
		log_info(logger,mensaje);
		break;
	case LOG_LEVEL_WARNING:
		log_warning(logger,mensaje);
		break;
	case LOG_LEVEL_ERROR:
		log_error(logger, mensaje);
	}
}
