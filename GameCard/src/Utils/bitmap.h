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

t_bitarray *bitarray;

// t_log *logger;
t_bitarray *bitarray;
int bitmap_fd;
int flag_bloques_libres;
char *bitarrayContent;

pthread_mutex_t MUTEX_BITMAP;



#ifndef UTILS_BITMAP_H_
#define UTILS_BITMAP_H_

void crear_bitmap(char* path_bitmap);

#endif /* UTILS_BITMAP_H_ */
