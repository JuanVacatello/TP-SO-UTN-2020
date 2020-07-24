#include "logBroker.h"

void iniciar_logger(){
	char* file = "Broker.log"; //obtener_log_file();
	if((logger = log_create(file,"BROKER",1,LOG_LEVEL_INFO )) == NULL){
		printf("No se pudo crear el log");
		exit(1);
	}
}

void completar_logger(char* mensaje, char* programa, t_log_level log_level)
{
	sem_wait(&MUTEX_LOGGER);
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
	sem_post(&MUTEX_LOGGER);
}

/*
 * 1.   Conexión de un proceso al broker.
 * 2.   Suscripción de un proceso a una cola de mensajes.
 * 3.   Llegada de un nuevo mensaje a una cola de mensajes.
 * 4.   Envío de un mensaje a un suscriptor específico.
 * 5.   Confirmación de recepción de un suscriptor al envío de un mensaje previo.
 * 6.   Almacenado de un mensaje dentro de la memoria (indicando posición de inicio de su partición).
 * 7.   Eliminado de una partición de memoria (indicando la posición de inicio de la misma).
 * 8.1. Ejecución de compactación para particiones dinámicas.
 * 8.2. Asociación de bloques para Buddy System (indicando qué particiones se asociaron, con posicion inicial de ambas).
 * 9.   Ejecución de Dump de cache (solo informar que se solicitó el mismo).
*/

// 1. Conexión de un proceso al broker.
void log_conexion(int socket_proceso){
	char* log = string_from_format("Se conectó proceso de socket %d al Broker.", socket_proceso);
	completar_logger(log, "BROKER", LOG_LEVEL_INFO);
}

// 2. Suscripción de un proceso a una cola de mensajes.
void log_suscripcion_nueva(int socket_suscriptor, int id_suscriptor, int cola_mensaje){
	char* cola_en_string = cola_referida(cola_mensaje);
	char* log = string_from_format("Se suscribió el proceso con socket %d y con id %d a la cola de mensajes %s.", socket_suscriptor, id_suscriptor, cola_en_string);
	completar_logger(log, "BROKER", LOG_LEVEL_INFO);
}

// 3. Llegada de un nuevo mensaje a una cola de mensajes.
void log_mensaje_nuevo(int cola){
	char* cola_en_string = cola_referida(cola);
	char* log = string_from_format("Llegó un nuevo mensaje a la cola de mensajes %s", cola_en_string);
	completar_logger(log, "Broker", LOG_LEVEL_INFO);
}

// 4. Envío de un mensaje a un suscriptor específico.
void log_envio_mensaje(int socket_suscriptor, int cola){
	char* cola_en_string = cola_referida(cola);
	char* log = string_from_format("Se le envió un mensaje al suscriptor de socket %d, a través de la cola %s.", socket_suscriptor, cola_en_string);
	completar_logger(log, "Broker", LOG_LEVEL_INFO);
}

// 5. Confirmación de recepción de un suscriptor al envío de un mensaje previo.
void log_confirmacion(int socket_suscriptor, int mensaje_id){
	char* log = string_from_format("El suscriptor de socket %d confirmó la recepción del mensaje con id %d.", socket_suscriptor, mensaje_id);
	completar_logger(log,"BROKER", LOG_LEVEL_INFO);
}

// 6. Almacenado de un mensaje dentro de la memoria (indicando posición de inicio de su partición).
void log_almacenar_mensaje(int posicion_mensaje){
	char* log = string_from_format("Se almacenó un mensaje en la posición %d de la memoria principal.", posicion_mensaje);
	completar_logger(log, "BROKER", LOG_LEVEL_INFO);
}

// 7. Eliminado de una partición de memoria (indicando la posición de inicio de la misma).
void log_particion_eliminada(int posicion_liberada){
	char* log = string_from_format("Se eliminó de memoria principal la partición que se encontraba en la posicion %d.", posicion_liberada);
	completar_logger(log, "BROKER", LOG_LEVEL_INFO);
}

// 8.1. Ejecución de compactación para particiones dinámicas.
void log_compactacion(){
	int frecuencia_de_compactacion = obtener_frecuencia_compactacion();
	char* log = string_from_format("Se compactó la memoria ya que la frecuencia de compactación es %d.", frecuencia_de_compactacion);
	completar_logger(log, "BROKER", LOG_LEVEL_INFO);
}

// 8.2. Asociación de bloques para Buddy System (indicando qué particiones se asociaron, con posicion inicial de ambas).
void log_asociacion_de_bloques(int posicion1, int posicion2){

	int pos_mas_chica = posicion1;
	int pos_mas_grande = posicion2;

	if(posicion1 > posicion2){
		pos_mas_chica = posicion2;
		pos_mas_grande = posicion1;
	}

	char* log = string_from_format("Se asociaron las particiones que se encontraban en las posiciones %d y %d.", pos_mas_chica, pos_mas_grande);
	completar_logger(log, "BROKER", LOG_LEVEL_INFO);
}

// 9. Ejecución de Dump de cache (solo informar que se solicitó el mismo).
void log_ejecucion_dump(){
	completar_logger("Se ejecutó el Dump de la cache", "BROKER", LOG_LEVEL_INFO);
}

char* cola_referida(int numero){
	char* cola = string_new();
	switch(numero){
		case 1:
			cola = "NEW_POKEMON";
			break;
		case 2:
			cola = "APPEARED_POKEMON";
			break;
		case 3:
			cola = "CATCH_POKEMON";
			break;
		case 4:
			cola = "CAUGHT_POKEMON";
			break;
		case 5:
			cola = "GET_POKEMON";
			break;
		case 6:
			cola = "LOCALIZED_POKEMON";
			break;
		}
	return cola;
}

