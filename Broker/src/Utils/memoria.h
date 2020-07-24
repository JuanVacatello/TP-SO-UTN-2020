#ifndef UTILS_MEMORIA_H_
#define UTILS_MEMORIA_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/txt.h>
#include<commons/temporal.h>
#include<string.h>
#include<pthread.h>
#include<semaphore.h>
#include"configBroker.h"
#include"logBroker.h"
#include<time.h>
#include<signal.h>
#include<time.h>
#include<errno.h>
#include<assert.h>

typedef enum
{
	SUSCRIPTOR=0,
	NEW_POKEMON=1,
	APPEARED_POKEMON=2,
	CATCH_POKEMON=3,
	CAUGHT_POKEMON=4,
	GET_POKEMON=5,
	LOCALIZED_POKEMON=6,
	MENSAJE=7
} op_code;

typedef struct
{
	int tamanio_ocupado;
	int byte_comienzo_ocupado;
	int ultima_referencia;
	op_code cola;
	int id;
}t_mensaje_guardado;

typedef struct {

	uint32_t id_mensaje_correlativo;
	t_list* suscriptores_ack;
	uint32_t tamanio_buffer;
	t_mensaje_guardado* ubicacion_mensaje;
	char* pokemon;
} t_mensaje_en_cola;

typedef struct
{
	int tam_particion;
	int comienzo_particion;
	int final_de_particion;
}t_particion_buddy;

t_list* suscriptores_new_pokemon;
t_list* suscriptores_appeared_pokemon;
t_list* suscriptores_catch_pokemon;
t_list* suscriptores_caught_pokemon;
t_list* suscriptores_get_pokemon;
t_list* suscriptores_localized_pokemon;

t_list* mensajes_de_cola_new_pokemon;
t_list* mensajes_de_cola_appeared_pokemon;
t_list* mensajes_de_cola_catch_pokemon;
t_list* mensajes_de_cola_caught_pokemon;
t_list* mensajes_de_cola_get_pokemon;
t_list* mensajes_de_cola_localized_pokemon;

t_list* elementos_en_memoria; // Lista de t_mensaje_guardado
t_list* elementos_en_buddy;
t_list* lista_de_todos_los_mensajes;

void* memoria_principal;
int contador_fallos; // Para cuando la frecuencia de compactación es mayor a 2
int tamanio_de_memoria;
int timestamp;
uint32_t mensaje_id;
sem_t MUTEX_MENSAJE_ID;
sem_t MUTEX_TIMESTAMP;
sem_t MUTEX_MEM_PRIN;

// Guardado de mensaje en memoria
t_mensaje_guardado* guardar_mensaje_en_memoria(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar, op_code cola);
void aplicar_timestamp_e_id(t_mensaje_guardado* mensaje_nuevo);

// REEMPLAZO
int ejecutar_algoritmo_reemplazo(void);
void eliminar_de_generales(int posicion_liberada);
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
int evaluar_posible_consolidacion(int posicion_inicial_nuevo_mensaje, int* hay_que_consolidar);

// AUXILIARES
bool comparar_inicios_mensajes(t_mensaje_guardado* mensaje1, t_mensaje_guardado* mensaje2);
bool comparar_timestamps_mensajes(t_mensaje_guardado* mensaje1, t_mensaje_guardado* mensaje2);
bool comparar_tamanios_de_particiones(t_particion_buddy* particion1, t_particion_buddy* particion2);
bool comparar_inicios_de_particiones(t_particion_buddy* particion1, t_particion_buddy* particion2);
void mostrar_memoria_principal(void);
int primera_posicion_vacia_y_entra(uint32_t tamanio_a_agregar);
int toda_la_memoria_esta_ocupada(void);
int espacio_liberado_despues_de_eliminar(int posicion_inicial);
int entra_en_hueco(int tamanio_a_agregar, int posicion_libre);
t_mensaje_guardado* guardar_en_posicion(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar, int posicion);
void* tratar_fragmentacion_interna(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar);
void* crear_fragmentacion_interna(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar, uint32_t tamanio_total);
int encontrar_mensaje_por_posicion(int posicion, t_list* lista);
t_particion_buddy* crear_nueva_particion(int comienzo, int tamanio, int final);

// DUMP
void actualizar_dump_cache(void);
void llenar_el_dump_para_particiones(FILE* dump);
void llenar_el_dump_para_buddy_system(FILE* dump);
void llenar_inicio_dump(FILE* dump);
int si_el_anterior_esta_vacio(t_list* lista_ordenada, FILE* dump, int index, int contador_de_particiones);
char* crear_linea_a_agregar_ocupada(int inicio, int final, int tamanio, int lru, int cola, int id);
char* crear_linea_a_agregar_vacia(int inicio, int final, int tamanio);
char* obtener_fecha(void);

void handler(int senial);

#endif /* UTILS_MEMORIA_H_ */
