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


t_accion* armar_movimiento(t_entrenador* entrenador){

	t_accion* accionNueva;
		//accionNueva->accion = accion;
		accionNueva->ciclo_cpu = 1;

	int x_a_moverse = entrenador->pokemon_a_atrapar->posicion->x;
	int y_a_moverse = entrenador->pokemon_a_atrapar->posicion->y;

	while(entrenador->posicion->x != x_a_moverse) {
			if(entrenador->posicion->x < x_a_moverse){
				//moverse derecha
		 		accionNueva->accion = moverse_derecha;
		 		return accionNueva;
			}
			else{
				//moverse izquierda
				accionNueva->accion = moverse_derecha;
				return accionNueva;
			}
		}

	while(entrenador->posicion->y != y_a_moverse){

			if(entrenador->posicion->y < y_a_moverse){
				accionNueva->accion = moverse_abajo;
				return accionNueva;
				//moverse abajo
			}
			else if(entrenador->posicion->y > y_a_moverse){
				accionNueva->accion = moverse_arriba;
				return accionNueva;
			}

		}

}



