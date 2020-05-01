#include "Team.h"

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>

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


t_posicion* posicion = {1,2};
t_posicion* posicionAMoverse = {3,3};


void moverse_A(t_posicion* posicion, t_posicion* posicionAMoverse)
{
	int posicionX =  posicionAMoverse -> x;
	int posicionY = posicionAMoverse -> y;

	if( posicion->x == posicionX && posicion->y == posicionY)
		printf("Has llegado al pokemon")
	else
	{
		//moverse izquierda
		void moverse_izquierda(t_posicion* posicion);
		//moverse derecha
		void moverse_derecha(t_posicion* posicion);
		//moverse arriba
		void moverse_arriba(t_posicion* posicion);
		//moverse abajo
		void moverse_abajo(t_posicion* posicion);
	}
}

void moverse_izquierda(t_posicion* posicion){

	posicion->x -= 1;
	//completar_logger("El entrenador se movio una posicion a la izquierda", ,);
}
void moverse_derecha(t_posicion* posicion) {

	posicion->x += 1;
	//completar_logger("El entrenador se movio una posicion a la derecha", ,);
}

void moverse_arriba(t_posicion* posicion){

	posicion->y += 1;
	//completar_logger("El entrenador se movio una posicion a la arriba", ,);
}

void moverse_abajo(t_posicion* posicion){

	posicion->y -= 1;
	//completar_logger("El entrenador se movio una posicion a la abajo", ,);
}
