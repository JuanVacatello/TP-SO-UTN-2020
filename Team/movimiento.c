#include "entrenador.h"

#include <stdio.h>
#include <stdlib.h>


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



void moverse_izquierda(t_posicion* posicion){


	if(sePuedeMover(posicion->x)){
	posicion->x -= 1;
	//completar_logger("El entrenador se movio una posicion a la izquierda", ,);
	}
	else{
	//completar_logger("No es posible realizar ese movimiento");
}
	ciclos_de_cpu(1);
}
void moverse_derecha(t_posicion* posicion) {

	posicion->x += 1;
	//completar_logger("El entrenador se movio una posicion a la derecha", ,);
	ciclos_de_cpu(1);

}

void moverse_arriba(t_posicion* posicion){
	if(sePuedeMover(posicion->y)){
	posicion->y -= 1;
	//completar_logger("El entrenador se movio una posicion a la arriba", ,);
	}
	else{
	//completar_logger("No es posible realizar ese movimiento");
}
	ciclos_de_cpu(1);
}

void moverse_abajo(t_posicion* posicion){

	posicion->y += 1;
	//completar_logger("El entrenador se movio una posicion a la abajo", ,);
	ciclos_de_cpu(1);
}

_Bool sePuedeMover(int posicion){
	return posicion != 0;
}
