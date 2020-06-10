/*
 * GameCard.h
 *
 *  Created on: 7 jun. 2020
 *      Author: utnso
 */

#ifndef GAMECARD_H_
#define GAMECARD_H_

#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<commons/string.h>
#include<commons/txt.h>
#include<commons/bitarray.h>
#include<string.h>
#include"Utils/configGameCard.h"
#include<stdlib.h>


void inicializar_file_system(char* punto_montaje);
char* concat(char* string1,char* string2);
void inicializar_metadata(char* path_metadata, int block_size, int cant_bloques);
void obtener_bloque_pokemon(char* pokemon,char* path_files);
void completar_metadata_directorio(char* path_directorio);

#endif /* GAMECARD_H_ */
