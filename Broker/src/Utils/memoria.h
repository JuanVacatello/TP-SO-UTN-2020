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
#include<semaphore.h>
#include"configBroker.h"
#include"logBroker.h"
#include<time.h>

typedef struct
{
	int tamanio_ocupado;
	int byte_comienzo_ocupado;
	int ultima_referencia;
}t_mensaje_guardado;

int tamanio_minimo_particion;
int tamanio_de_memoria;
t_list* elementos_en_memoria; // Lista de t_mensaje_guardado

void* memoria_principal;
sem_t mutex_mem_princial;

int timestamp;
sem_t mutex_timestamp;

int contador_fallos; // Para cuando la frecuencia de compactación es mayor a 2
sem_t mutex_contador_fallos;

t_mensaje_guardado* guardar_mensaje_en_memoria(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar);

t_mensaje_guardado* administracion_de_memoria_particiones(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar);
t_mensaje_guardado* agregar_segun_first_fit(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar);
t_mensaje_guardado* agregar_segun_best_fit(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar);

t_mensaje_guardado* administracion_de_memoria_buddy_system(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar);

int ejecutar_algoritmo_reemplazo(void);
int reemplazar_segun_FIFO(void);
int reemplazar_segun_LRU(void);
t_mensaje_guardado* eliminar_y_compactar_hasta_encontrar(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar);

bool comparar_inicios_mensajes(t_mensaje_guardado* mensaje1, t_mensaje_guardado* mensaje2);
bool comparar_timestamps_mensajes(t_mensaje_guardado* mensaje1, t_mensaje_guardado* mensaje2);

int toda_la_memoria_esta_ocupada(void);
void mostrar_memoria_principal(void);
int compactar_memoria(void);
int entra_en_hueco(int tamanio_a_agregar, int posicion_libre);
int primera_posicion_vacia_y_entra(uint32_t tamanio_a_agregar);
void* tratar_fragmentacion_interna(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar);
t_mensaje_guardado* guardar_en_posicion(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar, int posicion);

#endif /* UTILS_MEMORIA_H_ */
