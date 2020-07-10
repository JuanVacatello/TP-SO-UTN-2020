#include "logTeam.h"
#include "entrenador.h"

void iniciar_logger(){
	char* file = obtener_log_file();
	if((logger = log_create(file,"TEAM",1,LOG_LEVEL_INFO )) == NULL){
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

//1. Cambio de un entrenador de cola de planificación (indicando la razón del porqué).
void log_cambio_de_entrenador(t_entrenador* entrenador1, t_entrenador* entrenador2){}

//2. Movimiento de un entrenador (indicando la ubicación a la que se movió).
void log_movimiento_entrenador(t_entrenador* entrenador){
	//entrenador ya movido
	char idEntrenador = entrenador->ID_entrenador;
	int xPosicion = entrenador->posicion.x;
	int yPosicion = entrenador->posicion.y;

	char* mensaje = string_from_format("El entrenador %c se movió a la posicion %d|%d.",idEntrenador , xPosicion, yPosicion);

	completar_logger(mensaje, "TEAM", LOG_LEVEL_INFO);
}

//3. Operación de atrapar (indicando la ubicación y el pokemon a atrapar).
void log_operacion_de_atrapar_exitosa(t_entrenador* entrenador){
	//Si atrapó al pokemon
	char idEntrenador = entrenador->ID_entrenador;
	char* pokemonAtrapado = entrenador->pokemon_a_atrapar->especie;
	int xPosicion = entrenador->pokemon_a_atrapar->posicion.x;
	int yPosicion = entrenador->pokemon_a_atrapar->posicion.y;

	char* mensaje = string_from_format("El entrenador %c atrapó un %s en la posicion %d|%d.", idEntrenador, pokemonAtrapado, xPosicion, yPosicion);

	completar_logger(mensaje, "TEAM", LOG_LEVEL_INFO);
}

void log_operacion_de_atrapar_fallida(t_entrenador* entrenador){
	//No atrapó al pokemon
	char idEntrenador = entrenador->ID_entrenador;
	char* pokemonAtrapado = entrenador->pokemon_a_atrapar->especie;
	int xPosicion = entrenador->pokemon_a_atrapar->posicion.x;
	int yPosicion = entrenador->pokemon_a_atrapar->posicion.y;

	char* mensaje = string_from_format("El entrenador %c no pudo atrapar un %s en la posicion %d|%d.", idEntrenador, pokemonAtrapado, xPosicion, yPosicion);

	completar_logger(mensaje, "TEAM", LOG_LEVEL_INFO);
}

//4. Operación de intercambio (indicando entrenadores involucrados).
void log_operacion_de_intercambio(t_entrenador* entrenador1, t_entrenador* entrenador2,char* pokemon1, char* pokemon2){
	char idEntrenador1 = entrenador1->ID_entrenador;
	char idEntrenador2 = entrenador2->ID_entrenador;

	char* mensaje = string_from_format("El entrenador %c intercambio un %s por un %s del entrenador %c", idEntrenador1, pokemon1, pokemon2, idEntrenador2);

	completar_logger(mensaje, "TEAM", LOG_LEVEL_INFO);
}


//5. Inicio de algoritmo de detección de deadlock.
void log_inicio_deteccion_deadlock(){

	char* mensaje = string_from_format("Inicio de algoritmo de deteccion de Deadlock");

	completar_logger(mensaje, "TEAM", LOG_LEVEL_INFO);
}

//6. Resultado de algoritmo de detección de deadlock.
void log_deadlock_detectado(){

	char* mensaje = string_from_format("Se detecto que hay situacion de deadlock");

	completar_logger(mensaje, "TEAM", LOG_LEVEL_INFO);
}

void log_deadlock_no_detectado(){

	char* mensaje = string_from_format("No se detecto situacion de deadlock");

	completar_logger(mensaje, "TEAM", LOG_LEVEL_INFO);
}

//7. Llegada de un mensaje (indicando el tipo del mismo y sus datos). FALTA VER
//APPEARED POKEMON
void log_llego_mensaje_nuevo_appeared_pokemon(char* pokemon, int posX, int posY){

	char* mensaje = string_from_format("Se detecto que NO hay situacion de deadlock");

	completar_logger(mensaje, "TEAM", LOG_LEVEL_INFO);
}

//CAUGHT POKEMON
void log_llego_mensaje_nuevo_caught_pokemon(char* pokemon, int posX, int posY){

	char* mensaje = string_from_format("Se detecto que NO hay situacion de deadlock");

	completar_logger(mensaje, "TEAM", LOG_LEVEL_INFO);
}

//LOCALIZED POKEMON
void log_llego_mensaje_nuevo_localized_pokemon(char* pokemon, int posX, int posY){

	char* mensaje = string_from_format("Se detecto que NO hay situacion de deadlock");

	completar_logger(mensaje, "TEAM", LOG_LEVEL_INFO);
}


// 10. Inicio de proceso de reintento de comunicación con el Broker
void log_intento_reintento_comunicacion_broker(char* pokemon, int posX, int posY){
	int tiempoReconexion = obtener_tiempo_reconexion();
	char* mensaje = string_from_format("No se pudo conectar con el BROKER, reintentando en %d segundos...", tiempoReconexion);

	completar_logger(mensaje, "TEAM", LOG_LEVEL_INFO);
}


// 11. Resultado de proceso de reintento de comunicación con el Broker.
void log_reintento_comunicacion_Broker_exitoso(){
	char* mensaje = string_from_format("Reintento de comunicacion con BROKER exitoso");

	completar_logger(mensaje, "TEAM", LOG_LEVEL_INFO);
}

void log_reintento_comunicacion_Broker_fallido(){
	char* mensaje = string_from_format("Reintento de comunicacion con BROKER fallido");

	completar_logger(mensaje, "TEAM", LOG_LEVEL_INFO);
}

//-----------------------------------------------------------------------------------------------

void loguearMensaje(char* mensaje){
	completar_logger(mensaje,"TEAM", LOG_LEVEL_INFO);
}
