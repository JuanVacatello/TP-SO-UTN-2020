#include "logTeam.h"
#include "entrenador.h"

void iniciar_logger(){
	char** file = obtener_log_file();
	if((logger = log_create("Team.log","TEAM",0,LOG_LEVEL_INFO )) == NULL){
		printf("No se pudo crear el log");
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
 * 1. Cambio de un entrenador de cola de planificación (indicando la razón del porqué).
 * 2. Movimiento de un entrenador (indicando la ubicación a la que se movió).
 * 3. Operación de atrapar (indicando la ubicación y el pokemon a atrapar).
 * 4. Operación de intercambio (indicando entrenadores involucrados).
 * 5. Inicio de algoritmo de detección de deadlock.
 * 6. Resultado de algoritmo de detección de deadlock.
 * 7. Llegada de un mensaje (indicando el tipo del mismo y sus datos).
 * 8. Resultado del Team (especificado anteriormente).
 * 9. Errores de comunicación con el Broker (indicando que se realizará la operación por default).
 * 10. Inicio de proceso de reintento de comunicación con el Broker.
 * 11. Resultado de proceso de reintento de comunicación con el Broker.
 */



//2. Movimiento de un entrenador (indicando la ubicación a la que se movió).

void log_movimiento_entrenador(t_entrenador* entrenador){
	//entrenador ya movido
	char* xPosicion = string_itoa(entrenador->posicion->x);
	char* yPosicion = string_itoa(entrenador->posicion->y);

	char* mensaje = string_from_format("El entrenador se movió a la posicion %s|%s.", xPosicion, yPosicion);

	completar_logger(mensaje, "TEAM", LOG_LEVEL_INFO);
}


















