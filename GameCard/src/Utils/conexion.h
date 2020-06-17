/*
 * conexion.h
 *
 *  Created on: 17 jun. 2020
 *      Author: utnso
 */




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

int crear_conexion(char* ip, char* puerto);
void* serializar_paquete(t_paquete* paquete, int *bytes);
void enviar_mensaje_a_broker(int socket_cliente, op_code codigo_operacion, char* argv[]);
void* suscribirse_a_cola(int* tamanio_paquete,char* argv[]);




#endif /* UTILS_CONEXION_H_ */
