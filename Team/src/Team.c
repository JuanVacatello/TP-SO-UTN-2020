#include<stdio.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<stdbool.h>


#include "Team.h"
#include "entrenador.h"
#include "utils.h"
#include "configTeam.h"
#include "movimiento.h"

/*int main(void) {

		int conexion;
		char* ip;
		char* puerto;

		t_log* logger;


		//logger = iniciar_logger();
		//log_destroy(logger);
		//Loggear "soy un log"

		leer_config();
		armar_entrenadores();
		ip = config_get_string_value(config,"IP");
		puerto = config_get_string_value(config,"PUERTO");
		puts("/n");
		puts(ip);
		puts(puerto);

}
*/

//ARMAMOS EL TEAM


int cantidad_entrenadores(void){
	char** entrenadores = obtener_posiciones_entrenadores();
	int cantidad = 0;
	for(int i=0; entrenadores[i] != NULL; i++){
		cantidad = i;
	}
	return cantidad;
}


void armar_entrenadores(void){

	lista_de_entrenadores = list_create();

	//total_entrenadores es para saber el total de entrenadores del config

	int cantidad_elementos = cantidad_entrenadores();

	for (int indice=0; indice<cantidad_elementos; indice++){

		list_add(lista_de_entrenadores, armar_entrenador(indice));

	}

}

void generar_objetivo_global(void){

	objetivo_global = dictionary_create();

	t_list* entrenadores = lista_de_entrenadores;
	t_entrenador* entrenador;
	t_list* lista_pokemons_entrenador;
	int cantidad_pokemon;

	//DE CADA ENTRENADOR OBTENEMOS SU LISTA DE OBJETIVOS Y LA PASAMOS A UN DICCIONARIO
	for (int indice_entrenador=0; indice_entrenador<cantidad_entrenadores(); indice_entrenador++){

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

void generar_atrapados_global(void){

	atrapados_global = dictionary_create();

	t_list* entrenadores = lista_de_entrenadores;
	t_entrenador* entrenador;
	t_list* lista_pokemons_entrenador;
	int cantidad_pokemon;

	//DE CADA ENTRENADOR OBTENEMOS SU LISTA DE ATRAPADOS Y LA PASAMOS A UN DICCIONARIO
	for (int indice_entrenador=0; indice_entrenador<cantidad_entrenadores(); indice_entrenador++){

	entrenador = list_get(entrenadores, indice_entrenador);
	lista_pokemons_entrenador = entrenador->atrapados;


			for(int indice_pokemon=0; indice_pokemon<list_size(lista_pokemons_entrenador); indice_pokemon++){

				if(dictionary_has_key(atrapados_global, list_get(lista_pokemons_entrenador,indice_pokemon))){

					cantidad_pokemon = dictionary_get(atrapados_global, list_get(lista_pokemons_entrenador, indice_pokemon));

					dictionary_put(atrapados_global, list_get(lista_pokemons_entrenador, indice_pokemon), cantidad_pokemon++);

				}

				else{

				dictionary_put(atrapados_global, list_get(lista_pokemons_entrenador, indice_pokemon), 1);

				}

			}
		}

}

/*
void planificacion(void){
	op_planificacion planificacion = obtener_algoritmo_planificacion();

	switch(planificacion){
		case FIFO:
		planificar_fifo();
		break;

	}
}*/

void planificar_fifo(void){

	while (1){

		pthread_mutex_lock(&hilo_planificador);

		t_entrenador*  entrenador;

		entrenador = list_remove(lista_de_entrenadores_ready,0);

		/* IMPORTANTE HILOS
		 *
		 *	pthread_create(&hilo_entrenador, NULL, accion, entrenador);
		 *		la accion la sacamos de la cola de acciones (QUEUE_POP())
		 *	pthread_join(hilo_entrenador,NULL);
		 *		siempre abajo de un create tiene que haber un join
		*/

		//Le decimos al entrenador que ejecute sus acciones

	}

}

t_accion* armar_accion(void(*accion)(void*), int ciclos){

	t_accion* accionNueva;
	accionNueva->accion = accion;
	accionNueva->ciclo_cpu = ciclos;

	return accionNueva;
}


void aparicion_pokemon(t_pokemon* pokemon){

	if(es_pokemon_requerido(pokemon)==1){

		t_entrenador* entrenador = entrenador_mas_cercano(pokemon);
		entrenador->pokemon_a_atrapar = pokemon;

		while(entrenador->posicion->x != pokemon->posicion->x || entrenador->posicion->y != pokemon->posicion->y){
		t_accion* accion= armar_movimiento(entrenador);
		queue_push(entrenador->cola_de_acciones, accion);
		//AGREGAMOS ACCION A ENTRENADOR
		}

		list_add(lista_de_entrenadores_ready,entrenador);
		list_add(pokemones_requeridos,pokemon);
		pthread_mutex_unlock(&hilo_planificador);

	}
	else{
		//ESPERAMOS A LOG//
	}
}

//0 PARA NO | 1 PARA SI//
int es_pokemon_requerido(t_pokemon* pokemon){
	if(dictionary_has_key(objetivo_global,pokemon->especie)){
		if(dictionary_get(objetivo_global,pokemon->especie)==0){
			return 0;
		}
		else{
		}
		return 1;
}

	else{
	return 0;
	}
}



void terminar_programa(int conexion, t_log* logger){
	if(logger =! NULL)
		log_destroy(logger);

	if(config =! NULL)
		config_destroy(config);
}

