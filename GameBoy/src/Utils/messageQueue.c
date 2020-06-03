#include "../Utils/messageQueue.h"

int crear_conexion(char* ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	////
	completar_logger("Paso 1 conectar","GAMEBOY",LOG_LEVEL_INFO);
	////

	int socket_cliente;

	if((socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol)) == -1){
		printf("Error en crear socket.");
		exit(1);
	}

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1){
		printf("Error en conectar socket.");
		exit(2);
	}

	////
	completar_logger("paso 2 conectar","GAMEBOY",LOG_LEVEL_INFO);
	////

	freeaddrinfo(server_info);

	/////
	completar_logger("paso 3 conectar","GAMEBOY",LOG_LEVEL_INFO);
	////

	return socket_cliente;
}

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

// ENVIO DE MENSAJES

// MENSAJES A BROKER

void enviar_mensaje_a_broker(int socket_cliente, op_code codigo_operacion, char* argv[])
{
	int tamanio_paquete = 0;
	void* a_enviar;

	switch(codigo_operacion){
	case 1:
		a_enviar = iniciar_paquete_serializado_NewPokemon(&tamanio_paquete,argv);
		break;
	case 2:
		a_enviar = iniciar_paquete_serializado_AppearedPokemon(&tamanio_paquete,argv);
		break;
	case 3:
		a_enviar = iniciar_paquete_serializado_CatchPokemon(&tamanio_paquete,argv);
		break;
	case 4:
		a_enviar = iniciar_paquete_serializado_CaughtPokemon(&tamanio_paquete,argv);
		break;
	case 5:
		a_enviar = iniciar_paquete_serializado_GetPokemon(&tamanio_paquete,argv);
		break;
	case 6:
		break;
	}

	send(socket_cliente,a_enviar,tamanio_paquete,0);
	free(a_enviar);

//fflush(stdout);
}

// BROKER - NEW POKEMON

void* iniciar_paquete_serializado_NewPokemon(int* tamanio_paquete,char* argv[]){

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = NEW_POKEMON;

	paquete->buffer = malloc(sizeof(t_buffer));

	char* pokemon = argv[3];
	int caracteresPokemon = strlen(pokemon) + 1;
	int posX;
	sscanf(argv[4], "%d",&posX);
	int posY ;
	sscanf(argv[5], "%d",&posY);
	int cantidad_pokemon;
	sscanf(argv[6], "%d",&cantidad_pokemon);

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

	void* a_enviar = serializar_paquete(paquete, &tamanio_paquete); // y me ahorro todo esto:

	/*
						// TAMAÑO STREAM + OP CODE + VARIABLE SIZE
	*tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(int);
	void* a_enviar = malloc((*tamanio_paquete));

	int offsetDeSerializacion = 0;

		memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(op_code));
		offsetDeSerializacion += sizeof(op_code);

		memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(int));
		offsetDeSerializacion +=sizeof(int);

		memcpy(a_enviar + offset, &(paquete->buffer->stream), paquete->buffer->size);

	*/

	free(stream);
	free(paquete->buffer);
	free(paquete);

	return a_enviar;

}

// BROKER - APPEARED POKEMON

void* iniciar_paquete_serializado_AppearedPokemon(int* tamanio_paquete,char* argv[]){

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = APPEARED_POKEMON;

	paquete->buffer = malloc(sizeof(t_buffer));

	char* pokemon = argv[3];
	int caracteresPokemon = strlen(pokemon) + 1;
	int posX;
	sscanf(argv[4], "%d",&posX);
	int posY ;
	sscanf(argv[5], "%d",&posY);
	int id_mensaje_correlativo;
	sscanf(argv[6], "%d",&id_mensaje_correlativo); //el id lo definimos nosotros y debe ser único

						//INT CARACTERES + POKEMON + POSX + POSY + ID_MENSAJE_CORRELATIVO
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

		memcpy(stream + offset, &id_mensaje_correlativo, sizeof(int));
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

// BROKER - CATCH POKEMON

void* iniciar_paquete_serializado_CatchPokemon(int* tamanio_paquete,char* argv[]){

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = CATCH_POKEMON;

	paquete->buffer = malloc(sizeof(t_buffer));

	char* pokemon = argv[3];
	int caracteresPokemon = strlen(pokemon) + 1;
	int posX;
	sscanf(argv[4], "%d",&posX);
	int posY ;
	sscanf(argv[5], "%d",&posY);

						//INT CARACTERES + POKEMON + POSX + POSY
	paquete->buffer->size = sizeof(int) + caracteresPokemon + sizeof(int) + sizeof(int);
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

// BROKER - CAUGHT POKEMON

void* iniciar_paquete_serializado_CaughtPokemon(int* tamanio_paquete,char* argv[]){

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = CAUGHT_POKEMON;

	paquete->buffer = malloc(sizeof(t_buffer));

	int id_mensaje_correlativo;
	sscanf(argv[3], "%d", &id_mensaje_correlativo);

	int se_pudo_atrapar; // devuelve 1 o 0 dependiendo de si se pudo o no -> lo mandamos asi o como true/false string?
	sscanf(argv[4], "%d", &se_pudo_atrapar);

						// ID_MENSAJE_CORRELATIVO + OK/FAIL
	paquete->buffer->size = sizeof(int) + sizeof(int);
	void* stream = malloc(paquete->buffer->size);
	int offset = 0;

		memcpy(stream + offset, &id_mensaje_correlativo, sizeof(int));
		offset += sizeof(int);

		memcpy(stream + offset, &se_pudo_atrapar, sizeof(int));
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

// BROKER - GET POKEMON

void* iniciar_paquete_serializado_GetPokemon(int* tamanio_paquete,char* argv[]){

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = GET_POKEMON;

	paquete->buffer = malloc(sizeof(t_buffer));

	char* pokemon = argv[3];
	int caracteresPokemon = strlen(pokemon) + 1;

						//INT CARACTERES + POKEMON
	paquete->buffer->size = sizeof(int) + caracteresPokemon;
	void* stream = malloc(paquete->buffer->size);
	int offset = 0;

		memcpy(stream + offset, &caracteresPokemon, sizeof(int));
		offset += sizeof(int);

		memcpy(stream + offset, &pokemon, caracteresPokemon);
		offset +=caracteresPokemon;

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

// MENSAJE A TEAM

void enviar_mensaje_a_team(int socket_cliente, op_code codigo_operacion, char* argv[])
{
	int tamanio_paquete = 0;
	void* a_enviar;

	// no hace falta switch porque solo envia 1 tipo de mensaje

	a_enviar = iniciar_paquete_serializado_AppearedPokemonTeam(&tamanio_paquete,argv);

	send(socket_cliente,a_enviar,tamanio_paquete,0);
	free(a_enviar);

//fflush(stdout);
}

// TEAM - APPEARED POKEMON

void* iniciar_paquete_serializado_AppearedPokemonTeam(int* tamanio_paquete,char* argv[]){

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = APPEARED_POKEMON;

	paquete->buffer = malloc(sizeof(t_buffer));

	char* pokemon = argv[3];
	int caracteresPokemon = strlen(pokemon) + 1;
	int posX;
	sscanf(argv[4], "%d",&posX);
	int posY ;
	sscanf(argv[5], "%d",&posY);

							//INT CARACTERES + POKEMON + POSX + POSY
	paquete->buffer->size = sizeof(int) + caracteresPokemon +sizeof(int)+sizeof(int);
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

// MENSAJES A GAMECARD

void enviar_mensaje_a_gamecard(int socket_cliente, op_code codigo_operacion, char* argv[])
{
	int tamanio_paquete = 0;
	void* a_enviar;

	switch(codigo_operacion){
		case 1:
			a_enviar = iniciar_paquete_serializado_NewPokemonGC(&tamanio_paquete,argv);
			break;
		case 3:
			a_enviar = iniciar_paquete_serializado_CatchPokemonGC(&tamanio_paquete,argv);
			break;
		case 5:
			a_enviar = iniciar_paquete_serializado_GetPokemonGC(&tamanio_paquete,argv);
			break;
		case 2:
			break;
		case 4:
			break;
		case 6:
			break;
		}


	send(socket_cliente,a_enviar,tamanio_paquete,0);
	free(a_enviar);

//fflush(stdout);
}

// GAMECARD - NEW POKEMON

void* iniciar_paquete_serializado_NewPokemonGC(int* tamanio_paquete,char* argv[]){

	t_paquete* paquete = malloc(sizeof(t_paquete));

		paquete->codigo_operacion = NEW_POKEMON;

		paquete->buffer = malloc(sizeof(t_buffer));

		char* pokemon = argv[3];
		int caracteresPokemon = strlen(pokemon) + 1;
		int posX;
		sscanf(argv[4], "%d",&posX);
		int posY;
		sscanf(argv[5], "%d",&posY);
		int cantidad_pokemon;
		sscanf(argv[6], "%d",&cantidad_pokemon);
		int id_mensaje;
		sscanf(argv[7], "%d",&id_mensaje);

							//INT CARACTERES + POKEMON + POSX + POSY + CANTIDAD + ID MENSAJE
		paquete->buffer->size =sizeof(int) + caracteresPokemon +sizeof(int)+sizeof(int)+sizeof(int)+sizeof(int);
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

			memcpy(stream + offset, &id_mensaje, sizeof(int));
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

// GAMECARD - CATCH POKEMON

void* iniciar_paquete_serializado_CatchPokemonGC(int* tamanio_paquete,char* argv[]){

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = CATCH_POKEMON;

	paquete->buffer = malloc(sizeof(t_buffer));

	char* pokemon = argv[3];
	int caracteresPokemon = strlen(pokemon) + 1;
	int posX;
	sscanf(argv[4], "%d",&posX);
	int posY ;
	sscanf(argv[5], "%d",&posY);
	int id_mensaje;
	sscanf(argv[6], "%d",&id_mensaje);

						//INT CARACTERES + POKEMON + POSX + POSY + ID MENSAJE
	paquete->buffer->size = sizeof(int) + caracteresPokemon + sizeof(int) + sizeof(int) + sizeof(int);
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

		memcpy(stream + offset, &id_mensaje, sizeof(int));
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

// GAMECARD - GET POKEMON

void* iniciar_paquete_serializado_GetPokemonGC(int* tamanio_paquete,char* argv[]){

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = GET_POKEMON;

	paquete->buffer = malloc(sizeof(t_buffer));

	char* pokemon = argv[3];
	int caracteresPokemon = strlen(pokemon) + 1;
	int id_mensaje;
	sscanf(argv[4], "%d",&id_mensaje);

						//INT CARACTERES + POKEMON + ID MENSAJE
	paquete->buffer->size = sizeof(int) + caracteresPokemon + sizeof(int);
	void* stream = malloc(paquete->buffer->size);
	int offset = 0;

		memcpy(stream + offset, &caracteresPokemon, sizeof(int));
		offset += sizeof(int);

		memcpy(stream + offset, &pokemon, caracteresPokemon);
		offset +=caracteresPokemon;

		memcpy(stream + offset, &id_mensaje, sizeof(int));
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

// Paquete de prueba

void* iniciar_paquete_prueba(int* tamanio_paquete){

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = PRUEBA;

	paquete->buffer = malloc(sizeof(t_buffer));

	int numero = 2;

	paquete->buffer->size = sizeof(int);

	void* stream = malloc(paquete->buffer->size);
	int offset = 0;

		memcpy(stream + offset, &numero, sizeof(int));
		//offset += sizeof(int);

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

// El proceso GameBoy no recibe mensajes

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

}


