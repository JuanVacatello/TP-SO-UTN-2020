#ifndef GAMECARD_H_
#define GAMECARD_H_

#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<commons/string.h>
#include<commons/txt.h>
#include<commons/bitarray.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

#include"Utils/configGameCard.h"
#include"Utils/metadata.h"
#include"Utils/conexion.h"
#include"Utils/bitmap.h"
#include"Utils/logGameCard.h"
#include"Utils/bloques.h"

pthread_t hilo_new_pokemon;
pthread_t hilo_catch_pokemon;
pthread_t hilo_get_pokemon;

sem_t MUTEX_PRUEBA;

void inicializar_variables_globales(void);
void inicializar_file_system(char* punto_montaje);
char* concat(char* string1,char* string2);
void inicializar_metadata(char* path_metadata, int block_size, int cant_bloques);
void inicializar_files(char* path_files);
void inicializar_bloques(char* path_bloques);
void completar_metadata_directorio(char* path_directorio);
void crear_bloques(char* path_bloques);

#endif /* GAMECARD_H_ */
