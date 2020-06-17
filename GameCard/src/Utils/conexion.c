/*
 * conexion.c
 *
 *  Created on: 17 jun. 2020
 *      Author: utnso
 */

#include "conexion.h"




int crear_conexion(char* ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);
	int socket_cliente;

	if((socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol)) == -1){
		printf("Error en crear socket.");
		exit(1);
	}

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1){
		printf("Error en conectar socket.");
		exit(2);
	}

	freeaddrinfo(server_info);
	return socket_cliente;
}

// SERIALIZAR PAQUETE

void* serializar_paquete(t_paquete* paquete, int *bytes)
{
	void* a_enviar = malloc((*bytes));
	int offset = 0;

	memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(op_code));
	offset += sizeof(op_code);

	memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(int));
	offset +=sizeof(int);

	memcpy(a_enviar + offset, paquete->buffer->stream, paquete->buffer->size);

	return a_enviar;
}

// ENVIO DE MENSAJES

// MENSAJES A BROKER

/*

void enviar_mensaje_a_broker(int socket_cliente, op_code codigo_operacion, char* argv[])
{
	int tamanio_paquete = 0;
	void* a_enviar;

	switch(codigo_operacion){
	case 0:
		a_enviar = suscribirse_a_cola(&tamanio_paquete,argv);
		break;
	case 1:
		//a_enviar = iniciar_paquete_serializado_CatchPokemon(&tamanio_paquete,argv);
		break;
	case 2:
		a_enviar = iniciar_paquete_serializado_CaughtPokemon(&tamanio_paquete,argv);
		break;
	case 3:
		break;
	case 4:
		break;
	}

	if(send(socket_cliente,a_enviar,tamanio_paquete,0) == -1){
		printf("Error en enviar por el socket");
		exit(3);
	}

	free(a_enviar);

//fflush(stdout);
}
*/

// BROKER - SUSCRIPTOR

void* suscribirse_a_cola(int* tamanio_paquete,char* argv[]){

	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = SUSCRIPTOR;
	paquete->buffer = malloc(sizeof(t_buffer));

	uint32_t proccess_id = obtener_id_propio(); //getpid();
	uint32_t cola;
	sscanf(argv[2], "%d", &cola);
	uint32_t tiempo_de_suscripcion;
	sscanf(argv[3], "%d", &tiempo_de_suscripcion);

	paquete->buffer->size = sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t);
	void* stream = malloc(paquete->buffer->size);
	int offset = 0;

		memcpy(stream + offset, &proccess_id, sizeof(uint32_t));
		offset += sizeof(uint32_t);

		memcpy(stream + offset, &cola, sizeof(uint32_t));
		offset += sizeof(uint32_t);

		memcpy(stream + offset, &tiempo_de_suscripcion, sizeof(uint32_t));

	paquete->buffer->stream = stream;

						// TAMAÑO STREAM + OP CODE + VARIABLE SIZE
	*tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);
	void* a_enviar = serializar_paquete(paquete, tamanio_paquete);

	free(paquete->buffer);
	free(stream);
	free(paquete);

	return a_enviar;
}
