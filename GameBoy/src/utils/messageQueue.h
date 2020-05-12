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

typedef enum
{
	NEW_POKEMON=1,
	APPEARED_POKEMON=2,
	CATCH_POKEMON=3,
	CAUGHT_POKEMON=4,
	GET_POKEMON=5,
	LOCALIZED_POKEMON=6
}op_code;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

int crear_conexion(char* ip, char* puerto);
void enviar_mensaje(int socket_cliente, op_code codigo_operacion);
void recibir_mensaje(int socket_cliente);
void eliminar_paquete(t_paquete* paquete);
void liberar_conexion(int socket_cliente);
t_paquete* inicializar_paquete(op_code codigo_operacion, t_list* argumentos);
void* serializar_paquete(t_paquete* paquete , int *bytes);
void* iniciar_paquete_serializado(int* tamanio_paquete);

#endif /* MSGQ_H_ */
