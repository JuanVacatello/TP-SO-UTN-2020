#include "logGameBoy.h"


void iniciar_logger(){
	char* file = obtener_log_file();
	if((logger = log_create(file,"GAMEBOY",0,LOG_LEVEL_INFO)) == NULL){
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

/*
 * 1.   Conexión a cualquier proceso.
 * 2.   Suscripción a una cola de mensajes.
 * 3.   Llegada de un nuevo mensaje a una cola de mensajes.
*/

// 1. Conexión a cualquier proceso.
void log_conexion(char* proceso){
	char* log = string_from_format("GameBoy se conectó exitosamente al proceso %s.", proceso);
	completar_logger(log, "BROKER", LOG_LEVEL_INFO);
	free(log);
}

// 2. Suscripción a una cola de mensajes.
void log_suscripcion(char* cola){
	char* log = string_from_format("GameBoy se suscribió exitosamente a la cola %s.", cola);
	completar_logger(log, "BROKER", LOG_LEVEL_INFO);
	free(log);
}

// 3. Llegada de un nuevo mensaje a una cola de mensajes.
void log_mensaje_nuevo(char* cola){
	char* log = string_from_format("Llegada de un nuevo mensaje a la cola de mensajes %s.", cola);
	completar_logger(log, "GAMEBOY", LOG_LEVEL_INFO);
	//free(log);
}





