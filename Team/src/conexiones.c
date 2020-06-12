#include"conexiones.h"
#include"configTeam.h"


void iniciar_servidor(void)
{
	int socket_servidor;

    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(IP, PUERTO, &hints, &servinfo);

    for (p=servinfo; p != NULL; p = p->ai_next)
    {
        if ((socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            continue;

        if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
            close(socket_servidor);
            continue;
        }
        break;
    }

	listen(socket_servidor, SOMAXCONN);

	completar_logger("Se inició proceso Team", "TEAM", LOG_LEVEL_INFO); // LOG OBLIGATORIO

    freeaddrinfo(servinfo);

    while(1)
    	esperar_cliente(socket_servidor);
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

void esperar_cliente(int socket_servidor)
{
	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	pthread_create(&thread,NULL,(void*)serve_client,&socket_cliente);
	pthread_detach(thread);

}

void serve_client(int* socket_cliente)
{
	op_code cod_op;
	if(recv(*socket_cliente, &cod_op, sizeof(op_code), MSG_WAITALL) == -1)
		cod_op = -1;
	process_request(cod_op, *socket_cliente);
}

void process_request(op_code cod_op, int socket_cliente) {

		switch (cod_op) {
		case 2:
			recibir_appeared_pokemon_loggeo(socket_cliente);

			break;
		case 4:

			break;
		case 6:

			break;
		case -1:
			pthread_exit(NULL);
			break;
		}
}

void* recibir_mensaje(int socket_cliente, int* size)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
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

void devolver_mensaje(void* payload, int size, int socket_cliente)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = size;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, payload, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

//---SUSCRIPCION

void* suscribirse_a_cola(int socket_cliente, uint32_t cola, int* tamanio_paquete){
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = SUSCRIPTOR;
	paquete->buffer = malloc(sizeof(t_buffer));

	uint32_t proccess_id = getpid();

	paquete->buffer->size = sizeof(uint32_t)+sizeof(uint32_t);
	void* stream = malloc(paquete->buffer->size);
	int offset = 0;

		memcpy(stream + offset, &proccess_id, sizeof(uint32_t));
		offset += sizeof(uint32_t);

		memcpy(stream + offset, &cola, sizeof(uint32_t));


	paquete->buffer->stream = stream;

						// TAMAÑO STREAM + OP CODE + VARIABLE SIZE
	*tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);
	void* a_enviar = serializar_paquete(paquete, tamanio_paquete);

	free(paquete->buffer);
	free(stream);
	free(paquete);

	return a_enviar;
}


void enviar_suscripcion_a_cola(int socket_cliente, uint32_t cola) //HAY QUE VER ESTE TEMA DEL PARAMETRO
{
	int tamanio_paquete = 0;
	void* a_enviar;
	puts("aca entra1.5");
	a_enviar = suscribirse_a_cola(socket_cliente, cola, &tamanio_paquete);

	send(socket_cliente,a_enviar,tamanio_paquete,0);
	free(a_enviar);
}
//----------------------- COMUNICACION CON BROKER -----------------------

void enviar_CatchPokemon_a_broker(int socket_cliente, op_code codigo_operacion, t_entrenador* entrenador )
{
	int tamanio_paquete = 0;
	void* a_enviar;

	a_enviar = iniciar_paquete_serializado_CatchPokemon(&tamanio_paquete, entrenador);

	if(send(socket_cliente,a_enviar,tamanio_paquete,0) == -1){
		printf("Error en enviar por el socket");
		exit(3);
	}

	free(a_enviar);

}

void enviar_GetPokemon_a_broker(int socket_cliente, op_code codigo_operacion, t_pokemon* pokemon)
{
	int tamanio_paquete = 0;
	void* a_enviar;

	a_enviar = iniciar_paquete_serializado_GetPokemon(&tamanio_paquete, pokemon);

	if(send(socket_cliente,a_enviar,tamanio_paquete,0) == -1){
		printf("Error en enviar por el socket");
		exit(3);
	}

	free(a_enviar);

}


//////////////// CATCH POKEMON ////////////////

void* iniciar_paquete_serializado_CatchPokemon(int* tamanio_paquete,t_entrenador* entrenador){ //Deberia de recibir el entrenador pero no estoy seguro si como parametro o VG.

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = CATCH_POKEMON;

	paquete->buffer = malloc(sizeof(t_buffer));

	char* pokemon = entrenador->pokemon_a_atrapar->especie;
	uint32_t caracteresPokemon = strlen(pokemon) + 1;
	//Quizas es mejor hacer una variable pokemon para no tener tantas flechitas
	uint32_t posX = entrenador->pokemon_a_atrapar->posicion.x;
	uint32_t posY = entrenador->pokemon_a_atrapar->posicion.y;

						//INT CARACTERES + POKEMON + POSX + POSY
	paquete->buffer->size = sizeof(uint32_t) + caracteresPokemon + sizeof(uint32_t) + sizeof(uint32_t);
	void* stream = malloc(paquete->buffer->size);
	int offset = 0;

		memcpy(stream + offset, &caracteresPokemon, sizeof(uint32_t));
		offset += sizeof(uint32_t);

		memcpy(stream + offset, &pokemon, caracteresPokemon);
		offset +=caracteresPokemon;

		memcpy(stream + offset, &posX, sizeof(uint32_t));
		offset += sizeof(uint32_t);

		memcpy(stream + offset, &posY, sizeof(uint32_t));
		offset += sizeof(uint32_t);

		paquete->buffer->stream = stream;

						// TAMAÑO STREAM + OP CODE + VARIABLE SIZE
	*tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);

	void* a_enviar = serializar_paquete(paquete,tamanio_paquete);

	free(stream);
	free(paquete->buffer);
	free(paquete);

	return a_enviar;

}

//////////////// GET POKEMON ////////////////

void* iniciar_paquete_serializado_GetPokemon(int* tamanio_paquete,t_pokemon* pokemon_pedido){

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = GET_POKEMON;

	paquete->buffer = malloc(sizeof(t_buffer));

	char* pokemon = pokemon_pedido->especie;
	uint32_t caracteresPokemon = strlen(pokemon) + 1;

						//INT CARACTERES + POKEMON
	paquete->buffer->size = sizeof(uint32_t) + caracteresPokemon;
	void* stream = malloc(paquete->buffer->size);
	int offset = 0;

		memcpy(stream + offset, &caracteresPokemon, sizeof(uint32_t));
		offset += sizeof(uint32_t);

		memcpy(stream + offset, &pokemon, caracteresPokemon);
		offset +=caracteresPokemon;

		paquete->buffer->stream = stream;

						// TAMAÑO STREAM + OP CODE + VARIABLE SIZE
	*tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);

	void* a_enviar = malloc((*tamanio_paquete));
	int offsetDeSerializacion = 0;

		memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(op_code));
		offsetDeSerializacion += sizeof(op_code);

		memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(uint32_t));
		offsetDeSerializacion +=sizeof(uint32_t);

		memcpy(a_enviar + offset, &(paquete->buffer->stream), paquete->buffer->size);

	free(stream);
	free(paquete->buffer);
	free(paquete);

	return a_enviar;

}


//----------------------- RECEPCION DE MENSAJES DE GAMEBOY -----------------------

/*
void recibir_AppearedPokemon(int socket_cliente){

		uint32_t tamanio_buffer;
		recv(socket_cliente, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

		uint32_t caracteresPokemon;
		recv(socket_cliente, &caracteresPokemon, sizeof(uint32_t), MSG_WAITALL);

		char* pokemon = (char*)malloc(caracteresPokemon);
		recv(socket_cliente, pokemon, caracteresPokemon, MSG_WAITALL);

		uint32_t posX;
		recv(socket_cliente, &posX, sizeof(uint32_t), MSG_WAITALL);

		uint32_t posY;
		recv(socket_cliente, &posY, sizeof(uint32_t), MSG_WAITALL);

}*/

void recibir_appeared_pokemon_loggeo(int socket_cliente){


		uint32_t tamanio_buffer;
		recv(socket_cliente, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

		char* m1 = string_from_format("tamaño buffer: %d.", tamanio_buffer);
					completar_logger(m1, "BROKER", LOG_LEVEL_INFO);

		uint32_t tamanio_leido= 0;

		uint32_t caracteresPokemon;
		recv(socket_cliente, &caracteresPokemon, sizeof(uint32_t), MSG_WAITALL);
		tamanio_leido += sizeof(uint32_t);
			//log
			char* m2 = string_from_format("Los caracteres de pokemon son: %d.", caracteresPokemon);
			completar_logger(m2, "BROKER", LOG_LEVEL_INFO);

		char* pokemon = (char*)malloc(caracteresPokemon);
		recv(socket_cliente, pokemon, caracteresPokemon, MSG_WAITALL);
		tamanio_leido += caracteresPokemon;
			//log
			completar_logger(pokemon, "BROKER", LOG_LEVEL_INFO);

		uint32_t posX;
		recv(socket_cliente, &posX, sizeof(uint32_t), MSG_WAITALL);
		tamanio_leido += sizeof(uint32_t);
			//log
		char* m3 = string_from_format("La posicion en X es: %d", posX);
		completar_logger(m3, "BROKER", LOG_LEVEL_INFO);

		uint32_t posY;
		recv(socket_cliente, &posY, sizeof(uint32_t), MSG_WAITALL);
		tamanio_leido += sizeof(uint32_t);
			//log
		char* m4 = string_from_format("La posicion en Y es: %d", posY);
		completar_logger(m4, "BROKER", LOG_LEVEL_INFO);

		if((tamanio_buffer - tamanio_leido) != 0){
			uint32_t id_correlativo;
			recv(socket_cliente, &id_correlativo, sizeof(uint32_t), MSG_WAITALL);
						//log
				char* m5 = string_from_format("El id correlativo es: %d", id_correlativo);
				completar_logger(m5, "BROKER", LOG_LEVEL_INFO);
		}
		//armar pokemon con los datos recibidos y mandar el pokemon armado a APARACION_POKEMON()
}

/*
void recibir_CaughtPokemon(int socket_cliente){

		uint32_t tamanio_buffer;
		recv(socket_cliente, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

		uint32_t caracteresPokemon;
		recv(socket_cliente, &caracteresPokemon, sizeof(uint32_t), MSG_WAITALL);

		char* pokemon = (char*)malloc(caracteresPokemon);
		recv(socket_cliente, pokemon, caracteresPokemon, MSG_WAITALL);

		uint32_t posX;
		recv(socket_cliente, &posX, sizeof(uint32_t), MSG_WAITALL);

		uint32_t posY;
		recv(socket_cliente, &posY, sizeof(uint32_t), MSG_WAITALL);

}*/


void recibir_caught_pokemon_loggeo(int socket_cliente){

		uint32_t tamanio_buffer;
		recv(socket_cliente, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

		char* m1 = string_from_format("tamaño buffer: %d.", tamanio_buffer);
					completar_logger(m1, "BROKER", LOG_LEVEL_INFO);

		uint32_t tamanio_leido= 0;

		uint32_t caracteresPokemon;
		recv(socket_cliente, &caracteresPokemon, sizeof(uint32_t), MSG_WAITALL);
		tamanio_leido += sizeof(uint32_t);
			//log
			char* m2 = string_from_format("Los caracteres de pokemon son: %d.", caracteresPokemon);
			completar_logger(m2, "BROKER", LOG_LEVEL_INFO);

		char* pokemon = (char*)malloc(caracteresPokemon);
		recv(socket_cliente, pokemon, caracteresPokemon, MSG_WAITALL);
		tamanio_leido += caracteresPokemon;
			//log
			completar_logger(pokemon, "BROKER", LOG_LEVEL_INFO);

		uint32_t posX;
		recv(socket_cliente, &posX, sizeof(uint32_t), MSG_WAITALL);
		tamanio_leido += sizeof(uint32_t);
			//log
		char* m3 = string_from_format("La posicion en X es: %d", posX);
		completar_logger(m3, "BROKER", LOG_LEVEL_INFO);

		uint32_t posY;
		recv(socket_cliente, &posY, sizeof(uint32_t), MSG_WAITALL);
		tamanio_leido += sizeof(uint32_t);
			//log
		char* m4 = string_from_format("La posicion en Y es: %d", posY);
		completar_logger(m4, "BROKER", LOG_LEVEL_INFO);

		if((tamanio_buffer - tamanio_leido) != 0){
			uint32_t id_correlativo;
			recv(socket_cliente, &id_correlativo, sizeof(uint32_t), MSG_WAITALL);
						//log
				char* m5 = string_from_format("El id correlativo es: %d", id_correlativo);
				completar_logger(m5, "BROKER", LOG_LEVEL_INFO);
		}
		//armar pokemon con los datos recibidos y mandar el pokemon armado a APARACION_POKEMON()
}
/*
void recibir_LocalizedPokemon(int socket_cliente){

		uint32_t tamanio_buffer;
		recv(socket_cliente, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

		uint32_t caracteresPokemon;
		recv(socket_cliente, &caracteresPokemon, sizeof(uint32_t), MSG_WAITALL);

		char* pokemon = (char*)malloc(caracteresPokemon);
		recv(socket_cliente, pokemon, caracteresPokemon, MSG_WAITALL);

		uint32_t posX;
		recv(socket_cliente, &posX, sizeof(uint32_t), MSG_WAITALL);

		uint32_t posY;
		recv(socket_cliente, &posY, sizeof(uint32_t), MSG_WAITALL);

}*/


void recibir_localized_pokemon_loggeo(int socket_cliente){


		uint32_t tamanio_buffer;
		recv(socket_cliente, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

		char* m1 = string_from_format("tamaño buffer: %d.", tamanio_buffer);
					completar_logger(m1, "BROKER", LOG_LEVEL_INFO);

		uint32_t tamanio_leido= 0;

		uint32_t caracteresPokemon;
		recv(socket_cliente, &caracteresPokemon, sizeof(uint32_t), MSG_WAITALL);
		tamanio_leido += sizeof(uint32_t);
			//log
			char* m2 = string_from_format("Los caracteres de pokemon son: %d.", caracteresPokemon);
			completar_logger(m2, "BROKER", LOG_LEVEL_INFO);

		char* pokemon = (char*)malloc(caracteresPokemon);
		recv(socket_cliente, pokemon, caracteresPokemon, MSG_WAITALL);
		tamanio_leido += caracteresPokemon;
			//log
			completar_logger(pokemon, "BROKER", LOG_LEVEL_INFO);

		uint32_t posX;
		recv(socket_cliente, &posX, sizeof(uint32_t), MSG_WAITALL);
		tamanio_leido += sizeof(uint32_t);
			//log
		char* m3 = string_from_format("La posicion en X es: %d", posX);
		completar_logger(m3, "BROKER", LOG_LEVEL_INFO);

		uint32_t posY;
		recv(socket_cliente, &posY, sizeof(uint32_t), MSG_WAITALL);
		tamanio_leido += sizeof(uint32_t);
			//log
		char* m4 = string_from_format("La posicion en Y es: %d", posY);
		completar_logger(m4, "BROKER", LOG_LEVEL_INFO);

		if((tamanio_buffer - tamanio_leido) != 0){
			uint32_t id_correlativo;
			recv(socket_cliente, &id_correlativo, sizeof(uint32_t), MSG_WAITALL);
						//log
				char* m5 = string_from_format("El id correlativo es: %d", id_correlativo);
				completar_logger(m5, "BROKER", LOG_LEVEL_INFO);
		}
		//armar pokemon con los datos recibidos y mandar el pokemon armado a APARACION_POKEMON()
}

//----------------------- RECEPCION DE MENSAJES DE GAMEBOY -----------------------

void recibir_mensaje_Broker(int socket_cliente){
	op_code cod_op;
	recv(socket_cliente, &cod_op, sizeof(op_code), MSG_WAITALL);

	switch(cod_op){
	case 2:
		recibir_appeared_pokemon_loggeo(socket_cliente);
	}
}



/*
void recibir_appeared_pokemon_loggeo(int socket_cliente){

		uint32_t tamanio_buffer;
		recv(socket_cliente, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

		//void* buffer = malloc(tamanio_buffer);
			//log
			char* m1 = string_from_format("El tamanio del buffer es: %d.", tamanio_buffer);
			completar_logger(m1, "BROKER", LOG_LEVEL_INFO);

		uint32_t caracteresPokemon;
		recv(socket_cliente, &caracteresPokemon, sizeof(uint32_t), MSG_WAITALL);
			//log
			char* m2 = string_from_format("Los caracteres de pokemon son: %d.", caracteresPokemon);
			completar_logger(m2, "BROKER", LOG_LEVEL_INFO);

		char* pokemon = (char*)malloc(caracteresPokemon);
		recv(socket_cliente, pokemon, caracteresPokemon, MSG_WAITALL);
			//log
			completar_logger(pokemon, "BROKER", LOG_LEVEL_INFO);

		uint32_t posX;
		recv(socket_cliente, &posX, sizeof(uint32_t), MSG_WAITALL);
			//log
		char* m3 = string_from_format("La posicion en X es: %d", posX);
		completar_logger(m3, "BROKER", LOG_LEVEL_INFO);

		uint32_t posY;
		recv(socket_cliente, &posY, sizeof(uint32_t), MSG_WAITALL);
			//log
		char* m4 = string_from_format("La posicion en Y es: %d", posY);
		completar_logger(m4, "BROKER", LOG_LEVEL_INFO);

		//armar pokemon con los datos recibidos y mandar el pokemon armado a APARACION_POKEMON()
}


*/








void recibir_mensaje2(int socket_cliente)
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
