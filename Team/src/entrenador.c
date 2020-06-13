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

	//POSICION
	char** posiciones = obtener_posiciones_entrenadores();
	t_posicion posicion_entrenador = obtener_posicion(posiciones[indice]);
	entrenador->posicion = posicion_entrenador;

	//LISTA OBJETIVOS
	char** objetivos = obtener_objetivos_entrenadores();
	t_list* objetivo = obtener_objetivos(objetivos[indice]);
	entrenador->objetivo = objetivo;

	//LISTA ATRAPADOS
	char** atrapados = obtener_pokemon_entrenadores();
	t_list* atrapado = obtener_atrapados(atrapados[indice]);
	entrenador->atrapados = atrapado;

	//COLA DE ACCIONES
	t_queue* cola_de_acciones = queue_create();

	//ESTADO
	entrenador->estado = NEW;

	//CICLOS DE CPU
	entrenador->ciclos_de_cpu_totales = 0;

	//RAFAGA ANTERIOR
	entrenador->rafaga_anterior = 0;

	//ID_CATCH_POKEMON
	entrenador->ID_catch_pokemon = 0;

	return entrenador;
}

//"PIKACHU|PIKACHU|SQUIRTLE"  string_n_split ("texto", n, "|")

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


int cantidad_de_elementos(char* pokemons){
	int contador = 0;
	//if(pokemons != NULL || pokemons != '\0'){
		for(int i=0; i < string_length(pokemons); i++){
			if(pokemons[i] =='|')
				contador++;
		}
	//}
	//else{
	//	return 0;
	//}
	return contador+1;
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

	for (int indice_entrenador=0; indice_entrenador<cantidad_entrenadores(); indice_entrenador++){

		entrenador = list_get(entrenadores, indice_entrenador);
			posicion_entrenador = entrenador->posicion;
			distancia_actual=sacar_distancia(posicion_pokemon,posicion_entrenador);

			if(menor_distancia>distancia_actual || menor_distancia==-1){
				menor_distancia=distancia_actual;
				entrenador_cercano = entrenador;

		}
	}
	entrenador_cercano->ciclos_de_cpu_totales = entrenador_cercano->ciclos_de_cpu_totales + menor_distancia;
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

	entrenador->rafaga_anterior = 0;

	int contador_cpu = entrenador->ciclos_de_cpu_totales;

	pthread_t hilo_entrenador = entrenador->hilo_entrenador;

	entrenador->estado = EXEC;
	while(contador_cpu > 0) {
		t_accion* accion_a_ejecutar = queue_pop(entrenador->cola_de_acciones);
		entrenador->rafaga_anterior += accion_a_ejecutar->ciclo_cpu;

		pthread_create(&hilo_entrenador, NULL , accion_a_ejecutar->accion , entrenador);
		pthread_join(hilo_entrenador, NULL);

		contador_cpu -= accion_a_ejecutar->ciclo_cpu;
		free(accion_a_ejecutar->accion);
		free(accion_a_ejecutar->ciclo_cpu);
		free(accion_a_ejecutar);
	}

	void puede_seguir_atrapando(entrenador);	//Acá se fija si terminó, entró en deadlock o puede seguir atrapando
//	enviar_mensaje_a_broker(socket_conexion_broker, 3 , entrenador); //HABRIA QUE VER SI ES VG EL SOCKET
}

void atrapar_pokemon(t_entrenador* entrenador){

	list_add(entrenador->atrapados, entrenador->pokemon_a_atrapar);
	entrenador->pokemon_a_atrapar = NULL;

	free(entrenador->pokemon_a_atrapar);


}

void puede_seguir_atrapando(t_entrenador* entrenador){

	t_pokemon* pokemon;

	if(list_size(entrenador->objetivo) == list_size(entrenador->atrapados)){

	if(termino_de_atrapar(entrenador)){

		entrenador->estado = EXIT;

	}

	else{

			for (int indice_pokemon=0; indice_pokemon<list_size(entrenador->objetivo); indice_pokemon++){
				pokemon=list_get(entrenador->objetivo, indice_pokemon);
				if(comprobar_deadlock_entrenador(entrenador, pokemon)){

					entrenador->estado_deadlock = 1;	//VER SI ESTO ES NECESARIO

					list_add(lista_de_entrenadores_deadlock, entrenador);

					//PASARLO A DEADLOCK
				}
				entrenador->estado = BLOCKED;
			}


			}
	}

	else{
		entrenador->estado = BLOCKED;
	}
}



bool termino_de_atrapar(t_entrenador* entrenador){

	int tamanio_lista_objetivo = list_size(entrenador->objetivo);
	int tamanio_lista_atrapados = list_size(entrenador->atrapados);
	int contador_atrapados = 0;
	t_pokemon* pokemon;

	if(tamanio_lista_objetivo == tamanio_lista_atrapados){

	for (int indice_pokemon=0; indice_pokemon<list_size(entrenador->objetivo); indice_pokemon++){
		pokemon = list_get(entrenador->objetivo, indice_pokemon);
		if (termino_con_pokemon(entrenador, pokemon)){
			contador_atrapados++;
		}

	}

		if(contador_atrapados == tamanio_lista_objetivo){
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


bool termino_con_pokemon(t_entrenador* entrenador, t_pokemon* pokemon){

	t_pokemon* pokemon_auxiliar;
	int cantidad_objetivo = 0;
	int cantidad_atrapados = 0;

	//Recorremos la lista de objetivos del entrenador para saber cuantos pokemon de
	//determinada especie necesita en total

	for (int indice_pokemon=0; indice_pokemon<list_size(entrenador->objetivo); indice_pokemon++){

		pokemon_auxiliar = list_get(entrenador->objetivo, indice_pokemon);

		if(strcmp(pokemon_auxiliar->especie, pokemon->especie)==0){
			cantidad_objetivo++;
		}

	}

	//Recorremos la lista de atrapados para saber si ya tiene a todos los que necesita

	for (int indice_pokemon=0; indice_pokemon<list_size(entrenador->objetivo); indice_pokemon++){

			pokemon_auxiliar = list_get(entrenador->atrapados, indice_pokemon);

			if(strcmp(pokemon_auxiliar->especie, pokemon->especie)==0){
				cantidad_atrapados++;
			}

		}

	if(cantidad_atrapados >= cantidad_objetivo){
		return true;
	}
	else{
	return false;
	}
}

bool comprobar_deadlock_entrenador(t_entrenador* entrenador, t_pokemon* pokemon){

	t_pokemon* pokemon_auxiliar;
		int cantidad_objetivo = 0;
		int cantidad_atrapados = 0;

		//Recorremos la lista de objetivos del entrenador para saber cuantos pokemon de
		//determinada especie necesita en total

		for (int indice_pokemon=0; indice_pokemon<list_size(entrenador->objetivo); indice_pokemon++){

			pokemon_auxiliar = list_get(entrenador->objetivo, indice_pokemon);

			if(strcmp(pokemon_auxiliar->especie, pokemon->especie)==0){
				cantidad_objetivo++;
			}

		}

		//Recorremos la lista de atrapados para saber si supero la cantidad necesaria

		for (int indice_pokemon=0; indice_pokemon<list_size(entrenador->objetivo); indice_pokemon++){

				pokemon_auxiliar = list_get(entrenador->atrapados, indice_pokemon);

				if(strcmp(pokemon_auxiliar->especie, pokemon->especie)==0){
					cantidad_atrapados++;
				}

			}

		if(cantidad_atrapados > cantidad_objetivo){
			return true;
		}
		else{
		return false;
		}

}



