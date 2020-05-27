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



//------------CICLOS DE CPU---------------

void efectuar_ciclo_cpu(t_entrenador* entrenador, int ciclos){
	//contabilizar_ciclos( entrenador,ciclos);
	ciclos_de_cpu(ciclos);
}

void contabilizar_ciclos(t_entrenador* entrenador, int ciclos){
	entrenador->ciclos_de_cpu_totales += ciclos;
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

	t_entrenador* entrenador = malloc(sizeof(t_entrenador));

	//POSICION
	char** posiciones = obtener_posiciones_entrenadores();
	t_posicion* posicion_entrenador = obtener_posicion(posiciones[indice]);
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

	return entrenador;
}

//"PIKACHU|PIKACHU|SQUIRTLE"  string_n_split ("texto", n, "|")

t_posicion* obtener_posicion(char* posicion){

	char** vector_posicion = string_split(posicion, "|");
	int posicion_x = atoi(vector_posicion[0]);
	int posicion_y = atoi(vector_posicion[1]);

	t_posicion* posicion_casteada = malloc(sizeof(t_posicion));
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
		if(pokemons[i] =='|'){
			contador++;
		}
	}

	return contador+1;
}


//DADO UN POKEMON EVALUAR QUE ENTRENADOR ESTÁ MAS CERCA//

t_entrenador* entrenador_mas_cercano(t_pokemon* pokemon){

	t_list* entrenadores = lista_de_entrenadores;

	t_posicion* posicion_pokemon = pokemon->posicion;

	t_entrenador* entrenador;

	t_entrenador* entrenador_cercano;

	t_posicion* posicion_entrenador;

	int menor_distancia=-1;

	for (int indice_entrenador=0; indice_entrenador<cantidad_entrenadores(); indice_entrenador++){

		entrenador = list_get(entrenadores, indice_entrenador);
		if(entrenador->estado == NEW || entrenador->estado == BLOCKED){
			posicion_entrenador = entrenador->posicion;
			int distancia_actual;
			distancia_actual=sacar_distancia(posicion_pokemon,posicion_entrenador);

			if(menor_distancia>distancia_actual || menor_distancia==-1){
				menor_distancia=distancia_actual;
				entrenador_cercano = entrenador;
			}
		}
	}
	entrenador_cercano->ciclos_de_cpu_totales = entrenador_cercano->ciclos_de_cpu_totales + menor_distancia;
	return entrenador_cercano;
}

//OBTIENE CANTIDAD DE MOVIMIENTOS A REALIZAR PARA MOVILIZARSE//

int sacar_distancia(t_posicion* pokeposicion,t_posicion* entreposicion){

	int x,y,distancia;

	x = pokeposicion->x - entreposicion->x;
	y = pokeposicion->y - entreposicion->y;

	distancia = abs(x) + abs(y);

	return distancia;
}


/*
void moverse_A(t_entrenador* entrenador) //, t_posicion* posicionAMoverse)
{
	int contador_cpu = 0;
	int x_a_moverse =  entrenador->pokemon_a_atrapar->posicion->x;
	int y_a_moverse = entrenador->pokemon_a_atrapar->posicion->y;

	while(entrenador->posicion->x != x_a_moverse) {
		if(entrenador->posicion->x < x_a_moverse){
			//moverse derecha
	 		moverse_derecha(entrenador->posicion);
		}
		else{
			//moverse arriba
			moverse_izquierda(entrenador->posicion);
		}

		contador_cpu++;
		//efectuar_ciclo_cpu(entrenador, 1);
	}

	while(entrenador->posicion->y != y_a_moverse){

		if(entrenador->posicion->y < y_a_moverse){
			moverse_abajo(entrenador->posicion);
			//moverse abajo
		}
		else if(entrenador->posicion->y > y_a_moverse){
			moverse_arriba(entrenador->posicion);
		}

		contador_cpu++;
		//efectuar_ciclo_cpu(entrenador, 1);
	}

		printf("Has llegado al pokemon");

		contabilizar_ciclos(entrenador, contador_cpu);
}
*/












