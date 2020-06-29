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

/*
typedef struct t_paquete_devuelto
{
	op_code codigo_operacion;
	t_buffer* buffer;
	uint32_t identificador;
} t_paquete_devuelto;
*/

typedef struct {
	uint32_t identificador;
	op_code codigo_operacion;
	t_list* suscriptores;
	t_list* suscriptores_ack;
	t_mensaje_guardado* contenido_mensaje;
} t_mensaje_a_guardar;

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

t_buffer* buffer;
uint32_t mensaje_id;
pthread_t thread;

void iniciar_servidor(void);
void esperar_cliente(int);
void serve_client(int *socket);
void process_request(op_code cod_op, int socket_cliente);

void atender_suscripcion(int socket_cliente);
proceso* modelar_proceso(int socket);
int recibir_tamanio_buffer(int socket);
void suscribirse_a_cola(proceso* suscriptor, int socket, uint32_t tamanio_buffer);

void enviar_mensaje(int socket_cliente, char* mensaje);
void enviar_mensaje_id(int socket_cliente);

void recibir_new_pokemon(int socket_cliente);
void recibir_appeared_pokemon(int socket_cliente);
void recibir_catch_pokemon(int socket_cliente);
void recibir_caught_pokemon(int socket_cliente);
void recibir_get_pokemon(int socket_cliente);
void recibir_localized_pokemon(int socket_cliente);
void reenviar_mensaje_a_suscriptores(void* a_enviar, int tamanio_paquete, t_list* suscriptores);
void guardar_mensaje_en_cola(op_code cod_op, void* contenido, t_list* lista_mensajes, t_mensaje_guardado* mensaje_nuevo);
//void enviar_mensaje_a_suscriptores(int cola_mensaje,int socket_cliente);

void* recibir_mensaje(int socket_cliente, int* size);
void guardar_mensaje(t_list* cola_de_mensajes, op_code codigo_operacion, t_list* suscriptores);
void recibir_ack(int socket_cliente);

int recibir_operacion(int);
void* serializar_paquete(t_paquete* paquete, int bytes);
void devolver_mensaje(void* payload, int size, int socket_cliente);

#endif

