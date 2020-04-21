/*
 * conexiones.c
 *
 *  Created on: 2 mar. 2019
 *      Author: utnso
 */

#include "messageQueue.h"

//TODO
/*
 * Recibe un paquete a serializar, y un puntero a un int en el que dejar
 * el tamaño del stream de bytes serializados que devuelve
 */
void* serializar_paquete(t_paquete* paquete, int *bytes)
{
	*bytes = (paquete->buffer->size)+sizeof(op_code)+sizeof(int);
	void* a_enviar = malloc((*bytes));
	int offset = 0;

	memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(op_code));
	offset += sizeof(op_code);

	memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(int));
	offset +=sizeof(int);

	memcpy(a_enviar + offset, paquete->buffer->stream, paquete->buffer->size);

	return a_enviar;
}

int crear_conexion(char* ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
		printf("error");

	freeaddrinfo(server_info);

	return socket_cliente;
}

//TODO

t_paquete* inicializar_paquete(char* mensaje){

	t_paquete* paquete = malloc(sizeof(paquete));

	paquete->codigo_operacion = MENSAJE;

	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = strlen(mensaje)+1;
	buffer->stream = mensaje;

	paquete -> buffer = buffer;
	return paquete;

}

void enviar_mensaje(char* mensaje, int socket_cliente)
{
	t_paquete* paquete = inicializar_paquete(mensaje);
	fflush(stdout);

	int tamanio_paquete = 0;

	void* a_enviar = serializar_paquete(paquete,&tamanio_paquete);

	send(socket_cliente,a_enviar,tamanio_paquete,0);
}

char* recibir_mensaje(int socket_cliente)
{
	char* mensaje;
	void* stream;
	int size;
	int codigo;

	recv(socket_cliente,&codigo,sizeof(op_code),0);
	recv(socket_cliente,&size,sizeof(int),0);

	switch(codigo){
	case 1:

	stream = malloc(size);
	recv(socket_cliente,stream,size,0);

	//mensaje = malloc(size);
	memcpy(mensaje,stream,size);

	break;

	}

	free(stream);

	return mensaje;

}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
