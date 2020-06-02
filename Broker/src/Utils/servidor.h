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
	SUSCRIBIRSE=0,
	NEW_POKEMON=1,
	APPEARED_POKEMON=2,
	CATCH_POKEMON=3,
	CAUGHT_POKEMON=4,
	GET_POKEMON=5,
	LOCALIZED_POKEMON=6,
	PRUEBA = 7
} op_code;

typedef struct
{
	int id;
	int socket_cliente;
}proceso;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct t_paquete
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

typedef struct
{
	t_list* suscriptores;// = list_create(); // lista de procesos. t_list de las commons
	t_paquete mensajes[]; // LOS MENSAJES LLEVAN ID E ID CORRELATIVO POR LO QUE NO SERAN
						  //UNA LISTA T_PAQUETE

} cola_mensaje;

t_buffer* buffer;

pthread_t thread;



void* recibir_buffer(int*, int);

void iniciar_servidor(void);
void esperar_cliente(int);
void* recibir_mensaje(int socket_cliente, int* size);
int recibir_operacion(int);
void process_request(op_code cod_op, int socket_cliente);
void serve_client(int *socket);
void* serializar_paquete(t_paquete* paquete, int bytes);
void devolver_mensaje(void* payload, int size, int socket_cliente);
void atenderSuscripcion(int socket_cliente);
void suscribirseACola(proceso* suscriptor,cola_mensaje cola_mensaje );
proceso* modelarProceso(int socket);
void suscribirseAColas(proceso* suscriptor, int socket);
void recibir_new_pokemon(int socket_cliente);

void atenderMensajePrueba(int socket_cliente);



#endif /* CONEXIONES_H_ */
