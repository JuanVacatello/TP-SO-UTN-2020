#include "../Utils/messageQueue.h"

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

t_paquete* inicializar_paquete(op_code codigo_operacion, t_list* argumentos){

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = codigo_operacion;

	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = 0;
	void* stream = malloc(buffer->size);

	switch(codigo_operacion){
	case 1:

		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	}

	buffer->stream = argumentos;

	paquete->buffer = buffer;

	return paquete;
}

void enviar_mensaje(int socket_cliente, op_code codigo_operacion, char* argv[])
{
	int tamanio_paquete = 0;
	void* a_enviar;

	switch(codigo_operacion){
	case 1:
		a_enviar = iniciar_paquete_serializado_NewPokemon(&tamanio_paquete,argv);


		break;
	}

	send(socket_cliente,a_enviar,tamanio_paquete,0);
	free(a_enviar);

//fflush(stdout);
}


void recibir_mensaje(int socket_cliente)
{
	int caracteresPokemon, posX, posY, cantidad;
	char* pokemon;

	recv(socket_cliente,&caracteresPokemon ,sizeof(int),0);
	recv(socket_cliente,&pokemon ,caracteresPokemon,0);
	recv(socket_cliente,&posX ,sizeof(int),0);
	recv(socket_cliente,&posY ,sizeof(int),0);
	recv(socket_cliente,&cantidad ,sizeof(int),0);

	printf("%d /n",caracteresPokemon);
	printf("%d /n",posX);
	printf("%d /n",posY);
	printf("%d /n",cantidad);
	puts(pokemon);

} //GAMEBOY NO RECIBE MENSAJES*/

void* iniciar_paquete_serializado_NewPokemon(int* tamanio_paquete,char* argv[]){

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = NEW_POKEMON;

	paquete->buffer = malloc(sizeof(t_buffer));

	char* pokemon = argv[2];
	int caracteresPokemon = strlen(pokemon) + 1;
	int posX;
	sscanf(argv[3], "%d",&posX);
	int posY ;
	sscanf(argv[4], "%d",&posY);
	int cantidad_pokemon;
	sscanf(argv[5], "%d",&cantidad_pokemon);
						//INT CARACTERES + POKEMON + POSX + POSY + CANTIDAD
	paquete->buffer->size =sizeof(int) + caracteresPokemon +sizeof(int)+sizeof(int)+sizeof(int);
	void* stream = malloc(paquete->buffer->size);
	int offset = 0;

		memcpy(stream + offset, &caracteresPokemon, sizeof(int));
		offset += sizeof(int);

		memcpy(stream + offset, &pokemon, caracteresPokemon);
		offset +=caracteresPokemon;

		memcpy(stream + offset, &posX, sizeof(int));
		offset += sizeof(int);

		memcpy(stream + offset, &posY, sizeof(int));
		offset += sizeof(int);

		memcpy(stream + offset, &cantidad_pokemon, sizeof(int));
		offset += sizeof(int);

		paquete->buffer->stream = stream;

						// TAMAÑO STREAM + OP CODE + VARIABLE SIZE
	*tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(int);

	void* a_enviar = malloc((*tamanio_paquete));
	int offsetDeSerializacion = 0;

		memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(op_code));
		offsetDeSerializacion += sizeof(op_code);

		memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(int));
		offsetDeSerializacion +=sizeof(int);

		memcpy(a_enviar + offset, &(paquete->buffer->stream), paquete->buffer->size);

		free(stream);
		free(paquete->buffer);
		free(paquete);
	return a_enviar;

}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
