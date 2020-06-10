#ifndef CONEXIONES_H_
#define CONEXIONES_H_

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
	LOCALIZED_POKEMON=6
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

/*typedef struct
{
	t_list* suscriptores;// = list_create(); // lista de procesos. t_list de las commons
	t_paquete mensajes[]; // LOS MENSAJES LLEVAN ID E ID CORRELATIVO POR LO QUE NO SERAN UNA LISTA T_PAQUETE

} colaDeMensajes;*/

t_list* suscriptores_new_pokemon;
t_list* suscriptores_appeared_pokemon;
t_list* suscriptores_catch_pokemon;
t_list* suscriptores_caught_pokemon;
t_list* suscriptores_get_pokemon;
t_list* suscriptores_localized_pokemon;

t_buffer* buffer;

pthread_t thread;

void serve_client(int *socket);
void process_request(op_code cod_op, int socket_cliente);
void* recibir_caught_pokemon(int socket_cliente, int* tamanio_paquete);

void atender_suscripcion(int socket_cliente);
proceso* modelar_proceso(int socket);
int recibir_tamanio_buffer(int socket);
void suscribirse_a_cola(proceso* suscriptor, int socket, uint32_t tamanio_buffer);

void recibir_new_pokemon(int socket_cliente);

void* recibir_buffer(int*, int);


void iniciar_servidor(void);
void esperar_cliente(int);
void* recibir_mensaje(int socket_cliente, int* size);
int recibir_operacion(int);
void* serializar_paquete(t_paquete* paquete, int bytes);
void devolver_mensaje(void* payload, int size, int socket_cliente);

/* Para probar:

void recibir_caught_pokemon_loggeo(int socket_cliente);

 */

#endif /* CONEXIONES_H_ */
