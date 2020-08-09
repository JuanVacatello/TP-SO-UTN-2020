/*
 * metadata.h
 *
 *  Created on: 11 jun. 2020
 *      Author: utnso
 */

#ifndef UTILS_METADATA_H_
#define UTILS_METADATA_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<commons/string.h>
#include<semaphore.h>

#include"configGameCard.h"

t_config* metadata_tall_grass;
pthread_mutex_t MUTEX_ELSOLUCIONES;

void leer_metadata_tall_grass(char* path_metadata_tall_grass);

t_config* leer_metadata_pokemon(char* path_metadata);

char** obtener_bloques_pokemon(t_config* metadata_pokemon);
char* obtener_bloques_pokemon_string(t_config* metadata_pokemon);
int obtener_cantidad_bloques_pokemon(t_config* metadata_pokemon);
void asignar_bloque_pokemon(t_config* metadata_pokemon);
void modificar_campo_bloques_metadata(t_config* metadata_pokemon,char* bloques);
int archivo_esta_abierto(t_config* metadata_pokemon);
int obtener_tamanio_archivo(t_config* metadata_pokemon);

int tamanio_array_en_string(char* array);




#endif /* UTILS_METADATA_H_ */
