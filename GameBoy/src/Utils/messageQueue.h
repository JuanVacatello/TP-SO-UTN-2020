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
#include<semaphore.h>
#include"configGameBoy.h"
#include"logGameBoy.h"

#define IP "127.0.0.3"
#define PUERTO "6666"

pthread_t thread;

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

typedef struct t_paquete_devuelto
{
	op_code codigo_operacion;
	uint32_t identificador;
	t_buffer* buffer;
} t_paquete_devuelto;

// Conexion
int crear_conexion(char* ip, char* puerto);
void iniciar_servidor(void);
void esperar_cliente(int socket_servidor);
void serve_client(int* socket_cliente);
void process_request(op_code cod_op, int socket_cliente);

// Envio de mensajes a Broker
void enviar_mensaje_a_broker(int socket_cliente, op_code codigo_operacion, char* argv[]);
void* suscribirse_a_cola(int* tamanio_paquete,char* argv[]);
uint32_t cola_a_suscribirse(char* cola_leida);
void* iniciar_paquete_serializado_NewPokemon(int* tamanio_paquete, char* argv[]);
void* iniciar_paquete_serializado_AppearedPokemon(int* tamanio_paquete,char* argv[]);
void* iniciar_paquete_serializado_CatchPokemon(int* tamanio_paquete,char* argv[]);
void* iniciar_paquete_serializado_CaughtPokemon(int* tamanio_paquete,char* argv[]);
void* iniciar_paquete_serializado_GetPokemon(int* tamanio_paquete,char* argv[]);

// Envio de mensajes a Team
void enviar_mensaje_a_team(int socket_cliente, op_code codigo_operacion, char* argv[]);
void* iniciar_paquete_serializado_AppearedPokemonTeam(int* tamanio_paquete,char* argv[]);

// Envio de mensajes a GameCard
void enviar_mensaje_a_gamecard(int socket_cliente, op_code codigo_operacion, char* argv[]);
void* iniciar_paquete_serializado_NewPokemonGC(int* tamanio_paquete,char* argv[]);
void* iniciar_paquete_serializado_CatchPokemonGC(int* tamanio_paquete,char* argv[]);
void* iniciar_paquete_serializado_GetPokemonGC(int* tamanio_paquete,char* argv[]);

// Recepcion de mensajes
uint32_t recibir_mensaje(int socket_cliente);
uint32_t recibir_new_pokemon(int socket_cliente);
uint32_t recibir_appeared_pokemon(int socket_cliente);
uint32_t recibir_catch_pokemon(int socket_cliente);
uint32_t recibir_caught_pokemon(int socket_cliente);
uint32_t recibir_get_pokemon(int socket_cliente);

// Auxiliares
void* serializar_paquete(t_paquete* paquete , int *bytes);
void enviar_ACK(int socket_broker, char* mensaje, uint32_t id_mensaje);
void liberar_conexion(int socket_cliente);

#endif /* MSGQ_H_ */
