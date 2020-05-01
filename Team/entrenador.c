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


t_posicion moverse_A(t_posicion* posicion, t_posicion* posicionAMoverse)
{
	int posicionX =  posicionAMoverse -> x;
	int posicionY = posicionAMoverse -> y;

	while( posicion->x != posicionX || posicion->y != posicionY){
		if(posicion->x < posicionX){
			//moverse derecha
	 		void moverse_derecha(t_posicion* posicion);
		}
		else{
			//moverse arriba
			void moverse_izquierda(t_posicion* posicion);
		}
		if(posicion->y < posicionY){
			void moverse_abajo(t_posicion* posicion);
			//moverse abajo

		}
		else{
			void moverse_arriba(t_posicion* posicion);
		}
	}
		printf("Has llegado al pokemon");
		return posicion;
}




void ciclos_de_cpu(int ciclos){
	int segundos = transformarCiclos(ciclos);
	sleep(segundos);
}

int transformarCiclos(int ciclos){
	return ciclos*2;
}
