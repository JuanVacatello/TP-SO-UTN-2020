#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#include "entrenador.h"

void moverse_izquierda(t_posicion* posicion, t_config* config){


	if(sePuedeMover(posicion->x)){
	posicion->x -= 1;
	//completar_logger("El entrenador se movio una posicion a la izquierda", ,);
	}
	else{
	//completar_logger("No es posible realizar ese movimiento");
}
	ciclos_de_cpu(1, config);
}
void moverse_derecha(t_posicion* posicion, t_config* config) {

	posicion->x += 1;
	//completar_logger("El entrenador se movio una posicion a la derecha", ,);
	ciclos_de_cpu(1, config);

}

void moverse_arriba(t_posicion* posicion, t_config* config){
	if(sePuedeMover(posicion->y)){
	posicion->y -= 1;
	//completar_logger("El entrenador se movio una posicion a la arriba", ,);
	}
	else{
	//completar_logger("No es posible realizar ese movimiento");
}
	ciclos_de_cpu(1, config);
}

void moverse_abajo(t_posicion* posicion, t_config* config){

	posicion->y += 1;
	//completar_logger("El entrenador se movio una posicion a la abajo", ,);
	ciclos_de_cpu(1, config);
}
/*
_Bool sePuedeMover(int posicion){
	return posicion != 0;
}*/

