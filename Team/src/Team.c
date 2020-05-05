#include "Team.h"


#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<stdbool.h>

#include "Team.h"
#include "entrenador.h"
#include "movimiento.c"

int main(void) {

		int conexion;
		char* ip;
		char* puerto;

		t_log* logger;
		t_config* config;

		//logger = iniciar_logger();
		//log_destroy(logger);
		//Loggear "soy un log"

		config = leer_config();
		ip = config_get_string_value(config,"IP");
		puerto = config_get_string_value(config,"PUERTO");
		puts("/n");
		puts(ip);
		puts(puerto);

}



//ARMAMOS EL TEAM



t_list* armar_entrenadores(t_config* config){

	t_list* entrenadores = list_create();

	//total_entrenadores es para saber el total de entrenadores del config

	int cantidad_elementos = cantidad_entrenadores(config);

	for (int indice=0; indice<cantidad_elementos; indice++){

		list_add(entrenadores, armar_entrenador(config, indice));

	}

	return entrenadores;

}

int cantidad_entrenadores(t_config* config){
	t_list* entrenadores = obtener_posiciones_entrenadores(config);
	int cantidad = list_size(entrenadores);
	return cantidad;
}

