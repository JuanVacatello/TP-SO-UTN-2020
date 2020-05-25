#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "entrenador.h"

void moverse_izquierda(t_entrenador* entrenador){

	entrenador->posicion->x -= 1;
	//completar_logger("El entrenador se movio una posicion a la izquierda", ,);

	ciclos_de_cpu(1);
}
void moverse_derecha(t_entrenador* entrenador) {

	entrenador->posicion->x += 1;
	//completar_logger("El entrenador se movio una posicion a la derecha", ,);
	ciclos_de_cpu(1);

}

void moverse_arriba(t_entrenador* entrenador){

	entrenador->posicion->y -= 1;
	//completar_logger("El entrenador se movio una posicion a la arriba", ,);


	//completar_logger("No es posible realizar ese movimiento");

	ciclos_de_cpu(1);
}

void moverse_abajo(t_entrenador* entrenador){

	entrenador->posicion->y += 1;
	//completar_logger("El entrenador se movio una posicion a la abajo", ,);
	ciclos_de_cpu(1);
}






