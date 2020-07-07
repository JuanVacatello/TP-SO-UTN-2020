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
// "Pikachu|Squirtle,Pikachu|Gengar,Squirtle|Onix"
void generar_objetivo_global(void){

	objetivo_global = dictionary_create();
	t_list* pokemones = list_create();
	int cantidad_pokemon;
	char** objetivos = obtener_objetivos_entrenadores();

	for(int i = 0; i<cantidad_entrenadores(); i++){
		t_list* objetivo = obtener_objetivos(objetivos[i]);
		list_add_all(pokemones, objetivo);
	}

	//DE CADA ENTRENADOR OBTENEMOS SU LISTA DE OBJETIVOS Y LA PASAMOS A UN DICCIONARIO
	for(int indice_pokemon=0; indice_pokemon<list_size(pokemones); indice_pokemon++){

		if(dictionary_has_key(objetivo_global, list_get(pokemones,indice_pokemon))){

			cantidad_pokemon = dictionary_get(objetivo_global, list_get(pokemones, indice_pokemon));
			dictionary_remove_and_destroy(objetivo_global,list_get(pokemones, indice_pokemon),eliminar_pokemon);
			dictionary_put(objetivo_global, list_get(pokemones, indice_pokemon), cantidad_pokemon+1);
		}
		else{

			dictionary_put(objetivo_global, list_get(pokemones, indice_pokemon), 1);
		}
	}

}
//"Pikachu"
//"Pikachu,Pikachu|Gengar,Squirtle|Onix"
void generar_atrapados_global(void){

	atrapados_global = dictionary_create();
	t_list* pokemones = list_create();
	t_list* pokemones_atrapados;
	int cantidad_pokemon;
	char** atrapados = obtener_pokemon_entrenadores();

	for(int indice = 0; indice<cantidad_entrenadores(); indice++){
		if(atrapados[indice] != NULL){
			pokemones_atrapados = obtener_atrapados(atrapados[indice]);
			list_add_all(pokemones, pokemones_atrapados);
		}
	}

	//DE CADA ENTRENADOR OBTENEMOS SU LISTA DE ATRAPADOS Y LA PASAMOS A UN DICCIONARIO
	for(int indice_pokemon=0; indice_pokemon<list_size(pokemones); indice_pokemon++){

		if(dictionary_has_key(atrapados_global, list_get(pokemones,indice_pokemon))){

			cantidad_pokemon = dictionary_get(atrapados_global, list_get(pokemones, indice_pokemon));
			dictionary_remove_and_destroy(atrapados_global,list_get(pokemones, indice_pokemon),eliminar_pokemon);
			dictionary_put(atrapados_global, list_get(pokemones, indice_pokemon), cantidad_pokemon+1);
			}
		else{

			dictionary_put(atrapados_global, list_get(pokemones, indice_pokemon), 1);
			}
		}

}

void eliminar_pokemon(void* pokemon){}

void planificacion(){
	puts("aca entra2");
	//Esta funcion se la mandamos al hilo principal para que la ejecute
	int planificador = obtener_algoritmo_planificacion();

	puts("aca entra3");

	//puts("inicia las variables globales");

	switch(planificador){
		case 1:
		planificar_fifo();
		break;
		case 2:
		planificar_sjf_sd();
		break;
		//case 3:
		//planificar_sjf_cd();
		//break;
		case 4:
		planificar_rr();
		break;

	}

}


void aparicion_pokemon(t_pokemon* pokemon){
		t_entrenador* entrenador = entrenador_mas_cercano(pokemon);

		entrenador->pokemon_a_atrapar = pokemon;
		entrenador->estado = READY;

		//AGREGAMOS LOS MOVIMIENTOS DEL ENTRENADOR
		armar_movimiento(entrenador);
		//AGREGAMOS ACCION ATRAPAR A ENTRENADOR
		t_accion* accion = armar_accion(atrapar_pokemon, 3);
		list_add(entrenador->cola_de_acciones, accion);

		list_add(lista_de_entrenadores_ready,entrenador);
}


bool es_pokemon_requerido(char* pokemon){
	sem_wait(&MUTEX_POKEMON_REQUERIDO);
	if(dictionary_has_key(objetivo_global,pokemon)){
		int cantidad_objetivos = dictionary_get(objetivo_global,pokemon);
		int cantidad_atrapados = dictionary_get(atrapados_global,pokemon);
		if(cantidad_objetivos == cantidad_atrapados){
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

	t_entrenador* entrenador;
	t_list* total_atrapados = list_create();
	t_list* atrapados_aux;
	t_list* total_objetivos = list_create();
	t_list* objetivos_aux;

	for(int indice = 0; indice < cantidad_entrenadores(); indice++){
		entrenador = list_get(lista_de_entrenadores,indice);
		atrapados_aux = entrenador->atrapados;
		list_add_all(total_atrapados, atrapados_aux);
		objetivos_aux = entrenador->objetivo;
		list_add_all(total_objetivos, objetivos_aux);
	}

	if(list_size(total_atrapados) == list_size(total_objetivos)){

		log_inicio_deteccion_deadlock();

		if(!list_is_empty(lista_de_entrenadores_deadlock)){
			log_deadlock_detectado();
			return true;
		}
		else{
			log_deadlock_no_detectado();
			return false;
		}

	}

	//free(atrapados_aux);
	//free(objetivos_aux);

	return false;
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

//pokemonesAAtrapar = [Pikachu,Squirtle,Pikachu,Gengar,Squirtle,Onix]

void informar_pokemones_a_atrapar(){
	t_list* pokemones_aux = list_create();
	t_list* pokemones_a_atrapar = list_create();
	t_entrenador* entrenador_aux;
	char* pokemon;

	for(int indice; indice < cantidad_entrenadores(); indice++){
		entrenador_aux = list_get(lista_de_entrenadores, indice);
		list_add_all(pokemones_aux, entrenador_aux->objetivo);
	}

	for(int indice; indice < list_size(pokemones_aux); indice++){
		pokemon = list_get(pokemones_aux, indice);

		if(dictionary_get(atrapados_global, pokemon)>0){
			if(!esta_en_lista(pokemones_a_atrapar, pokemon)){
				list_add(pokemones_a_atrapar, pokemon);
			}
		}
	}

	for(int indice; indice < list_size(pokemones_a_atrapar); indice++){
		pokemon = list_get(pokemones_a_atrapar, indice);
		enviar_GetPokemon_a_broker(5 /*GET_POKEMON*/, pokemon);
	}

	free(pokemones_aux);
	free(pokemones_a_atrapar);
	free(entrenador_aux);
}



















