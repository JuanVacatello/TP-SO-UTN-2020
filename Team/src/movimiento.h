#ifndef MOVIMIENTO_H_
#define MOVIMIENTO_H_

#include "entrenador.h"


#include <stdio.h>
#include <stdlib.h>

void moverse_izquierda(t_posicion* posicion);
void moverse_derecha(t_posicion* posicion);
void moverse_arriba(t_posicion* posicion);
void moverse_abajo(t_posicion* posicion);
void armar_movimiento(t_entrenador* entrenador);


#endif /* MOVIMIENTO_H_ */
