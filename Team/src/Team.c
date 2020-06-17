#include<stdio.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<stdbool.h>


#include "Team.h"
#include "entrenador.h"
#include "utils.h"
#include "configTeam.h"
#include "movimiento.h"
#include "planificacion.h"


/*int main(void) {



}
*/

//ARMAMOS EL TEAM


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

	if(!list_is_empty(lista_pokemons_entrenador)){
		for(int indice_pokemon=0; indice_pokemon<list_size(lista_pokemons_entrenador); indice_pokemon++){

			if(list_get(lista_pokemons_entrenador,indice_pokemon) != NULL){
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
	}

}


void planificacion(){
	puts("aca entra2");
	//Esta funcion se la mandamos al hilo principal para que la ejecute

	int planificador = obtener_algoritmo_planificacion();

	puts("aca entra3");
	//HAY QUE CHEQUEAR ESTO

	iniciar_vg();

	puts("inicia las variables globales");

	switch(planificador){
		case 1:
		planificar_fifo();
		break;
		case 2:
		planificar_sjf_sd();
		break;
		//case 3:
		//planificar_fifo();
		//break;
		//case 4:
		//planificar_fifo();
		//break;

	}

}


void aparicion_pokemon(t_pokemon* pokemon){
	puts("aca entra 6");
	if(es_pokemon_requerido(pokemon)){
		puts("aca entra 7");
		t_entrenador* entrenador = entrenador_mas_cercano(pokemon);
		puts("aca entra 8");
		entrenador->pokemon_a_atrapar = pokemon;

		//AGREGAMOS LOS MOVIMIENTOS DEL ENTRENADOR
		armar_movimiento(entrenador);
		//AGREGAMOS ACCION ATRAPAR A ENTRENADOR
		t_accion* accion = armar_accion(atrapar_pokemon, 1);
		list_add(entrenador->cola_de_acciones, accion);

		puts("aca entra 9");
		list_add(lista_de_entrenadores_ready,entrenador);
		//list_add(pokemones_requeridos,pokemon);// no entiendo esto tampoco

		//pthread_mutex_unlock(hilo_planificador); //este es el del entrenador??
		//ejecutamos las acciones

	}
	else{
		//ESPERAMOS A LOG//
	}
}

bool es_pokemon_requerido(t_pokemon* pokemon){
	if(dictionary_has_key(objetivo_global,pokemon->especie)){
		if(dictionary_get(objetivo_global,pokemon->especie)==0){
			return false;
		}
		else{
		}
		return true;
}

	else{
	return false;
	}
}

t_accion* armar_accion(void(*accion)(void*), int ciclos){

	t_accion* accionNueva = malloc(sizeof(t_accion));
	accionNueva->accion = accion;
	accionNueva->ciclo_cpu = ciclos;

	return accionNueva;
}

bool deteccion_de_deadlock(){

	if(!list_is_empty(lista_de_entrenadores_deadlock)){
		return true;
	}
	else{
		return false;
	}
}




void terminar_programa(int conexion, t_log* logger){
	if(logger =! NULL)
		log_destroy(logger);

	if(config =! NULL)
		config_destroy(config);
}


//----------------

//0 para vacia, 1 para hay un pokemons
bool hay_pokemones_sueltos(t_list* listaTest){
	if(list_is_empty(listaTest))
		return false;
	else
		return true;
}
















