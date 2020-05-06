#include "Team.h"


#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<commons/collections/dictionary.h>
#include<readline/readline.h>
#include<stdbool.h>


#include "Team.h"
#include "entrenador.h"
#include "movimiento.c"
#include "utils/utils.h"

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
		armar_entrenadores(config);
		ip = config_get_string_value(config,"IP");
		puerto = config_get_string_value(config,"PUERTO");
		puts("/n");
		puts(ip);
		puts(puerto);

}


//ARMAMOS EL TEAM


int cantidad_entrenadores(t_config* config){
	t_list* entrenadores = obtener_posiciones_entrenadores(config);
	int cantidad = list_size(entrenadores);
	return cantidad;
}


void armar_entrenadores(t_config* config){

	lista_de_entrenadores = list_create();

	//total_entrenadores es para saber el total de entrenadores del config

	int cantidad_elementos = cantidad_entrenadores(config);

	for (int indice=0; indice<cantidad_elementos; indice++){

		list_add(lista_de_entrenadores, armar_entrenador(config, indice));

	}

}

void generar_objetivo_global(t_config* config){

	objetivo_global = dictionary_create();

	t_list* entrenadores = lista_de_entrenadores;
	t_entrenador* entrenador;
	t_list* lista_pokemons_entrenador;
	int cantidad_pokemon;

	//DE CADA ENTRENADOR OBTENEMOS SU LISTA DE OBJETIVOS Y LA PASAMOS A UN DICCIONARIO
	for (int indice_entrenador=0; indice_entrenador<cantidad_entrenadores(config); indice_entrenador++){

	entrenador = list_get(entrenadores, indice_entrenador);
	lista_pokemons_entrenador = entrenador->objetivo;


			for(int indice_pokemon=0; indice_pokemon<list_size(lista_pokemons_entrenador); indice_pokemon++){

				if(dictionary_has_key(objetivo_global, list_get(lista_pokemons_entrenador,indice_pokemon))){

					cantidad_pokemon = dictionary_get(objetivo_global, list_get(lista_pokemons_entrenador, indice_pokemon));

					dictionary_put(objetivo_global, list_get(lista_pokemons_entrenador, indice_pokemon), cantidad_pokemon++);

				}

				else{

				dictionary_put(objetivo_global, list_get(lista_pokemons_entrenador, indice_pokemon), 1);

				}

			}
		}

}




