#ifndef CONEXIONES_H_
#define CONEXIONES_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<commons/collections/dictionary.h>
#include<string.h>
#include<pthread.h>
#include "entrenador.h"
#include "Team.h"
#include "utils.h"


typedef struct
{
	uint32_t size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

pthread_t thread;

void* recibir_buffer(int*, int);

int crear_conexion(char* ip, char* puerto);
void iniciar_servidor(void);
void esperar_cliente(int);
void* recibir_mensaje(int socket_cliente, int* size);
int recibir_operacion(int);
void process_request(int cod_op, int cliente_fd);
void serve_client(int *socket);
void* serializar_paquete(t_paquete* paquete, int* bytes);
void devolver_mensaje(void* payload, int size, int socket_cliente);

void* suscribirse_a_cola(int socket_cliente, uint32_t cola, int* tamanio_paquete);
void enviar_suscripcion_a_cola(int socket_cliente, uint32_t cola);

void enviar_CatchPokemon_a_broker(int socket_cliente, op_code codigo_operacion, t_entrenador* entrenador);
void enviar_GetPokemon_a_broker(int socket_cliente, op_code codigo_operacion, t_pokemon* pokemon);

void* iniciar_paquete_serializado_CatchPokemon(int* tamanio_paquete,t_entrenador* entrenador);
void* iniciar_paquete_serializado_GetPokemon(int* tamanio_paquete,t_pokemon* pokemon_pedido);
void recibir_mensaje2(int socket_cliente);




#endif
