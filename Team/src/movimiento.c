#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "entrenador.h"

void moverse_izquierda(t_entrenador* entrenador){

	entrenador->posicion.x -= 1;
	ciclos_de_cpu(1);
	log_movimiento_entrenador(entrenador);
}
void moverse_derecha(t_entrenador* entrenador) {

	entrenador->posicion.x += 1;
	ciclos_de_cpu(1);
	log_movimiento_entrenador(entrenador);

}

void moverse_arriba(t_entrenador* entrenador){

	entrenador->posicion.y -= 1;
	ciclos_de_cpu(1);
	log_movimiento_entrenador(entrenador);
}

void moverse_abajo(t_entrenador* entrenador){

	entrenador->posicion.y += 1;
	ciclos_de_cpu(1);
	log_movimiento_entrenador(entrenador);
}


void armar_movimiento(t_entrenador* entrenador){

	int x_a_moverse, y_a_moverse;

	t_accion* accionNuevaX = malloc(sizeof(t_accion));
	accionNuevaX->ciclo_cpu = 1;
	t_accion* accionNuevaY = malloc(sizeof(t_accion));
	accionNuevaY->ciclo_cpu = 1;

	if(entrenador->posicionIntercambio.x != NULL){
		x_a_moverse = entrenador->posicionIntercambio.x;
		y_a_moverse = entrenador->posicionIntercambio.y;
	}
	else{
		x_a_moverse = entrenador->pokemon_a_atrapar->posicion.x;
		y_a_moverse = entrenador->pokemon_a_atrapar->posicion.y;
	}

	int posicion_ficticia_entrenador_x = entrenador->posicion.x;
	int posicion_ficticia_entrenador_y = entrenador->posicion.y;

	while(posicion_ficticia_entrenador_x != x_a_moverse) {
			if(posicion_ficticia_entrenador_x < x_a_moverse){
				//moverse derecha
				accionNuevaX->accion = moverse_derecha;
				list_add(entrenador->cola_de_acciones,accionNuevaX);
				posicion_ficticia_entrenador_x ++;

			}
			else if(posicion_ficticia_entrenador_x > x_a_moverse){
				//moverse izquierda
				accionNuevaX->accion = moverse_izquierda;
				list_add(entrenador->cola_de_acciones,accionNuevaX);
				posicion_ficticia_entrenador_x --;

			}
		}

	while(posicion_ficticia_entrenador_y != y_a_moverse){

			if(posicion_ficticia_entrenador_y < y_a_moverse){
				accionNuevaY->accion = moverse_abajo;
				list_add(entrenador->cola_de_acciones,accionNuevaY);
				posicion_ficticia_entrenador_y ++;
				//moverse abajo
			}
			else if(posicion_ficticia_entrenador_y > y_a_moverse){
				accionNuevaY->accion = moverse_arriba;
				list_add(entrenador->cola_de_acciones,accionNuevaY);
				posicion_ficticia_entrenador_y --;
			}

		}

}



