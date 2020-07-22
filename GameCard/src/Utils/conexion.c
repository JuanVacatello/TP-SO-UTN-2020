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

// RECIBIR MENSAJES DE GAMEBOY

void iniciar_espera_mensajes_Gameboy(void){

	int socket_servidor;
    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(IP, PUERTO, &hints, &servinfo);

    for (p=servinfo; p != NULL; p = p->ai_next){
        if ((socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            continue;

        if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
            close(socket_servidor);
            continue;
        }
        break;
    }

	if(listen(socket_servidor, SOMAXCONN) == -1){
		close(socket_servidor);
		exit(6);
	}

    freeaddrinfo(servinfo);

    while(1)

    	esperar_cliente(socket_servidor);
}

void esperar_cliente(int socket_servidor)
{
	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);
	//sem_wait(&MUTEX_MENSAJES_GB);
	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	pthread_create(&thread,NULL,(void*)serve_client,&socket_cliente);
	pthread_detach(thread);
}

void serve_client(int* socket)
{
	op_code cod_op;
	if(recv(*socket, &cod_op, sizeof(op_code), MSG_WAITALL) == -1)
		pthread_exit(NULL);

	//char* mensaje = string_from_format("El codigo de operacion es: %d.", cod_op);
	//completar_logger(mensaje, "TEAM", LOG_LEVEL_INFO);

	process_request(cod_op, *socket);
}

// ATENDER AL CLIENTE

void process_request(op_code cod_op, int socket_cliente) {

	switch(cod_op){
		case 1:
			recibir_new_pokemon(socket_cliente);
			break;
		case 3:
			recibir_catch_pokemon(socket_cliente);
			break;
		case 5:
			recibir_get_pokemon(socket_cliente);
			break;
	}
	//sem_post(&MUTEX_MENSAJES_GB);
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

// RECEPCION DE MENSAJES DE BROKER

void new_pokemon_broker(){
	int socket_broker = suscribirse_globalmente(NEW_POKEMON);

	while(1){

		op_code cod_op = -1;
		recv(socket_broker, &cod_op, sizeof(op_code), MSG_WAITALL);

		if(cod_op == NEW_POKEMON){

			//char* mensaje = string_from_format("El codigo de operacion es: %d.", cod_op);
			//completar_logger(mensaje, "TEAM", LOG_LEVEL_INFO);

			recibir_new_pokemon(socket_broker);

		}
	}
}

void catch_pokemon_broker(){
	int socket_broker = suscribirse_globalmente(CATCH_POKEMON);

	while(1){

		op_code cod_op = -1;
		recv(socket_broker, &cod_op, sizeof(op_code), MSG_WAITALL);

		if(cod_op == CATCH_POKEMON){

			//char* mensaje = string_from_format("El codigo de operacion es: %d.", cod_op);
			//completar_logger(mensaje, "TEAM", LOG_LEVEL_INFO);

			recibir_catch_pokemon(socket_broker);

		}
	}
}

void get_pokemon_broker(){
	int socket_broker = suscribirse_globalmente(GET_POKEMON);

	while(1){

		op_code cod_op = -1;
		recv(socket_broker, &cod_op, sizeof(op_code), MSG_WAITALL);

		if(cod_op == GET_POKEMON){

			//char* mensaje = string_from_format("El codigo de operacion es: %d.", cod_op);
			//completar_logger(mensaje, "TEAM", LOG_LEVEL_INFO);

			recibir_get_pokemon(socket_broker);

		}
	}
}

// BROKER - SUSCRIPTOR

int suscribirse_globalmente(op_code cola_a_suscribirse){

	char* puerto_broker = obtener_puerto_broker();
	char* ip_broker = obtener_ip_broker();

	int socket_broker = -1;
	int tiempo_reconexion = tiempo_de_reintento_conexion();
	//sem_wait(&MUTEX_SUB);
	socket_broker = crear_conexion(ip_broker,puerto_broker);

	while(socket_broker == -1){
		log_reintento_comunicacion_Broker_fallido();
		log_intento_reintento_comunicacion_broker();
		sleep(tiempo_reconexion);
		socket_broker = crear_conexion(ip_broker,puerto_broker);
	}

	log_intento_comunicacion_Broker_exitoso();

	int tamanio_paquete = 0;
	void* a_enviar = suscribirse_a_cola(socket_broker, cola_a_suscribirse, &tamanio_paquete);

	send(socket_broker,a_enviar,tamanio_paquete,0);

	char* mensaje_subscripcion = recibir_mensaje(socket_broker); //por ahora lo dejo despues se sacará
	puts(mensaje_subscripcion);

	//pthread_mutex_unlock(&mutex_conexion);
	free(a_enviar);

	return socket_broker;
}

void* suscribirse_a_cola(int socket_broker, uint32_t cola_a_suscribirse, int* tamanio_paquete){

	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = SUSCRIPTOR;
	paquete->buffer = malloc(sizeof(t_buffer));

	uint32_t process_id = getpid();

	paquete->buffer->size = sizeof(uint32_t)+sizeof(uint32_t);
	void* stream = malloc(paquete->buffer->size);
	int offset = 0;

		memcpy(stream + offset, &process_id, sizeof(uint32_t));
		offset += sizeof(uint32_t);

		memcpy(stream + offset, &cola_a_suscribirse, sizeof(uint32_t));

	paquete->buffer->stream = stream;

						// TAMAÑO STREAM + OP CODE + VARIABLE SIZE
	*tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);
	void* a_enviar = serializar_paquete(paquete, tamanio_paquete);

	free(paquete->buffer);
	free(stream);
	free(paquete);

	return a_enviar;
}


void recibir_new_pokemon(int socket_cliente){
	uint32_t tamanio_buffer;
	recv(socket_cliente, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

	uint32_t mensaje_id;  // Hola chicos este mismo id lo van a tener que poner en el id_mensaje_correlativo del appeared correspondiente a este new salu2
	recv(socket_cliente, &mensaje_id, sizeof(uint32_t), MSG_WAITALL);

	uint32_t caracteresPokemon;
	recv(socket_cliente, &caracteresPokemon, sizeof(uint32_t), MSG_WAITALL);

	char* pokemon = (char*)malloc(caracteresPokemon);
	recv(socket_cliente, pokemon, caracteresPokemon, MSG_WAITALL);

	uint32_t posX;
	recv(socket_cliente, &posX, sizeof(uint32_t), MSG_WAITALL);

	uint32_t posY;
	recv(socket_cliente, &posY, sizeof(uint32_t), MSG_WAITALL);

	uint32_t cantidad;
	recv(socket_cliente, &cantidad, sizeof(uint32_t), MSG_WAITALL);

	agregar_pokemon(pokemon, posX, posY, -1); // Holi no quiero tocar nada pero pasan 4 parametros pero la funcion es de 3

	responder_ack();
}

void recibir_catch_pokemon(int socket_cliente){

	uint32_t tamanio_buffer;
	recv(socket_cliente, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

	uint32_t mensaje_id;
	recv(socket_cliente, &mensaje_id, sizeof(uint32_t), MSG_WAITALL);

	uint32_t caracteresPokemon;
	recv(socket_cliente, &caracteresPokemon, sizeof(uint32_t), MSG_WAITALL);

	char* pokemon = (char*)malloc(caracteresPokemon);
	recv(socket_cliente, pokemon, caracteresPokemon, MSG_WAITALL);

	uint32_t posX;
	recv(socket_cliente, &posX, sizeof(uint32_t), MSG_WAITALL);

	uint32_t posY;
	recv(socket_cliente, &posY, sizeof(uint32_t), MSG_WAITALL);

	responder_ack();

}

void recibir_get_pokemon(int socket_cliente){

	uint32_t tamanio_buffer;
	recv(socket_cliente, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

	uint32_t mensaje_id;
	recv(socket_cliente, &mensaje_id, sizeof(uint32_t), MSG_WAITALL);

	uint32_t caracteresPokemon;
	recv(socket_cliente, &caracteresPokemon, sizeof(uint32_t), MSG_WAITALL);

	char* pokemon = (char*)malloc(caracteresPokemon);
	recv(socket_cliente, pokemon, caracteresPokemon, MSG_WAITALL);

	responder_ack();
}

// MENSAJES A BROKER

//falta todavia

// ACK

void* enviar_ACK(int socket_broker, int* tamanio){

	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = 7;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->stream = "ACK";
	paquete->buffer->size = strlen(paquete->buffer->stream) + 1;

	*tamanio = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);
	void* a_enviar = serializar_paquete(paquete,tamanio);

	return a_enviar;
}

void responder_ack(){
	char* puerto_broker = obtener_puerto_broker();
	char* ip_broker = obtener_ip_broker();

	int socket_ack = crear_conexion(ip_broker,puerto_broker);

	int tamanio = 0;
	void* a_enviar = enviar_ACK(socket_ack, &tamanio);
	send(socket_ack,a_enviar,tamanio,0);

	free(a_enviar);
}

char* recibir_mensaje(int socket_broker){

	op_code code_op;
	recv(socket_broker, &code_op, sizeof(op_code), MSG_WAITALL);

	uint32_t tamanio_buffer;
	recv(socket_broker, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

	char* mensaje = malloc(tamanio_buffer);
	recv(socket_broker, mensaje, tamanio_buffer, MSG_WAITALL);

	//sem_post(&MUTEX_SUB);

	return mensaje;
}
