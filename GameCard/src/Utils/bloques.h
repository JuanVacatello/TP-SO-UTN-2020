/*
 * bloques.h
 *
 *  Created on: 21 jun. 2020
 *      Author: utnso
 */

#ifndef UTILS_BLOQUES_H_
#define UTILS_BLOQUES_H_

#include<stdio.h>
#include<commons/string.h>
#include<sys/stat.h>

#include"metadata.h"
#include"configGameCard.h"

int bloque_esta_vacio(char* bloque);
int existe_bloque(char* path_bloques,char* numero_de_bloque);

#endif /* UTILS_BLOQUES_H_ */
