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
#include<math.h>

typedef struct
{
	int tamanio_ocupado;
	int byte_comienzo_ocupado;
	int ultima_referencia;
	//t_particion_buddy* particion;
}t_mensaje_guardado;

typedef struct
{
	int tam_particion;
	int comienzo_particion;
	int final_de_particion;
}t_particion_buddy;

int tamanio_de_memoria;
t_list* elementos_en_memoria; // Lista de t_mensaje_guardado
t_list* elementos_en_buddy;
sem_t MUTEX_LISTA;

void* memoria_principal;
sem_t MUTEX_MEM_PRIN;

int timestamp;
sem_t MUTEX_TIMESTAMP;

int contador_fallos; // Para cuando la frecuencia de compactación es mayor a 2
sem_t MUTEX_FALLOS;

t_mensaje_guardado* guardar_mensaje_en_memoria(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar);

// REEMPLAZO
int ejecutar_algoritmo_reemplazo(void);
int reemplazar_segun_FIFO(void);
int reemplazar_segun_LRU(void);

// PARTICIONES DINÁMICAS
t_mensaje_guardado* eliminar_y_compactar_hasta_encontrar(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar);
void compactar_memoria(void);
t_mensaje_guardado* administracion_de_memoria_particiones(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar);
t_mensaje_guardado* agregar_segun_first_fit(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar);
int buscar_first_fit(int *se_guardo_el_mensaje, void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar);
t_mensaje_guardado* agregar_segun_best_fit(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar);
int buscar_best_fit(int *encontrado, void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar);

// BUDDY SYSTEM
t_mensaje_guardado* administracion_de_memoria_buddy_system(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar);
int for_best_fit_BS(int *encontrado, uint32_t tamanio_a_agregar);
int for_first_fit_BS(int *encontrado, uint32_t tamanio_a_agregar);
int particionar_buddy_system(uint32_t tamanio_a_agregar, int *tamanio_minimo, int index);
t_mensaje_guardado* eliminar_y_consolidar_hasta_encontrar(uint32_t tamanio_a_agregar, void* bloque_a_agregar_en_memoria);
void consolidar_buddy_system(int posicion_inicial_nuevo_mensaje);

// AUXILIARES
bool comparar_inicios_mensajes(t_mensaje_guardado* mensaje1, t_mensaje_guardado* mensaje2);
bool comparar_timestamps_mensajes(t_mensaje_guardado* mensaje1, t_mensaje_guardado* mensaje2);
bool comparar_tamanios_de_particiones(t_particion_buddy* particion1, t_particion_buddy* particion2);
bool comparar_inicios_de_particiones(t_particion_buddy* particion1, t_particion_buddy* particion2);
void mostrar_memoria_principal(void);
int primera_posicion_vacia_y_entra(uint32_t tamanio_a_agregar);
int toda_la_memoria_esta_ocupada(void);
int entra_en_hueco(int tamanio_a_agregar, int posicion_libre);
t_mensaje_guardado* guardar_en_posicion(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar, int posicion);
void* tratar_fragmentacion_interna(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar);
void* crear_fragmentacion_interna(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar, uint32_t tamanio_total);
int encontrar_mensaje_a_eliminar_por_posicion(int posicion, t_list* lista);

#endif /* UTILS_MEMORIA_H_ */
