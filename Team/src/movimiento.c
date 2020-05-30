#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "entrenador.h"

void moverse_izquierda(t_entrenador* entrenador){

	entrenador->posicion->x -= 1;
	log_movimiento_entrenador(entrenador);
	ciclos_de_cpu(1);
}
void moverse_derecha(t_entrenador* entrenador) {

	entrenador->posicion->x += 1;
	log_movimiento_entrenador(entrenador);
	ciclos_de_cpu(1);

}

void moverse_arriba(t_entrenador* entrenador){

	entrenador->posicion->y -= 1;
	log_movimiento_entrenador(entrenador);
	ciclos_de_cpu(1);
}

void moverse_abajo(t_entrenador* entrenador){

	entrenador->posicion->y += 1;
	log_movimiento_entrenador(entrenador);
	ciclos_de_cpu(1);
}


void armar_movimiento(t_entrenador* entrenador){

	t_accion* accionNueva = malloc(sizeof(t_accion));
		//accionNueva->accion = accion;
		accionNueva->ciclo_cpu = 1;

	int x_a_moverse = entrenador->pokemon_a_atrapar->posicion->x;
	int y_a_moverse = entrenador->pokemon_a_atrapar->posicion->y;

	int posicion_ficticia_entrenador_x = entrenador->posicion->x;
	int posicion_ficticia_entrenador_y = entrenador->posicion->y;

	while(posicion_ficticia_entrenador_x != x_a_moverse) {
			if(entrenador->posicion->x < x_a_moverse){
				//moverse derecha
		 		accionNueva->accion = moverse_derecha;
				queue_push(entrenador->cola_de_acciones,accionNueva);
				posicion_ficticia_entrenador_x ++;

			}
			else if(posicion_ficticia_entrenador_x > x_a_moverse){
				//moverse izquierda
				accionNueva->accion = moverse_izquierda;
				queue_push(entrenador->cola_de_acciones,accionNueva);
				posicion_ficticia_entrenador_x --;

			}
		}

	while(posicion_ficticia_entrenador_y != y_a_moverse){

			if(posicion_ficticia_entrenador_y < y_a_moverse){
				accionNueva->accion = moverse_abajo;
				queue_push(entrenador->cola_de_acciones,accionNueva);
				posicion_ficticia_entrenador_y ++;
				//moverse abajo
			}
			else if(posicion_ficticia_entrenador_y > y_a_moverse){
				accionNueva->accion = moverse_arriba;
				queue_push(entrenador->cola_de_acciones,accionNueva);
				posicion_ficticia_entrenador_y --;
			}

		}

}



