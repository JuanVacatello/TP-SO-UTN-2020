/*
 * files.h
 *
 *  Created on: 22 jun. 2020
 *      Author: utnso
 */

#ifndef UTILS_FILES_H_
#define UTILS_FILES_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<commons/string.h>
#include<commons/txt.h>
#include<commons/bitarray.h>
#include<commons/collections/list.h>


#include"configGameCard.h"
#include"metadata.h"
#include"conexion.h"
#include"bitmap.h"



char* obtener_path_pokemon(char * pokemon);
int existe_posicion_pokemon(char* pokemon, int posX, int posY);
void insertar_nueva_linea(char* path_pokemon, int posX, int posY);
char* generar_linea_a_insertar(int posX, int posY, int cantidad);

#endif /* UTILS_FILES_H_ */
