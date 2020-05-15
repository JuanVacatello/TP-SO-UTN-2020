#include "configTeam.h"


//t_config* config = leer_config();

void leer_config(void)
{
	if(config_create("/home/utnso/Documentos/tp-2020-1c-wRAPPERS/Team/Team.config") == NULL){
			printf("Error en la conexion");
			exit(2);

	}
	config = config_create("/home/utnso/Documentos/tp-2020-1c-wRAPPERS/Team/Team.config");
}

char* obtener_ip(void){
	char* ip;
	ip = config_get_string_value(config, "IP_BROKER");
	return ip;
}
char* obtener_puerto(void){
	char* puerto;
	puerto = config_get_string_value(config, "PUERTO_BROKER");
	return puerto;
}
int obtener_retardo_ciclo_cpu(void){
	int retardo;
	retardo = config_get_int_value(config, "RETARDO_CICLO_CPU");
	return retardo;
}
int obtener_tiempo_reconexion(void){
	int tiempo_reconexion;
	tiempo_reconexion = config_get_int_value(config, "TIEMPO_RECONEXION");
	return tiempo_reconexion;
}
char* obtener_algoritmo_planificacion(void){
	char* algoritmo_planificacion;
	algoritmo_planificacion = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
	return algoritmo_planificacion;
}
int obtener_estimacion_inicial(void){
	int estimacion_inicial;
	estimacion_inicial = config_get_int_value(config, "ESTIMACION_INICIAL");
	return estimacion_inicial;
}
int obtener_quantum(void){
	int quantum;
	quantum = config_get_int_value(config, "QUANTUM");
	return quantum;
}
char* obtener_log_file(void){
	char* ruta_log;
	ruta_log = config_get_string_value(config, "LOG_FILE");
	return ruta_log;
}

//Acá obtengo un vector, hay que ver como vamos obteniendo los elementos
t_list* obtener_posiciones_entrenadores(void){
	t_list* posiciones_entrenadores = list_create();
	posiciones_entrenadores = config_get_array_value(config, "POSICIONES_ENTRENADORES");
	return posiciones_entrenadores;
}

t_list* obtener_pokemon_entrenadores(void){
	t_list* pokemon_entrenadores = list_create();
	pokemon_entrenadores = config_get_array_value(config, "POKEMON_ENTRENADORES");
	return pokemon_entrenadores;
}

t_list* obtener_objetivos_entrenadores(void){
	t_list* objetivos_entrenadores = list_create();
	objetivos_entrenadores = config_get_array_value(config, "OBJETIVOS_ENTRENADORES");
	return objetivos_entrenadores;
}

int obtener_alpha(void){
	int alpha;
	alpha = config_get_int_value(config, "ALPHA");
	return alpha;
}


