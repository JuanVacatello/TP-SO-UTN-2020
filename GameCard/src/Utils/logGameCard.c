#include "logGameCard.h"


void iniciar_logger(){
	if((logger = log_create("/home/utnso/Documentos/Delibird/GameCard/tp-2020-1c-wRAPPERS/GameCard/GameCard.log","GAMECARD",1,LOG_LEVEL_INFO )) == NULL){
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

// Inicio de proceso de reintento de comunicación con el Broker
// Resultado de proceso de reintento de comunicación con el Broker.



// Inicio de proceso de reintento de comunicación con el Broker
void log_intento_reintento_comunicacion_broker(){
	int tiempoReconexion = tiempo_de_reintento_conexion();
	char* mensaje = string_from_format("Reintentando conexion con BROKER en %d segundos...", tiempoReconexion);

	completar_logger(mensaje, "GAMECARD", LOG_LEVEL_INFO);
	free(mensaje);
}


// Resultado de proceso de reintento de comunicación con el Broker.
void log_intento_comunicacion_Broker_exitoso(){
	char* mensaje = string_from_format("Se logró conectar con el BROKER de manera exitosa");

	completar_logger(mensaje, "GAMECARD", LOG_LEVEL_INFO);
	free(mensaje);
}

void log_reintento_comunicacion_Broker_fallido(){
	char* mensaje = string_from_format("NO se logró conectar con el BROKER");

	completar_logger(mensaje, "GAMECARD", LOG_LEVEL_INFO);
	free(mensaje);
}

void log_asignar_bloque(int bloque){
	char* mensaje = string_from_format("Se asignó correctamente el bloque numero %d",bloque);

	completar_logger(mensaje, "GAMECARD", LOG_LEVEL_INFO);
	free(mensaje);
}

void log_liberar_bloque(int bloque){
	char* mensaje = string_from_format("Se libero correctamente el bloque numero %d",bloque);

	completar_logger(mensaje, "GAMECARD", LOG_LEVEL_INFO);
	free(mensaje);
}





