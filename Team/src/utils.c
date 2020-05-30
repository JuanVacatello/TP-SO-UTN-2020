
#include"utils.h"
#include"configTeam.h"

void iniciar_servidor(void)
{
	int socket_servidor;

    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(obtener_ip(), obtener_puerto(), &hints, &servinfo);

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

    freeaddrinfo(servinfo);

    while(1)
    	esperar_cliente(socket_servidor);
}

void esperar_cliente(int socket_servidor)
{
	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	pthread_create(&thread,NULL,(void*)serve_client,&socket_cliente);
	pthread_detach(thread);

}

void serve_client(int* socket)
{
	int cod_op;
	if(recv(*socket, &cod_op, sizeof(int), MSG_WAITALL) == -1)
		cod_op = -1;
	process_request(cod_op, *socket);
}

void process_request(int cod_op, int cliente_fd) {
	int size;
	void* msg;
		switch (cod_op) {
		case MENSAJE:
			msg = recibir_mensaje(cliente_fd, &size);
			devolver_mensaje(msg, size, cliente_fd);
			free(msg);
			break;
		case 0:
			pthread_exit(NULL);
		case -1:
			pthread_exit(NULL);
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

void* serializar_paquete(t_paquete* paquete, int bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
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


//----------------------- COMUNICACION CON BROKER -----------------------

void enviar_mensaje_a_broker(int socket_cliente, op_code codigo_operacion, char* argv[], t_entrenador* entrenador) //HAY QUE VER ESTE TEMA DEL PARAMETRO
{
	int tamanio_paquete = 0;
	void* a_enviar;

	switch(codigo_operacion){
	case CATCH_POKEMON:
		a_enviar = iniciar_paquete_serializado_CatchPokemon(&tamanio_paquete,entrenador);
		break;
	case GET_POKEMON:
		a_enviar = iniciar_paquete_serializado_GetPokemon(&tamanio_paquete,argv);
		break;
	case 6:
		break;
	}

	send(socket_cliente,a_enviar,tamanio_paquete,0);
	free(a_enviar);

//fflush(stdout);
}

//////////////// CATCH POKEMON ////////////////

void* iniciar_paquete_serializado_CatchPokemon(int* tamanio_paquete,t_entrenador* entrenador){ //Deberia de recibir el entrenador pero no estoy seguro si como parametro o VG.

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = CATCH_POKEMON;

	paquete->buffer = malloc(sizeof(t_buffer));

	char* pokemon = entrenador->pokemon_a_atrapar->especie;
	int caracteresPokemon = strlen(pokemon) + 1;
	//Quizas es mejor hacer una variable pokemon para no tener tantas flechitas
	int posX = entrenador->pokemon_a_atrapar->posicion->x;
	int posY = entrenador->pokemon_a_atrapar->posicion->y;

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

//////////////// GET POKEMON ////////////////

void* iniciar_paquete_serializado_GetPokemon(int* tamanio_paquete,char* argv[]){

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = GET_POKEMON;

	paquete->buffer = malloc(sizeof(t_buffer));

	char* pokemon = argv[2];
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
