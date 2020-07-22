
#ifndef UTILS_CONEXION_H_
#define UTILS_CONEXION_H_


#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/collections/list.h>
#include<stdint.h>
#include<pthread.h>

#include"configGameCard.h"
#include"logGameCard.h"


#define IP "127.0.0.4"
#define PUERTO "7777"

typedef enum{
	SUSCRIPTOR=0,
	NEW_POKEMON=1,
	APPEARED_POKEMON=2,
	CATCH_POKEMON=3,
	CAUGHT_POKEMON=4,
	GET_POKEMON=5,
	LOCALIZED_POKEMON=6,
	MENSAJE=7
}op_code;

typedef struct{
	uint32_t size;
	void* stream;
} t_buffer;

typedef struct{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

pthread_t thread;

int crear_conexion(char* ip, char* puerto);

void iniciar_espera_mensajes_Gameboy(void);
void esperar_cliente(int socket_servidor);
void serve_client(int* socket);
void process_request(op_code cod_op, int socket_cliente);

void* serializar_paquete(t_paquete* paquete, int *bytes);
void enviar_mensaje_a_broker(int socket_cliente, op_code codigo_operacion, char* argv[]);
int suscribirse_globalmente(op_code cola_a_suscribirse);
void* suscribirse_a_cola(int socket_broker, uint32_t cola_a_suscribirse, int* tamanio_paquete);

void iniciar_espera_mensajes(void);
void esperar_cliente(int socket_servidor);
void serve_client(int* socket_cliente);
void process_request(op_code cod_op, int socket_cliente);

void new_pokemon_broker();
void catch_pokemon_broker();
void get_pokemon_broker();

void recibir_new_pokemon(int socket_cliente);
void recibir_catch_pokemon(int socket_cliente);
void recibir_get_pokemon(int socket_cliente);

void* enviar_ACK(int socket_broker, int* tamanio);
void responder_ack();
char* recibir_mensaje(int socket_broker);

#endif /* UTILS_CONEXION_H_ */
