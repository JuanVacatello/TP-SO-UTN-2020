
#ifndef UTILS_MEMORIA_H_
#define UTILS_MEMORIA_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/string.h>
#include<string.h>
#include<pthread.h>
#include"configBroker.h"

void* memoria_principal;
int desplazamiento;
char* algoritmo_reemplazo;
int tamanio_minimo_particion;
int frecuencia_compactacion;

void guardar_mensaje_en_memoria(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar);

void administracion_de_memoria_particiones(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar);

#endif /* UTILS_MEMORIA_H_ */
