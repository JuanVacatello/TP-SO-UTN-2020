/*
 * bitmap.h
 *
 *  Created on: 21 jun. 2020
 *      Author: utnso
 */


#include<string.h>
#include<stdio.h>
#include<semaphore.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<unistd.h>
#include<ctype.h>
#include<xlocale.h>
#include<commons/bitarray.h>
#include<commons/config.h>
#include<commons/string.h>

#include"bloques.h"
#include"metadata.h"
#include"configGameCard.h"



// t_log *logger;
t_bitarray *bitarray;
int bitmap_fd;
int flag_bloques_libres;
char *bitarrayContent;

pthread_mutex_t MUTEX_BITMAP;

sem_t semaforo_bitmap;



#ifndef UTILS_BITMAP_H_
#define UTILS_BITMAP_H_

void crear_bitmap(char* path_bitmap);
void eliminar_bitmap();
void bitmap_liberar_bloque(int bloque);
int obtener_nuevo_bloque();

#endif /* UTILS_BITMAP_H_ */
