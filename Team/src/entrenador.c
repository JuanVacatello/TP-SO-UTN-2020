#include "Team.h"
#include "entrenador.h"
#include "movimiento.h"
#include "utils/utils.h"

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<readline/readline.h>
#include<stdbool.h>


t_entrenador* moverse_A(t_entrenador* entrenador, t_posicion* posicionAMoverse)
{
	int posicionXPokemon =  posicionAMoverse -> x;
	int posicionYPokemon = posicionAMoverse -> y;

	while(entrenador->posicion->x != posicionXPokemon) {
		if(entrenador->posicion->x < posicionXPokemon){
			//moverse derecha
	 		moverse_derecha(entrenador->posicion, config);
		}
		else{
			//moverse arriba
			moverse_izquierda(entrenador->posicion, config);
		}

		efectuar_ciclo_cpu(entrenador, 1);
	}

	while(entrenador->posicion->y != posicionYPokemon){

		if(entrenador->posicion->y < posicionYPokemon){
			moverse_abajo(entrenador->posicion, config);
			//moverse abajo
		}
		else if(entrenador->posicion->y > posicionYPokemon){
			moverse_arriba(entrenador->posicion, config);
		}

		efectuar_ciclo_cpu(entrenador, 1);
	}

		printf("Has llegado al pokemon");
		return entrenador;

}
/*
t_entrenador* atrapar_Pokemon(t_entrenador* entrenador, t_pokemon* pokemon){ //PREGUNTAR POR ESTA FUNCION

	int ultimaPosicion=0;

	for(int i=0; i<sizeof(entrenador->atrapados);i++){
		ultimaPosicion = i;
	}

	entrenador->atrapados[ultimaPosicion+1] = pokemon;
	entrenador->estado = 1;

	return entrenador;
}
*/



//------------CICLOS DE CPU---------------

void efectuar_ciclo_cpu(t_entrenador* entrenador, int ciclos){
	contabilizar_ciclos( entrenador,ciclos);
	ciclos_de_cpu(ciclos);
}

void contabilizar_ciclos(t_entrenador* entrenador, int ciclos){
	entrenador->ciclos_de_cpu += ciclos;
}

int transformarCiclos(int ciclos){
	int retardo;
	retardo = obtener_retardo_ciclo_cpu();
	return ciclos * retardo ;
}

void ciclos_de_cpu(int ciclos){
	int segundos = transformarCiclos(ciclos);
	sleep(segundos);
}

//ARMAMOS TODOS LOS ENTRENADORES




//ARMAMOS EL ENTRENADOR


t_entrenador* armar_entrenador(int indice){

	t_entrenador* entrenador = malloc(sizeof(entrenador));

	//POSICION
	t_list* posiciones = obtener_posiciones_entrenadores();
	t_posicion* posicion_entrenador = obtener_posicion(list_get(posiciones,indice));
	entrenador->posicion = posicion_entrenador;

	//LISTA OBJETIVOS
	t_list* objetivos = obtener_objetivos_entrenadores();
	t_list* objetivo = obtener_objetivos(list_get(objetivos, indice));
	entrenador->objetivo = objetivo;

	//LISTA ATRAPADOS
	t_list* atrapados = obtener_pokemon_entrenadores();
	t_list* atrapado = obtener_atrapados(list_get(atrapados, indice));
	entrenador->atrapados = atrapado;

	//ESTADO
	entrenador->estado = READY;

	return entrenador;
}

//"PIKACHU|PIKACHU|SQUIRTLE"  string_n_split ("texto", n, "|")

t_posicion* obtener_posicion(char* posicion){

	char** vector_posicion = string_split(posicion, "|");
	int posicion_x = list_get(vector_posicion,0);
	int posicion_y = list_get(vector_posicion,1);

	t_posicion* posicion_casteada;
	posicion_casteada->x = posicion_x;
	posicion_casteada->y = posicion_y;

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
		if(pokemons[i] =="|"){
			contador++;
		}
	}

	return contador+1;
}


//DADO UN POKEMON EVALUAR QUE ENTRENADOR ESTÁ MAS CERCA//

t_list* entrenadores_mas_cercanos(t_pokemon* pokemon){

	t_list* entrenadores = lista_de_entrenadores;

	t_list* entrenadores_cercanos;

	t_posicion* posicion_pokemon = pokemon->posicion;

	t_entrenador* entrenador;

	t_posicion* posicion_entrenador;

	int menor_distancia=-1;

	for (int indice_entrenador=0; indice_entrenador<cantidad_entrenadores(); indice_entrenador++){

		entrenador = list_get(entrenadores, indice_entrenador);
		posicion_entrenador = entrenador->posicion;
		int distancia_actual;
		distancia_actual=sacar_distancia(posicion_pokemon,posicion_entrenador);

		if(menor_distancia>=distancia_actual || menor_distancia==-1){
			menor_distancia=distancia_actual;
			list_add(entrenadores_cercanos,entrenador);
		}
	}
	return entrenadores_cercanos;
}

//OBTIENE CANTIDAD DE MOVIMIENTOS A REALIZAR PARA MOVILIZARSE//

int sacar_distancia(t_posicion* pokeposicion,t_posicion* entreposicion){

	int x,y,distancia;

	x = pokeposicion->x - entreposicion->x;
	y = pokeposicion->y - entreposicion->y;

	distancia = abs(x) + abs(y);

	return distancia;
}


t_entrenador* prioridad_fifo(t_list* entrenadores){

	t_entrenador*  entrenador;

	entrenador = list_get(entrenadores,0);

	return entrenador;
}


