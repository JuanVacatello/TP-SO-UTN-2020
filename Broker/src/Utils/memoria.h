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

typedef struct
{
	int tamanio_ocupado;
	int byte_comienzo_ocupado;
}t_mensaje_guardado;

void* memoria_principal;
int tamanio_de_memoria;
int desplazamiento_memoria_principal;
t_list* elementos_en_memoria; // Lista de t_mensaje_guardado

char* algoritmo_reemplazo;
int tamanio_minimo_particion;
int frecuencia_compactacion;

void guardar_mensaje_en_memoria(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar);

void administracion_de_memoria_particiones(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar);
void agregar_segun_first_fit(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar);
void agregar_segun_best_fit(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar);

void administracion_de_memoria_buddy_system(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar);

int comparar_inicios_mensajes(t_mensaje_guardado* mensaje1, t_mensaje_guardado* mensaje2);
int primera_posicion_vacia_y_entra(uint32_t tamanio_a_agregar);
void guardar_en_primera_posicion(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar, t_mensaje_guardado* mensaje_nuevo);


#endif /* UTILS_MEMORIA_H_ */
