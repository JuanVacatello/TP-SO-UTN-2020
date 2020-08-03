#include "conexion.h"

// Crear conexion (para Broker)

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
		return -1;
	}

	freeaddrinfo(server_info);

	char* mensaje = string_from_format("el socket de conexion es: %d.", socket_cliente);
	completar_logger(mensaje, "GAMECARD", LOG_LEVEL_INFO);

	return socket_cliente;
}

// Esperar y recibir mensajes de Gameboy

void iniciar_espera_mensajes_Gameboy(void){

	int socket_servidor;
    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    char* ip = obtener_ip_gamecard();
    char* puerto = obtener_puerto_gamecard();

    getaddrinfo(ip, puerto, &hints, &servinfo);

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

    while(1){
    	esperar_gameboy(socket_servidor);
    }
}

void esperar_gameboy(int socket_servidor){
	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);
	sem_wait(&MUTEX_MENSAJES_GB);
	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	pthread_create(&thread,NULL,(void*)serve_client,&socket_cliente);
	pthread_detach(thread);
}

void serve_client(int* socket){
	op_code cod_op;
	if(recv(*socket, &cod_op, sizeof(op_code), MSG_WAITALL) == -1)
		pthread_exit(NULL);

	char* mensaje = string_from_format("El codigo de operacion es: %d.", cod_op);
	completar_logger(mensaje, "GAMECARD", LOG_LEVEL_INFO);

	process_request(cod_op, *socket);
}

void process_request(op_code cod_op, int socket_cliente) {

	switch(cod_op){
		case 1:
			recibir_new_pokemon(socket_cliente);//pthread_create(&hilo_new_pokemon_gameboy, NULL, recibir_new_pokemon, socket_cliente);
			//pthread_detach(hilo_new_pokemon_gameboy);
			break;
		case 3:
			recibir_catch_pokemon(socket_cliente);//pthread_create(&hilo_catch_pokemon_gameboy, NULL, recibir_catch_pokemon, socket_cliente);
			//pthread_detach(hilo_catch_pokemon_gameboy);
			break;
		case 5:
			recibir_get_pokemon(socket_cliente);//pthread_create(&hilo_get_pokemon_gameboy, NULL, recibir_get_pokemon, socket_cliente);
			//pthread_detach(hilo_get_pokemon_gameboy);
			break;
	}
	sem_post(&MUTEX_MENSAJES_GB);
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

// Conexion con Broker y espera de mensajes

void new_pokemon_broker(){
	int socket_broker = suscribirse_globalmente(1);

	while(1){

		op_code cod_op = -1;
		recv(socket_broker, &cod_op, sizeof(op_code), MSG_WAITALL);

		if(cod_op == NEW_POKEMON){
			pthread_create(&hilo_new_pokemon_broker, NULL, recibir_new_pokemon, NULL);
			pthread_detach(hilo_new_pokemon_broker);
		}
	}
}

void catch_pokemon_broker(){
	int socket_broker = suscribirse_globalmente(CATCH_POKEMON);

	while(1){

		op_code cod_op = -1;
		recv(socket_broker, &cod_op, sizeof(op_code), MSG_WAITALL);

		if(cod_op == CATCH_POKEMON){
			pthread_create(&hilo_catch_pokemon_broker, NULL, recibir_catch_pokemon, socket_broker);
			pthread_detach(hilo_catch_pokemon_broker);
		}
	}
}

void get_pokemon_broker(){
	int socket_broker = suscribirse_globalmente(GET_POKEMON);

	while(1){

		op_code cod_op = -1;
		recv(socket_broker, &cod_op, sizeof(op_code), MSG_WAITALL);

		if(cod_op == GET_POKEMON){
			pthread_create(&hilo_get_pokemon_broker, NULL, recibir_get_pokemon, socket_broker);
			pthread_detach(hilo_get_pokemon_broker);
		}
	}
}

// Suscripciones a las colas de Broker

int suscribirse_globalmente(op_code cola_a_suscribirse){

	char* puerto_broker = obtener_puerto_broker();
	char* ip_broker = obtener_ip_broker();

	int socket_broker = -1;
	int tiempo_reconexion = tiempo_de_reintento_conexion();
	sem_wait(&MUTEX_SUB);
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

// Recibir mensajes de GameBoy y Broker

void recibir_new_pokemon(int socket_cliente){
	uint32_t tamanio_buffer;
	recv(socket_cliente, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

	uint32_t mensaje_id;
	recv(socket_cliente, &mensaje_id, sizeof(uint32_t), MSG_WAITALL);

	uint32_t caracteresPokemon;
	recv(socket_cliente, &caracteresPokemon, sizeof(uint32_t), MSG_WAITALL);

	char* pokemon = malloc(caracteresPokemon);
	recv(socket_cliente, pokemon, caracteresPokemon, MSG_WAITALL);

	uint32_t posX;
	recv(socket_cliente, &posX, sizeof(uint32_t), MSG_WAITALL);

	uint32_t posY;
	recv(socket_cliente, &posY, sizeof(uint32_t), MSG_WAITALL);

	uint32_t cantidad;
	recv(socket_cliente, &cantidad, sizeof(uint32_t), MSG_WAITALL);

		// Una vez chequeado que funciona todo se puede borrar esto:
		printf("El tamaño del buffer es %d \n", tamanio_buffer);
		printf("El mensaje_id es %d \n", mensaje_id);
		printf("El largo del pokemon es %d \n", caracteresPokemon);
		printf("El pokemon es %s \n", pokemon);
		printf("La posicion en x es %d \n", posX);
		printf("La posicion en Y %d \n", posY);
		printf("La cantidad es %d \n", cantidad);

	responder_ack(mensaje_id, socket_cliente);

	new_pokemon(pokemon, posX, posY, cantidad);

	uint32_t id_mensaje_correlativo = mensaje_id;
	//enviar_appeared_pokemon(pokemon, posX, posY, id_mensaje_correlativo);

}

void recibir_catch_pokemon(int socket_cliente){//RECIBE TODO PERFECTO (NO MUEVAN EL ORDEN DE LAS COSAS BOE)

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

		// Una vez chequeado que funciona todo se puede borrar esto:
		printf("El tamaño del buffer es %d \n", tamanio_buffer);
		printf("El mensaje_id es %d \n", mensaje_id);
		printf("El largo del pokemon es %d \n", caracteresPokemon);
		printf("El pokemon es %s \n", pokemon);
		printf("La posicion en x es %d \n", posX);
		printf("La posicion en Y %d \n", posY);

	responder_ack(mensaje_id, socket_cliente);

	uint32_t se_pudo_encontrar = catch_pokemon(pokemon, posX, posY);

	//enviar_caught_pokemon(mensaje_id, se_pudo_encontrar);
}

void recibir_get_pokemon(int socket_cliente){//RECIBE TODO PERFECTO (NO MUEVAN EL ORDEN DE LAS COSAS BOE)

	uint32_t tamanio_buffer;
	recv(socket_cliente, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

	uint32_t mensaje_id;
	recv(socket_cliente, &mensaje_id, sizeof(uint32_t), MSG_WAITALL);

	uint32_t caracteresPokemon;
	recv(socket_cliente, &caracteresPokemon, sizeof(uint32_t), MSG_WAITALL);

	char* pokemon = (char*)malloc(caracteresPokemon);
	recv(socket_cliente, pokemon, caracteresPokemon, MSG_WAITALL);

		// Una vez chequeado que funciona todo se puede borrar esto:
		printf("El tamaño del buffer es %d \n", tamanio_buffer);
		printf("El mensaje_id es %d \n", mensaje_id);
		printf("El largo del pokemon es %d \n", caracteresPokemon);
		printf("El pokemon es %s \n", pokemon);


	responder_ack(mensaje_id, socket_cliente);

	uint32_t tamanio_void = 0;
	void* respuesta = get_pokemon(pokemon, &tamanio_void);

	//enviar_localized_pokemon(respuesta, tamanio_void, pokemon, mensaje_id);
}

// Enviar mensaje a Broker

void enviar_appeared_pokemon(char* pokemon, uint32_t posX, uint32_t posY, uint32_t id_mensaje_correlativo){

	char* puerto_broker = obtener_puerto_broker();
	char* ip_broker = obtener_ip_broker();
	int socket_broker = crear_conexion(ip_broker,puerto_broker);

	if(socket_broker == -1){
		// Comportamiento default en caso de que no se pueda conectar a Broker
		// Segun lo poco que lei en el enunciado solo hay que avisarlo por logs y seguir la ejecucion pero no se quizas
		// hay mas info que esa
	}

	int tamanio_paquete = 0;
	void* a_enviar = iniciar_paquete_serializado_AppearedPokemon(&tamanio_paquete, pokemon, posX, posY, id_mensaje_correlativo);

	if(send(socket_broker,a_enviar,tamanio_paquete,0) == -1){
		printf("Error en enviar por el socket");
		exit(3);
	}
}

void* iniciar_paquete_serializado_AppearedPokemon(int* tamanio_paquete, char* pokemon, uint32_t posX, uint32_t posY, uint32_t id_mensaje_correlativo){
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = 2;
	paquete->buffer = malloc(sizeof(t_buffer));

	uint32_t caracteres_pokemon = strlen(pokemon)+1;
	paquete->buffer->size = sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+caracteres_pokemon;

	void* stream = malloc(paquete->buffer->size);
	int offset = 0;

	memcpy(stream + offset, &caracteres_pokemon, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(stream + offset, pokemon, caracteres_pokemon);
	offset += caracteres_pokemon;

	memcpy(stream + offset, &posX, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(stream + offset, &posY, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(stream + offset, &id_mensaje_correlativo, sizeof(uint32_t));

	paquete->buffer->stream = stream;

	*tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);
	void* a_enviar = serializar_paquete(paquete, tamanio_paquete);
	return a_enviar;
}

void enviar_caught_pokemon(uint32_t id_mensaje_correlativo, uint32_t se_pudo_atrapar){

	char* puerto_broker = obtener_puerto_broker();
	char* ip_broker = obtener_ip_broker();
	int socket_broker = crear_conexion(ip_broker,puerto_broker);

	if(socket_broker == -1){
		// Comportamiento default en caso de que no se pueda conectar a Broker
		// Segun lo poco que lei en el enunciado solo hay que avisarlo por logs y seguir la ejecucion pero no se quizas
		// hay mas info que esa
	}

	int tamanio_paquete = 0;
	void* a_enviar = iniciar_paquete_serializado_CaughtPokemon(&tamanio_paquete, id_mensaje_correlativo, se_pudo_atrapar);

	if(send(socket_broker,a_enviar,tamanio_paquete,0) == -1){
		printf("Error en enviar por el socket");
		exit(3);
	}
}

void* iniciar_paquete_serializado_CaughtPokemon(int* tamanio_paquete, uint32_t id_mensaje_correlativo, uint32_t se_pudo_atrapar){
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = 4;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = sizeof(uint32_t) + sizeof(uint32_t);

	void* stream = malloc(paquete->buffer->size);
	int desplazamiento = 0;

	memcpy(stream + desplazamiento, &id_mensaje_correlativo, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(stream + desplazamiento, &se_pudo_atrapar, sizeof(uint32_t));

	paquete->buffer->stream = stream;

	*tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);
	void* a_enviar = serializar_paquete(paquete, tamanio_paquete);
	return a_enviar;
}

void enviar_localized_pokemon(void* cantidad_y_posiciones, uint32_t tamanio_void, char* pokemon, uint32_t id_mensaje_correlativo){

	char* puerto_broker = obtener_puerto_broker();
	char* ip_broker = obtener_ip_broker();
	int socket_broker = crear_conexion(ip_broker,puerto_broker);

	if(socket_broker == -1){
		// Comportamiento default en caso de que no se pueda conectar a Broker
		// Segun lo poco que lei en el enunciado solo hay que avisarlo por logs y seguir la ejecucion pero no se quizas
		// hay mas info que esa
	}

	int tamanio_paquete = 0;
	void* a_enviar = iniciar_paquete_serializado_LocalizedPokemon(&tamanio_paquete, id_mensaje_correlativo, pokemon, cantidad_y_posiciones, tamanio_void);

	if(send(socket_broker,a_enviar,tamanio_paquete,0) == -1){
		printf("Error en enviar por el socket");
		exit(3);
	}
}

void* iniciar_paquete_serializado_LocalizedPokemon(int* tamanio_paquete, uint32_t id_mensaje_correlativo, char* pokemon, void* cantidad_y_posiciones, uint32_t tamanio_void){

	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = 1;
	paquete->buffer = malloc(sizeof(t_buffer));
	uint32_t caracteres_pokemon = strlen(pokemon)+1;
							// ID CORRELATIVO + CARACTERES + POKEMON + CANTIDAD + (POSX+POSY)*CANTIDAD
	paquete->buffer->size = sizeof(uint32_t) + sizeof(uint32_t) + caracteres_pokemon + tamanio_void;

	void* stream = malloc(paquete->buffer->size);
	int desplazamiento = 0;

	memcpy(stream + desplazamiento, &id_mensaje_correlativo, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(stream + desplazamiento, &caracteres_pokemon, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(stream + desplazamiento, pokemon, caracteres_pokemon);
	desplazamiento += caracteres_pokemon;

	memcpy(stream + desplazamiento, cantidad_y_posiciones, tamanio_void);
	desplazamiento += tamanio_void;

	paquete->buffer->stream = stream;

	*tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);
	void* a_enviar = serializar_paquete(paquete, tamanio_paquete);
	return a_enviar;
}

// ACK

void responder_ack(uint32_t mensaje_id, int socket_broker){

	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = 7;
	paquete->buffer = malloc(sizeof(t_buffer));

	char* mensaje = "ACK";
	uint32_t caracteres = strlen(mensaje)+1;
	paquete->buffer->size = sizeof(uint32_t) + sizeof(uint32_t) + caracteres + sizeof(uint32_t);
	void* stream = malloc(paquete->buffer->size);
	int offset = 0;

	memcpy(stream + offset, &caracteres, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(stream + offset, mensaje, caracteres);
	offset += caracteres;

	memcpy(stream + offset, &mensaje_id, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	uint32_t proceso_id = obtener_id_propio();
	memcpy(stream + offset, &proceso_id, sizeof(uint32_t));

	paquete->buffer->stream = stream;

	int tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);
	void* a_enviar = serializar_paquete(paquete,&tamanio_paquete);

	if(send(socket_broker,a_enviar,tamanio_paquete,0) == -1){
		completar_logger("Error en enviar por el socket","GAMEBOY", LOG_LEVEL_INFO);
		exit(3);
	}

	free(paquete->buffer);
	free(paquete);
}

char* recibir_mensaje(int socket_broker){

	op_code code_op;
	recv(socket_broker, &code_op, sizeof(op_code), MSG_WAITALL);

	uint32_t tamanio_buffer;
	recv(socket_broker, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

	char* mensaje = malloc(tamanio_buffer);
	recv(socket_broker, mensaje, tamanio_buffer, MSG_WAITALL);

	sem_post(&MUTEX_SUB);

	return mensaje;
}
