#include<stdio.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<stdbool.h>


#include "Team.h"



//ARMAMOS EL TEAM


void armar_entrenadores(void){

	lista_de_entrenadores = list_create();

	int cantidad_elementos = cantidad_entrenadores();

	for (int indice=0; indice<cantidad_elementos; indice++){

		list_add(lista_de_entrenadores, armar_entrenador(indice));

	}

}

void generar_objetivo_global(void){

	objetivo_global = dictionary_create();
	t_list* pokemones = list_create();
	int cantidad_pokemon = 0;
	char** objetivos = obtener_objetivos_entrenadores();

	for(int i = 0; i<cantidad_entrenadores(); i++){
		t_list* objetivo = obtener_objetivos(objetivos[i]);
		list_add_all(pokemones, objetivo);
	}

	//DE CADA ENTRENADOR OBTENEMOS SU LISTA DE OBJETIVOS Y LA PASAMOS A UN DICCIONARIO
	for(int indice_pokemon=0; indice_pokemon<list_size(pokemones); indice_pokemon++){
		//char* pokemonSinMalloc = list_get(pokemones,indice_pokemon);
		//int tamanio = strlen(pokemonSinMalloc);

		//char* pokemon = malloc(tamanio);
		char* pokemon = list_get(pokemones,indice_pokemon);

		if(dictionary_has_key(objetivo_global, pokemon)){

			cantidad_pokemon = dictionary_get(objetivo_global, pokemon);
			dictionary_remove_and_destroy(objetivo_global,pokemon,eliminarPokemon);
			dictionary_put(objetivo_global, pokemon, cantidad_pokemon+1);
		}
		else{

			dictionary_put(objetivo_global, pokemon, 1);
		}
	}
}

void eliminarPokemon(void* pokemon){

}

void generar_atrapados_global(void){

	atrapados_global = dictionary_create();
	t_list* pokemones = list_create();
	t_list* pokemones_atrapados;
	int cantidad_pokemon;
	int flag_atrapados = 0;
	char** atrapados = obtener_pokemon_entrenadores();

	if(!string_is_empty(atrapados)){
		for(int indice = 0; indice<cantidad_entrenadores(); indice++){
			if(flag_atrapados == 0){
			if(atrapados[indice] != NULL){
				pokemones_atrapados = obtener_atrapados(atrapados[indice]);
				list_add_all(pokemones, pokemones_atrapados);
			}
			else{
				flag_atrapados = 1;
			}
		}

		}
	}

	//DE CADA ENTRENADOR OBTENEMOS SU LISTA DE ATRAPADOS Y LA PASAMOS A UN DICCIONARIO
	for(int indice_pokemon=0; indice_pokemon<list_size(pokemones); indice_pokemon++){
		char* pokemon = list_get(pokemones,indice_pokemon);

		if(dictionary_has_key(atrapados_global, pokemon)){

			cantidad_pokemon = dictionary_get(atrapados_global, pokemon);
			dictionary_remove_and_destroy(atrapados_global,pokemon,(void*) eliminarPokemon);
			dictionary_put(atrapados_global, pokemon, cantidad_pokemon+1);
			}
		else{

			dictionary_put(atrapados_global, pokemon, 1);
			}
		}

	free(pokemones);
}

void planificacion(){
	int planificador = obtener_algoritmo_planificacion();



	switch(planificador){
		case 1:
		planificar_fifo();
		break;
		case 2:
		planificar_sjf_sd();
		break;
		case 3:
		planificar_sjf_cd();
		break;
		case 4:
		planificar_rr();
		break;
	}
}


void aparicion_pokemon(t_pokemon* pokemon){
		t_entrenador* entrenador = entrenador_mas_cercano(pokemon);
		int cantidad_pokemon = 0;

		if(dictionary_has_key(atrapados_global, pokemon->especie)){

					cantidad_pokemon = dictionary_get(atrapados_global, pokemon->especie);
		}

				dictionary_remove_and_destroy(atrapados_global, pokemon->especie, (void *) eliminarPokemon);
				dictionary_put(atrapados_global, pokemon->especie, cantidad_pokemon + 1);

		entrenador->pokemon_a_atrapar = pokemon;
		entrenador->estado = READY;

		//AGREGAMOS LOS MOVIMIENTOS DEL ENTRENADOR
		armar_movimiento(entrenador);
		//AGREGAMOS ACCION ATRAPAR A ENTRENADOR
		t_accion* accion = armar_accion(intentar_atrapar_pokemon, 1);
		list_add(entrenador->cola_de_acciones, accion);

		list_add(lista_de_entrenadores_ready,entrenador);
	}


bool es_pokemon_requerido(char* pokemon){

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
			deadlocksProducidos++;
			return true;
		}
		else{
			log_deadlock_no_detectado();
			return false;
		}

	}

	return false;
}

//pokemonesAAtrapar = [Pikachu,Squirtle,Pikachu,Gengar,Squirtle,Onix]

void informar_pokemones_a_atrapar(){
	t_list* pokemones_aux = list_create();
	t_list* pokemones_a_atrapar = list_create();
	t_entrenador* entrenador_aux;
	char* pokemon;

	for(int indice = 0; indice < cantidad_entrenadores(); indice++){
		entrenador_aux = list_get(lista_de_entrenadores, indice);
		list_add_all(pokemones_aux, entrenador_aux->objetivo);
	}

	for(int indice = 0; indice < list_size(pokemones_aux); indice++){
		pokemon = list_get(pokemones_aux, indice);

		if(dictionary_has_key(atrapados_global, pokemon)){
			if(dictionary_get(atrapados_global, pokemon) < dictionary_get(objetivo_global, pokemon)){
				if(!esta_en_lista(pokemones_a_atrapar, pokemon)){
					list_add(pokemones_a_atrapar, pokemon);
				}
			}
		}
		else
			if(!esta_en_lista(pokemones_a_atrapar, pokemon)){
				list_add(pokemones_a_atrapar, pokemon);
			}
	}

	for(int indice = 0; indice < list_size(pokemones_a_atrapar); indice++){
		pokemon = list_get(pokemones_a_atrapar, indice);
		enviar_GetPokemon_a_broker(GET_POKEMON, pokemon);
	}

	//free(pokemones_aux);
	//free(pokemones_a_atrapar);
}

bool terminoTeam(){
	return list_all_satisfy(lista_de_entrenadores, esta_en_exit);
}

void finalizoTeam(){
	t_entrenador* entrenador;
	int ciclosPorEntrenador = 0;

	loguearMensaje("Finalizo el TEAM");

//	CICLOS DE CPU TOTALES
	for(int i = 0; i<cantidad_entrenadores(); i++){
		entrenador = list_get(lista_de_entrenadores, i);
		ciclosCpuTotales += entrenador->ciclos_de_cpu_totales;
	}
	char* mensajeCiclosTotales = string_from_format("Se ejecutaron %d ciclos de cpu",ciclosCpuTotales);
	loguearMensaje(mensajeCiclosTotales);

//	CAMBIOS DE CONTEXTO
	char* mensajeCambiosDeContexto = string_from_format("Ocurrieron %d cambios de contexto durante todo el team", cambiosDeContexto);
	loguearMensaje(mensajeCambiosDeContexto);

//	CICLOS DE CPU POR ENTRENADOR
	for(int i = 0; i<cantidad_entrenadores(); i++){
		entrenador = list_get(lista_de_entrenadores, i);
		ciclosPorEntrenador = entrenador->ciclos_de_cpu_totales;
		char* mensajeCiclosEntrenador = string_from_format("El entrenador %c ejecutó %d ciclos de cpu",entrenador->ID_entrenador, ciclosPorEntrenador);
		loguearMensaje(mensajeCiclosEntrenador);
	}
//	DEADLOCKS PRODUCIDOS
	char* mensajeDeadlockProducidos = string_from_format("Se produjeron %d deadlocks", deadlocksProducidos);
	loguearMensaje(mensajeDeadlockProducidos);

//	DEADLOCKS RESUELTOS
	char* mensajeDeadlockResueltos = string_from_format("Se resolvieron %d deadlocks", deadlocksResueltos);
	loguearMensaje(mensajeDeadlockResueltos);

	liberar_team();

	free(mensajeCiclosTotales);
	free(mensajeCambiosDeContexto);
	free(mensajeDeadlockProducidos);
	free(mensajeDeadlockResueltos);
}

void liberar_entrenador(t_entrenador* entrenador){
	free(entrenador->objetivo);
	free(entrenador->atrapados);
	free(entrenador->pokemon_a_atrapar);
	free(entrenador->cola_de_acciones);
	free(entrenador);
}

void liberar_pokemon(t_pokemon* pokemon){
	free(pokemon->especie);
	free(pokemon);
}

void liberar_team(){
	list_destroy_and_destroy_elements(lista_de_entrenadores,liberar_entrenador);
	list_destroy_and_destroy_elements(lista_de_entrenadores_ready,liberar_entrenador);
	list_destroy_and_destroy_elements(lista_de_entrenadores_deadlock,liberar_entrenador);
	list_destroy_and_destroy_elements(lista_pokemonesNoRequeridos_enElMapa,liberar_pokemon);
	list_destroy(lista_ids_getPokemon);
	//dictionary_destroy_and_destroy_elements(atrapados_global,free);
	//dictionary_destroy_and_destroy_elements(objetivo_global,free);
	dictionary_destroy(atrapados_global);
	dictionary_destroy(objetivo_global);
	log_destroy(logger);
	config_destroy(config);
}












