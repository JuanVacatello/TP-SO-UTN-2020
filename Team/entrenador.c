#include "Team.h"
#include "entrenador.h"
#include "movimiento.c"

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<stdbool.h>

typedef enum{

	READY = 1,
	BLOCKED = 2

}op_estado;

typedef struct
{
	int x;
	int y;

} t_posicion;


typedef struct
{
	char* especie;
	char* tipo;
	t_posicion posicion;

} t_pokemon;

typedef struct
{
	int ciclos_de_cpu;
	op_estado estado;
	t_pokemon objetivo[];
	t_posicion posicion;
	t_pokemon atrapados[];

} t_entrenador;

typedef struct
{
	t_pokemon objetivoGlobal[];
	t_posicion posicion;
	t_pokemon atrapadosGlobal[];

} t_team;

/*
t_posicion* posicion = {1,2};
t_posicion* posicionAMoverse = {3,3};
*/


t_entrenador moverse_A(t_entrenador* entrenador, t_posicion* posicionAMoverse)
{
	int posicionXPokemon =  posicionAMoverse -> x;
	int posicionYPokemon = posicionAMoverse -> y;

	while(entrenador->posicion->x != posicionXPokemon) {
		if(entrenador->posicion->x < posicionXPokemon){
			//moverse derecha
	 		moverse_derecha(entrenador->posicion);
		}
		else{
			//moverse arriba
			moverse_izquierda(entrenador->posicion);
		}

		efectuar_ciclo_cpu(entrenador, 1);
	}

	while(entrenador->posicion->y != posicionYPokemon){

		if(entrenador->posicion->y < posicionYPokemon){
			moverse_abajo(entrenador->posicion);
			//moverse abajo
		}
		else if(entrenador->posicion->y > posicionYPokemon){
			moverse_arriba(entrenador->posicion);
		}

		efectuar_ciclo_cpu(entrenador, 1);
	}

		printf("Has llegado al pokemon");
		return entrenador;

}

t_entrenador atrapar_Pokemon(t_entrenador* entrenador, t_pokemon* pokemon){ //PREGUNTAR POR ESTA FUNCION

	int ultimaPosicion=0;

	for(int i=0; i<sizeof(entrenador->atrapados);i++){
		ultimaPosicion = i;
	}

	entrenador->atrapados[ultimaPosicion+1] = pokemon;
	entrenador->estado = READY;

	return entrenador;
}




//------------CICLOS DE CPU---------------

void efectuar_ciclo_cpu(t_entrenador entrenador, int ciclos){
	contabilizar_ciclos( entrenador,ciclos);
	ciclos_de_cpu(ciclos);
}

void contabilizar_ciclos(t_entrenador* entrenador, int ciclos){
	entrenador->ciclos_de_cpu += ciclos;
}

int transformarCiclos(int ciclos){
	return ciclos*2;
}

void ciclos_de_cpu(int ciclos){
	int segundos = transformarCiclos(ciclos);
	sleep(segundos);
}


