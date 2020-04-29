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
		//moverse izquierda
		int moverse_izquierda(int x);
		//moverse derecha
		int moverse_derecha(int x);
		//moverse arriba
		//moverse abajo
}

int moverse_izquierda(int x){
	x--;
	return x;
	completar_logger("El entrenador se movio una posicion a la izquierda", ,)
}
int moverse_derecha(int x) {
	x++;
	return x;
	completar_logger("El entrenador se movio una posicion a la derecha", ,)
}

int moverse_arriba(int y){
	y--;
	return y;
	completar_logger("El entrenador se movio una posicion a la arriba", ,)
}

int moverse_abajo(int y, int y){
	y++;
	return y;
	completar_logger("El entrenador se movio una posicion a la abajo", ,)
}
