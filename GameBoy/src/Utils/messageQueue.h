#ifndef MSGQ_H_
#define MSGQ_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/collections/list.h>
#include<stdint.h>

#include"logGameBoy.h"

typedef enum
{
	SUSCRIBIRSE=0,
	NEW_POKEMON=1,
	APPEARED_POKEMON=2,
	CATCH_POKEMON=3,
	CAUGHT_POKEMON=4,
	GET_POKEMON=5,
	LOCALIZED_POKEMON=6
}op_code;

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

int crear_conexion(char* ip, char* puerto);
void recibir_mensaje(int socket_cliente);
void eliminar_paquete(t_paquete* paquete);
t_paquete* inicializar_paquete(op_code codigo_operacion, t_list* argumentos);
void* serializar_paquete(t_paquete* paquete , int *bytes);

void enviar_mensaje_a_broker(int socket_cliente, op_code codigo_operacion, char* argv[]);
void* iniciar_paquete_serializado_NewPokemon(int* tamanio_paquete, char* argv[]);
void* iniciar_paquete_serializado_AppearedPokemon(int* tamanio_paquete,char* argv[]);
void* iniciar_paquete_serializado_CatchPokemon(int* tamanio_paquete,char* argv[]);
void* iniciar_paquete_serializado_CaughtPokemon(int* tamanio_paquete,char* argv[]);
void* iniciar_paquete_serializado_GetPokemon(int* tamanio_paquete,char* argv[]);

void enviar_mensaje_a_team(int socket_cliente, op_code codigo_operacion, char* argv[]);
void* iniciar_paquete_serializado_AppearedPokemonTeam(int* tamanio_paquete,char* argv[]);

void enviar_mensaje_a_gamecard(int socket_cliente, op_code codigo_operacion, char* argv[]);
void* iniciar_paquete_serializado_NewPokemonGC(int* tamanio_paquete,char* argv[]);
void* iniciar_paquete_serializado_CatchPokemonGC(int* tamanio_paquete,char* argv[]);
void* iniciar_paquete_serializado_GetPokemonGC(int* tamanio_paquete,char* argv[]);

void* iniciar_paquete_prueba(int* tamanio_paquete);

void liberar_conexion(int socket_cliente);

#endif /* MSGQ_H_ */
