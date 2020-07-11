#include "Team.h"
#include "entrenador.h"
#include "movimiento.h"
#include "utils.h"
#include "conexiones.h"

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<readline/readline.h>
#include<stdbool.h>
#include<string.h>



//ARMAMOS TODOS LOS ENTRENADORES




//ARMAMOS EL ENTRENADOR


t_entrenador* armar_entrenador(int indice){

	t_entrenador* entrenador = malloc(sizeof(t_entrenador));

	//NOMBRE
	entrenador->ID_entrenador = indice + 65;

	//POSICION
	char** posiciones = obtener_posiciones_entrenadores();
	t_posicion posicion_entrenador = obtener_posicion(posiciones[indice]);
	entrenador->posicion = posicion_entrenador;

	//LISTA OBJETIVOS
	char** objetivos = obtener_objetivos_entrenadores();
	t_list* objetivo = obtener_objetivos(objetivos[indice]);
	entrenador->objetivo = objetivo;

	//LISTA ATRAPADOS
//--------------[Pikachu|Squirtle,Onix]
	char** atrapados =obtener_pokemon_entrenadores();
	t_list* atrapado = list_create();

	if(!string_is_empty(atrapados)){
		if(atrapados[indice]!=NULL){
			atrapado = obtener_atrapados(atrapados[indice]);
		}
	}
	entrenador->atrapados = atrapado;

	//COLA DE ACCIONES
	t_list* cola_de_acciones_entrenador = list_create();
	entrenador->cola_de_acciones  = cola_de_acciones_entrenador;

	//ESTADO
	entrenador->estado = NEW;

	//CICLOS DE CPU
	entrenador->ciclos_de_cpu_totales = 0;

	//RAFAGA ANTERIOR
	entrenador->rafaga_anterior = 0;

	//ID_CATCH_POKEMON
	entrenador->ID_catch_pokemon = 0;

	//POKEMON_A_ATRAPAR
	entrenador->pokemon_a_atrapar = NULL;

	//POSICION_INTERCAMBIO
	entrenador->posicionIntercambio.x = NULL;
	entrenador->posicionIntercambio.y = NULL;

	return entrenador;
}

t_posicion obtener_posicion(char* posicion){

	char** vector_posicion = string_split(posicion, "|");
	int posicion_x = atoi(vector_posicion[0]);
	int posicion_y = atoi(vector_posicion[1]);

	t_posicion posicion_casteada;
	posicion_casteada.x = posicion_x;
	posicion_casteada.y = posicion_y;

	return posicion_casteada;
}

t_list* obtener_objetivos(char* objetivos){

	int cantidad_elementos = cantidad_de_elementos(objetivos);

	char** vector_objetivos = string_n_split(objetivos, cantidad_elementos,"|");

	t_list* objetivos_casteados = list_create();

	for(int i=0; i<cantidad_elementos; i++){

		list_add(objetivos_casteados, vector_objetivos[i]);

	}

	return objetivos_casteados;

}

t_list* obtener_atrapados(char* atrapados){

	int cantidad_elementos = cantidad_de_elementos(atrapados);

	t_list* atrapados_casteados = list_create();

	if(cantidad_elementos != 0){
		char** vector_atrapados = string_n_split(atrapados, cantidad_elementos,"|");

		for(int i=0; i<cantidad_elementos; i++){

			list_add(atrapados_casteados, vector_atrapados[i]);

		}

	}

	return atrapados_casteados;
}


//DADO UN POKEMON EVALUAR QUE ENTRENADOR ESTÁ MAS CERCA//

t_entrenador* entrenador_mas_cercano(t_pokemon* pokemon){

	//Filtrar sólo a los entrenadores que se puedan planificar

	t_list* entrenadores = list_filter(lista_de_entrenadores, puede_atrapar);
	t_posicion posicion_pokemon = pokemon->posicion;
	t_entrenador* entrenador;
	t_entrenador* entrenador_cercano;
	t_posicion posicion_entrenador;

	int menor_distancia=-1;
	int distancia_actual = 0;

	for (int indice_entrenador=0; indice_entrenador<list_size(entrenadores); indice_entrenador++){

		entrenador = list_get(entrenadores, indice_entrenador);


			posicion_entrenador = entrenador->posicion;
			distancia_actual=sacar_distancia(posicion_pokemon,posicion_entrenador);

			if(menor_distancia>distancia_actual || menor_distancia==-1){
				menor_distancia=distancia_actual;
				entrenador_cercano = entrenador;

			}

	}

	return entrenador_cercano;
}

//OBTIENE CANTIDAD DE MOVIMIENTOS A REALIZAR PARA MOVILIZARSE//

int sacar_distancia(t_posicion pokeposicion,t_posicion entreposicion){

	int x,y,distancia;

	x = pokeposicion.x - entreposicion.x;
	y = pokeposicion.y - entreposicion.y;

	distancia = abs(x) + abs(y);

	return distancia;
}


bool puede_atrapar(t_entrenador* entrenador){
	if (entrenador->estado == BLOCKED || entrenador->estado == NEW){
		if(list_size(entrenador->atrapados) < list_size(entrenador->objetivo)){
			return true;
		}
		else{
			return false;
		}
	}
	else{
		return false;
	}

}

void ejecutar_entrenador(t_entrenador* entrenador){

	t_accion* accion_a_ejecutar;
	pthread_t hilo_entrenador = entrenador->hilo_entrenador;

	accion_a_ejecutar = list_remove(entrenador->cola_de_acciones,0);
	entrenador->rafaga_anterior += accion_a_ejecutar->ciclo_cpu;
	entrenador->ciclos_de_cpu_totales += accion_a_ejecutar->ciclo_cpu;

		pthread_create(&hilo_entrenador, NULL , (accion_a_ejecutar->accion) , entrenador);
		pthread_join(hilo_entrenador,NULL);

	//free(accion_a_ejecutar->accion);
	//free(accion_a_ejecutar->ciclo_cpu);
	//free(accion_a_ejecutar);

}


void atrapar_pokemon(t_entrenador* entrenador){

	int cantidad_pokemon = 0;
	enviar_CatchPokemon_a_broker(3, entrenador);
	efectuar_ciclo_cpu(entrenador, 1);
	pthread_mutex_lock(&mutex_entrenador);

	if(entrenador->pudo_atrapar_pokemon == 0){
		log_operacion_de_atrapar_fallida(entrenador);	//NO ATRAPÓ AL POKEMON
	}
	else if(entrenador->pudo_atrapar_pokemon == 1){
		list_add(entrenador->atrapados, entrenador->pokemon_a_atrapar->especie);
		log_operacion_de_atrapar_exitosa(entrenador);	//ATRAPÓ AL POKEMON

		if(dictionary_has_key(atrapados_global, entrenador->pokemon_a_atrapar->especie)){

			cantidad_pokemon = dictionary_get(atrapados_global, entrenador->pokemon_a_atrapar->especie);
		}
		dictionary_remove_and_destroy(atrapados_global, entrenador->pokemon_a_atrapar->especie, eliminar_pokemon);
		dictionary_put(atrapados_global, entrenador->pokemon_a_atrapar->especie, cantidad_pokemon + 1);

		//Actualizamos diccionarios

		int pid = process_getpid();
		char* mensaje = string_from_format("pid: %d.",pid);
		completar_logger(mensaje, "TEAM", LOG_LEVEL_INFO);

		verificar_estado_entrenador(entrenador);

	}


	entrenador->pokemon_a_atrapar = NULL;
	sem_post(&MUTEX_POKEMON_REQUERIDO);
}


void verificar_estado_entrenador(t_entrenador* entrenador){

	int tamanio_lista_objetivo = list_size(entrenador->objetivo);
	int tamanio_lista_atrapados = list_size(entrenador->atrapados);
	int contador_atrapados = 0;
	char* pokemon;

	if(tamanio_lista_objetivo == tamanio_lista_atrapados){

	for(int indice_pokemon=0; indice_pokemon<list_size(entrenador->objetivo); indice_pokemon++){
		pokemon = list_get(entrenador->objetivo, indice_pokemon);


		if (termino_con_pokemon(entrenador, pokemon)){
			contador_atrapados++;
		}

	}
		if(contador_atrapados == tamanio_lista_objetivo){
			if(esta_entrenador_en_lista(entrenador, lista_de_entrenadores_deadlock)){
				remover_entrenador(entrenador);
			}
			entrenador->estado = EXIT;
			cambiosDeContexto++;
		}
		else{
			//PONEMOS ENTRENADOR EN DEADLOCK
			entrenador->estado = BLOCKED;
			cambiosDeContexto++;
			if(!esta_entrenador_en_lista(entrenador, lista_de_entrenadores_deadlock)){
				list_add(lista_de_entrenadores_deadlock, entrenador);
			}
		}
	}
	else{
		entrenador->estado = BLOCKED;
		cambiosDeContexto++;
	}

}
//-----------------------

void remover_entrenador(t_entrenador* entrenador){
	t_entrenador* entrenador_aux;
	for(int i = 0; i<list_size(lista_de_entrenadores_deadlock); i++){
		entrenador_aux = list_get(lista_de_entrenadores_deadlock, i);
		if(entrenador->ID_entrenador == entrenador_aux->ID_entrenador){
			entrenador_aux = list_remove(lista_de_entrenadores_deadlock, i);
			break;
		}
	}
}

bool esta_entrenador_en_lista(t_entrenador* entrenador ,t_list* lista){
	t_entrenador* entrenador_aux;
	for(int i = 0; i<list_size(lista); i++){
		entrenador_aux = list_get(lista, i);
		if(entrenador->ID_entrenador == entrenador_aux->ID_entrenador){
			return true;
		}
	}
	return false;
}
//------------------

bool termino_con_pokemon(t_entrenador* entrenador, char* pokemon){

	char* pokemon_auxiliar;
	int cantidad_objetivo = 0;
	int cantidad_atrapados = 0;

	//Recorremos la lista de objetivos del entrenador para saber cuantos pokemon de
	//determinada especie necesita en total

	for (int indice_pokemon=0; indice_pokemon<list_size(entrenador->objetivo); indice_pokemon++){
		pokemon_auxiliar = list_get(entrenador->objetivo, indice_pokemon);

		if(strcmp(pokemon_auxiliar, pokemon)==0){
			cantidad_objetivo++;
		}
	}
	//Recorremos la lista de atrapados para saber si ya tiene a todos los que necesita

	for (int indice_pokemon=0; indice_pokemon<list_size(entrenador->objetivo); indice_pokemon++){
			pokemon_auxiliar = list_get(entrenador->atrapados, indice_pokemon);

			if(strcmp(pokemon_auxiliar, pokemon)==0){
				cantidad_atrapados++;
			}
		}
	if(cantidad_atrapados == cantidad_objetivo){
		return true;
	}
	else{
		return false;
	}
}

t_entrenador* preparar_intercambio(){
	t_entrenador* entrenador1 = list_get(lista_de_entrenadores_deadlock,0);
	t_entrenador* entrenador2 = obtener_entrenador_con_pokemon_del_entrenador1(entrenador1);

	entrenador1->posicionIntercambio.x = entrenador2->posicion.x;
	entrenador1->posicionIntercambio.y = entrenador2->posicion.y;

	entrenador1->estado = READY;
	cambiosDeContexto++;

	armar_movimiento(entrenador1);

	t_accion* accionIntercambio = malloc(sizeof(t_accion));
	accionIntercambio->ciclo_cpu = 5;
	accionIntercambio->accion = intercambiar_pokemones;

	list_add(entrenador1->cola_de_acciones, accionIntercambio);

	list_add(lista_de_entrenadores_ready, entrenador1);

	return entrenador1;
}


void intercambiar_pokemones(t_entrenador* entrenador1){

	t_entrenador* entrenador2 = obtener_entrenador_con_pokemon_del_entrenador1(entrenador1);


	entrenador1->posicionIntercambio.x = entrenador2->posicion.x;
	entrenador1->posicionIntercambio.y = entrenador2->posicion.y;

	char* pokemon_sobra_entrenador1;
	char* pokemon_sobra_entrenador2;
	char* pokemon_aux_1;
	char* pokemon_aux_2;

	t_list* intercambiables_entrenador1 = list_create();
	t_list* intercambiables_entrenador2 = list_create();

//-----Ubicamos los pokemones intercambiables de cada entrenador

	for(int indice_pokemon = 0; indice_pokemon < list_size(entrenador1->atrapados); indice_pokemon++){
		pokemon_sobra_entrenador1 = list_get(entrenador1->atrapados, indice_pokemon);

		if(es_intercambiable_pokemon(entrenador1, pokemon_sobra_entrenador1)){
			list_add(intercambiables_entrenador1, pokemon_sobra_entrenador1);
		}
	}

	for(int indice_pokemon = 0; indice_pokemon < list_size(entrenador2->atrapados); indice_pokemon++){
		pokemon_sobra_entrenador2 = list_get(entrenador2->atrapados, indice_pokemon);

		if(es_intercambiable_pokemon(entrenador2, pokemon_sobra_entrenador2)){
			list_add(intercambiables_entrenador2, pokemon_sobra_entrenador2);
		}
	}

//-----Bucamos el pokemon que necesita a cada uno y sino el que le sobra a cada uno

	for(int indice_pokemon = 0; indice_pokemon < list_size(intercambiables_entrenador1); indice_pokemon++){
		pokemon_sobra_entrenador1 = list_get(intercambiables_entrenador1, indice_pokemon);
		if(necesita_pokemon(entrenador2, pokemon_sobra_entrenador1)){
			break;
		}
	}

	for(int indice_pokemon = 0; indice_pokemon < list_size(intercambiables_entrenador2); indice_pokemon++){
		pokemon_sobra_entrenador2 = list_get(intercambiables_entrenador2, indice_pokemon);
		if(necesita_pokemon(entrenador1, pokemon_sobra_entrenador2)){
			break;
		}
	}

// Ya tenemos los dos pokemones a intercambiar, falta intercambiarlos

	for(int indice_pokemon = 0; indice_pokemon < list_size(entrenador1->objetivo); indice_pokemon++){
		pokemon_aux_1= list_get(entrenador1->atrapados, indice_pokemon);
		if(strcmp(pokemon_sobra_entrenador1, pokemon_aux_1)==0){
			pokemon_aux_1 = list_remove(entrenador1->atrapados, indice_pokemon);
			list_add(entrenador2->atrapados, pokemon_aux_1);
			break;
		}
	}

	for(int indice_pokemon = 0; indice_pokemon < list_size(entrenador2->objetivo); indice_pokemon++){
		pokemon_aux_2= list_get(entrenador2->atrapados, indice_pokemon);
		if(strcmp(pokemon_sobra_entrenador2, pokemon_aux_2)==0){
			pokemon_aux_2 = list_remove(entrenador2->atrapados, indice_pokemon);
			list_add(entrenador1->atrapados, pokemon_aux_2);
			break;
		}
	}

// POKEMONES INTERCAMBIADOS
	ciclos_de_cpu(5);
	log_operacion_de_intercambio(entrenador1,entrenador2, pokemon_aux_1, pokemon_aux_2);
	deadlocksResueltos++;

	verificar_estado_entrenador(entrenador1);
	verificar_estado_entrenador(entrenador2);
}


bool es_intercambiable_pokemon(t_entrenador* entrenador, char* pokemon){
	int contador_pokemon_atrapado = 0;
	int contador_pokemon_objetivo = 0;
	char* pokemon_entrenador;

	for(int indice_pokemon = 0; indice_pokemon < list_size(entrenador->objetivo); indice_pokemon++){
		pokemon_entrenador = list_get(entrenador->objetivo,indice_pokemon);
		if(strcmp(pokemon_entrenador, pokemon)==0)
			contador_pokemon_objetivo++;
	}

	for(int indice_pokemon = 0; indice_pokemon < list_size(entrenador->atrapados); indice_pokemon++){
		pokemon_entrenador = list_get(entrenador->atrapados,indice_pokemon);
		if(strcmp(pokemon_entrenador, pokemon)==0)
			contador_pokemon_atrapado++;
	}

	if(contador_pokemon_atrapado > contador_pokemon_objetivo){
		return true;
	}
	else{
		return false;
	}

}

bool necesita_pokemon(t_entrenador* entrenador, char* pokemon){

	char* pokemon_auxiliar;
	int cantidad_objetivo = 0;
	int cantidad_atrapados = 0;

	//Recorremos la lista de objetivos del entrenador para saber cuantos pokemon de
	//determinada especie necesita en total

	for (int indice_pokemon=0; indice_pokemon<list_size(entrenador->objetivo); indice_pokemon++){
		pokemon_auxiliar = list_get(entrenador->objetivo, indice_pokemon);

		if(strcmp(pokemon_auxiliar, pokemon)==0){
			cantidad_objetivo++;
		}
	}
	//Recorremos la lista de atrapados para saber si ya tiene a todos los que necesita

	for (int indice_pokemon=0; indice_pokemon<list_size(entrenador->objetivo); indice_pokemon++){
			pokemon_auxiliar = list_get(entrenador->atrapados, indice_pokemon);

			if(strcmp(pokemon_auxiliar, pokemon)==0){
				cantidad_atrapados++;
			}
		}
	if(cantidad_atrapados < cantidad_objetivo){
		return true;
	}
	else{
		return false;
	}
}

bool esta_en_exit(t_entrenador* entrenador){
	if(entrenador->estado == EXIT)
		return true;
	else
		return false;
}


t_entrenador* obtener_entrenador_con_pokemon_del_entrenador1(t_entrenador* entrenador1){
	t_entrenador* entrenador2;
	char* pokemon_sobra_entrenador2;
	t_list* intercambiables = list_create();

	for(int i = 0; i<list_size(lista_de_entrenadores_deadlock); i++){
		entrenador2 =list_get(lista_de_entrenadores_deadlock, i);
		list_clean(intercambiables);
		for(int indice_pokemon = 0; indice_pokemon < list_size(entrenador2->atrapados); indice_pokemon++){
			pokemon_sobra_entrenador2 = list_get(entrenador2->atrapados, indice_pokemon);
			if(es_intercambiable_pokemon(entrenador2, pokemon_sobra_entrenador2)){
				list_add(intercambiables, pokemon_sobra_entrenador2);
			}
		}

		for(int indice_pokemon = 0; indice_pokemon < list_size(intercambiables); indice_pokemon++){
			pokemon_sobra_entrenador2 = list_get(intercambiables, indice_pokemon);
			if(necesita_pokemon(entrenador1, pokemon_sobra_entrenador2)){
				return entrenador2;
			}
		}
	}
	return entrenador2;
}

/*
t_entrenador* obtener_entrenador_con_pokemon_del_entrenador1(t_entrenador* entrenador1){
	t_entrenador* entrenador2;
	char* pokemon_sobra_entrenador1;
	t_list* intercambiables = list_create();

		for(int indice_pokemon = 0; indice_pokemon < list_size(entrenador1->atrapados); indice_pokemon++){
				pokemon_sobra_entrenador1 = list_get(entrenador1->atrapados, indice_pokemon);
				if(es_intercambiable_pokemon(entrenador1, pokemon_sobra_entrenador1)){
					list_add(intercambiables, pokemon_sobra_entrenador1);
				}
		}

	for(int i = 1; i<list_size(lista_de_entrenadores_deadlock); i++){
			entrenador2 =list_get(lista_de_entrenadores_deadlock, i);
		for(int indice_pokemon = 0; indice_pokemon < list_size(intercambiables); indice_pokemon++){
			pokemon_sobra_entrenador1 = list_get(intercambiables, indice_pokemon);
			if(necesita_pokemon(entrenador2, pokemon_sobra_entrenador1)){
				return entrenador2;
			}
		}
	}

	return entrenador2;
}*/










