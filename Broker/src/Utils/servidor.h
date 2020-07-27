#ifndef SERVIDOR_H_
#define SERVIDOR_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<commons/string.h>
#include<string.h>
#include<pthread.h>
#include"memoria.h"
#include"logBroker.h"

#define IP "127.0.0.1"
#define PUERTO "4444"

typedef struct
{
	int id;
	int socket_cliente;
} proceso;

typedef struct
{
	uint32_t size;
	void* stream;
} t_buffer;

typedef struct t_paquete
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

typedef struct
{
	op_code cola;
	int socket_cliente;
	int tiempo;
} t_suscripcion;

t_buffer* buffer;
pthread_t thread;

// CONEXION CON CLIENTE
void iniciar_servidor(void);
void esperar_cliente(int);
void serve_client(int *socket);

// ATENDER AL CLIENTE
void process_request(op_code cod_op, int socket_cliente);

// ATENDER SUSCRIPCION
void atender_suscripcion(int socket_cliente);
proceso* modelar_proceso(int socket);
void suscribirse_a_cola(proceso* suscriptor, int socket, uint32_t tamanio_buffer);
void enviar_mensajes_al_nuevo_suscriptor_NP(t_list* mensajes_de_dicha_cola, int socket_suscriptor);
void enviar_mensajes_al_nuevo_suscriptor_AP(t_list* mensajes_de_dicha_cola, int socket_suscriptor);
void enviar_mensajes_al_nuevo_suscriptor_CATP(t_list* mensajes_de_dicha_cola, int socket_suscriptor);
void enviar_mensajes_al_nuevo_suscriptor_CAUP(t_list* mensajes_de_dicha_cola, int socket_suscriptor);
void enviar_mensajes_al_nuevo_suscriptor_GP(t_list* mensajes_de_dicha_cola, int socket_suscriptor);
void enviar_mensajes_al_nuevo_suscriptor_LP(t_list* mensajes_de_dicha_cola, int socket_suscriptor);
void correr_tiempo_suscripcion(t_suscripcion* suscripcion);
int encontrar_suscriptor_por_posicion(int socket_cliente, t_list* lista);

// RECEPCION, ALMACENAMIENTO EN MEMORIA Y REENVIO DE MENSAJES
void recibir_new_pokemon(int socket_cliente);
void recibir_appeared_pokemon(int socket_cliente);
void recibir_catch_pokemon(int socket_cliente);
void recibir_caught_pokemon(int socket_cliente);
void recibir_get_pokemon(int socket_cliente);
void recibir_localized_pokemon(int socket_cliente);
void reenviar_mensaje_a_suscriptores(void* a_enviar, int tamanio_paquete, t_list* suscriptores, int cola);
void guardar_mensaje_en_cola(t_list* lista_mensajes, t_mensaje_guardado* mensaje_en_memoria, uint32_t tamanio_buffer, uint32_t id_mensaje_correlativo, char* pokemon, t_list* lista_de_suscriptores);

void recibir_ack(int socket_cliente);
void eliminar_suscriptor_que_ya_ack(uint32_t mensaje_id_recibido, uint32_t proceso_id);

// AUXILIAR
void* serializar_paquete(t_paquete* paquete, int bytes);
int recibir_tamanio_buffer(int socket);

// ENVIAR MENSAJE
void enviar_mensaje(int socket_cliente, char* mensaje);
void enviar_id_mensaje(uint32_t id_del_mensaje, int socket_cliente);

pthread_t hilo_suscripcion;
pthread_t hilo_newPokemon;
pthread_t hilo_appearedPokemon;
pthread_t hilo_catchPokemon;
pthread_t hilo_caughtPokemon;
pthread_t hilo_getPokemon;
pthread_t hilo_localizedPokemon;
//pthread_t hilo_ack;

pthread_mutex_t mutex_suscripcion;

#endif

