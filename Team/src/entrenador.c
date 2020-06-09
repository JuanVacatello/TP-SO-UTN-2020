#include "Team.h"
#include "entrenador.h"
#include "movimiento.h"
#include "utils.h"

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<readline/readline.h>
#include<stdbool.h>



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

	char** vector_atrapados = string_n_split(atrapados, cantidad_elementos,"|");

	t_list* atrapados_casteados = list_create();

	for(int i=0; i<cantidad_elementos; i++){

		list_add(atrapados_casteados, vector_atrapados[i]);

	}

	return atrapados_casteados;

}


int cantidad_de_elementos(char* pokemons){
	int contador = 0;
	for(int i=0; i < string_length(pokemons); i++){
		if(pokemons[i] =='|'){
			contador++;
		}
	}
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
	entrenador->estado = BLOCKED;// Hay que ver si termina aca

	enviar_mensaje_a_broker(socket_conexion_broker, 3 , entrenador); //HABRIA QUE VER SI ES VG EL SOCKET
}

void atrapar_pokemon(t_entrenador* entrenador){

	list_add(entrenador->atrapados, entrenador->pokemon_a_atrapar);
	entrenador->pokemon_a_atrapar = NULL;

	free(entrenador->pokemon_a_atrapar);


}

void puede_seguir_atrapando(t_entrenador* entrenador){

	if(list_size(entrenador->objetivo) == list_size(entrenador->atrapados)){

		if(termino_de_atrapar(entrenador)){
			//LO PASAMOS A EXIT
		}
		else{
			entrenador->estado = BLOCKED;
			}
	}
}

bool termino_de_atrapar(t_entrenador* entrenador){

	//LISTA DE OBJETIVOS MISMOS POKEMONES Y CANTIDADES QUE LISTA ATRAPADOS
	return true;
}








