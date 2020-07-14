#include "servidor.h"
#include <pthread.h>

// CONEXION CON CLIENTE

void iniciar_servidor(void)
{
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
    	esperar_cliente(socket_servidor); //
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
	completar_logger("Se conectó proceso al Broker", "BROKER", LOG_LEVEL_INFO); // LOG OBLIGATORIO

	op_code cod_op;
	if(recv(*socket, &cod_op, sizeof(op_code), MSG_WAITALL) == -1)
		pthread_exit(NULL);

	process_request(cod_op, *socket);
}

// ATENDER AL CLIENTE

void process_request(op_code cod_op, int socket_cliente) {

	if(cod_op != 0){
		completar_logger("Llegó un nuevo mensaje a la cola de mensajes", "BROKER", LOG_LEVEL_INFO); // LOG OBLIGATORIO
	}

	switch (cod_op) {
		case 0:
			atender_suscripcion(socket_cliente);
			enviar_mensaje(socket_cliente, "Suscripto");
			break;
		case 1:
			recibir_new_pokemon(socket_cliente);
			break;
		case 2:
			recibir_appeared_pokemon(socket_cliente);
			break;
		case 3:
			recibir_catch_pokemon(socket_cliente);
			break;
		case 4:
			recibir_caught_pokemon(socket_cliente);
			break;
		case 5:
			recibir_get_pokemon(socket_cliente);
			break;
		case 6:
			recibir_localized_pokemon(socket_cliente);
			break;
		case 7: //ack
			recibir_ack(socket_cliente);
			break;
	}
}

// Enviar mensaje

void enviar_mensaje(int socket_cliente, char* mensaje){
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->stream = mensaje;
	paquete->buffer->size = strlen(mensaje) + 1;

	int tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);
	void* a_enviar = serializar_paquete(paquete,tamanio_paquete);

	send(socket_cliente,a_enviar,tamanio_paquete,0);

	free(paquete);
	free(paquete->buffer);
}

void enviar_mensaje_id(int socket_cliente){
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = sizeof(uint32_t);

	void* stream = malloc(paquete->buffer->size);
	int offset = 0;

		memcpy(stream + offset, &mensaje_id, sizeof(uint32_t));

	paquete->buffer->stream = stream;

	int tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);
	void* a_enviar = serializar_paquete(paquete,tamanio_paquete);

	send(socket_cliente,a_enviar,tamanio_paquete,0);

	free(paquete);
	free(paquete->buffer);
	free(stream);
}

// Atender suscripcion

void atender_suscripcion(int socket_cliente)
{
	uint32_t tamanio_buffer;
	tamanio_buffer = recibir_tamanio_buffer(socket_cliente);

	proceso* suscriptor = modelar_proceso(socket_cliente);

	suscribirse_a_cola(suscriptor, socket_cliente, tamanio_buffer);
}

proceso* modelar_proceso(int socket){

	proceso* suscriptor = malloc(sizeof(proceso));

	recv(socket, &(suscriptor->id), sizeof(uint32_t), MSG_WAITALL);
	suscriptor->socket_cliente = socket;

	return suscriptor;
}

int recibir_tamanio_buffer(int socket){
	uint32_t tamanio_buffer;
	recv(socket, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);
	return tamanio_buffer;
}

void suscribirse_a_cola(proceso* suscriptor, int socket, uint32_t tamanio_buffer){

	uint32_t cola_a_suscribirse;
	recv(socket, &cola_a_suscribirse, sizeof(uint32_t), MSG_WAITALL);

	switch(cola_a_suscribirse){
		case 0:
			break;
		case 1:
			list_add(suscriptores_new_pokemon, suscriptor);
			enviar_mensajes_al_nuevo_suscriptor(mensajes_de_cola_new_pokemon, suscriptor->socket_cliente);
			break;
		case 2:
			list_add(suscriptores_appeared_pokemon, suscriptor);
			enviar_mensajes_al_nuevo_suscriptor(mensajes_de_cola_appeared_pokemon, suscriptor->socket_cliente);
			break;
		case 3:
			list_add(suscriptores_catch_pokemon, suscriptor);
			enviar_mensajes_al_nuevo_suscriptor(mensajes_de_cola_catch_pokemon, suscriptor->socket_cliente);
			break;
		case 4:
			list_add(suscriptores_caught_pokemon, suscriptor);
			enviar_mensajes_al_nuevo_suscriptor(mensajes_de_cola_caught_pokemon, suscriptor->socket_cliente);
			break;
		case 5:
			list_add(suscriptores_get_pokemon, suscriptor);
			enviar_mensajes_al_nuevo_suscriptor(mensajes_de_cola_get_pokemon, suscriptor->socket_cliente);
			break;
		case 6:
			list_add(suscriptores_localized_pokemon, suscriptor);
			enviar_mensajes_al_nuevo_suscriptor(mensajes_de_cola_localized_pokemon, suscriptor->socket_cliente);
			break;
	}

	if(tamanio_buffer == 12){ // Entonces la suscripción es del GameBoy
		uint32_t tiempo_de_suscripcion;
		recv(socket, &tiempo_de_suscripcion, sizeof(uint32_t), MSG_WAITALL);
	}

	char* log = string_from_format("Se suscribió el proceso con id %d y con socket %d a la cola de mensajes %d", suscriptor->id, suscriptor->socket_cliente, cola_a_suscribirse);
	completar_logger(log, "BROKER", LOG_LEVEL_INFO); //LOG OBLIGATORIO

}

void enviar_mensajes_al_nuevo_suscriptor(t_list* mensajes_de_dicha_cola, int socket_suscriptor){

	int tamanio_lista = list_size(mensajes_de_dicha_cola);
	printf("El tamaño de la lista es %d", tamanio_lista);
	t_mensaje_a_guardar* mensaje_a_enviar = malloc(sizeof(t_mensaje_a_guardar));

	for(int i = 0; i<tamanio_lista; i++){

		mensaje_a_enviar = list_get(mensajes_de_dicha_cola, i);

		mensaje_a_enviar->ubicacion_mensaje->ultima_referencia = timestamp; // Aumento el timestamp
		timestamp++;

		t_paquete* paquete = malloc(sizeof(t_paquete));
		paquete->codigo_operacion = mensaje_a_enviar->codigo_operacion;;
		paquete->buffer = malloc(sizeof(t_buffer));
		paquete->buffer->size = mensaje_a_enviar->tamanio_buffer;

		void* stream = malloc(paquete->buffer->size);
		int inicio_mensaje = mensaje_a_enviar->ubicacion_mensaje->byte_comienzo_ocupado;
		int tamanio_mensaje = mensaje_a_enviar->ubicacion_mensaje->tamanio_ocupado;

		memcpy(stream, memoria_principal + inicio_mensaje, tamanio_mensaje);

		if(paquete->codigo_operacion == 4){
			memcpy(stream + tamanio_mensaje, &(mensaje_a_enviar->id_mensaje_correlativo), sizeof(uint32_t));
		}

		paquete->buffer->stream = stream; //mensaje_a_enviar->contenido_del_mensaje;

		int tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);
		void* a_enviar = serializar_paquete(paquete, tamanio_paquete);

		if(send(socket_suscriptor,a_enviar,tamanio_paquete,0) == -1){
			completar_logger("Error en enviar por el socket","BROKER", LOG_LEVEL_INFO);
			exit(3);

		char* mensaje = string_from_format("Se le envió un mensaje al suscriptor de socket: %d.", socket_suscriptor);
		completar_logger(mensaje, "Broker", LOG_LEVEL_INFO); // LOG OBLIGATORIO

		}

		free(paquete);
		free(paquete->buffer);
		free(stream);
	}

	free(mensaje_a_enviar);
}

// Recepcion de mensajes y almacenamiento de mensajes en memoria

void recibir_new_pokemon(int socket_cliente)
{
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

	uint32_t cantidad;
	recv(socket_cliente, &cantidad, sizeof(uint32_t), MSG_WAITALL);

	// Creacion de bloque a guardar en memoria

	void* bloque_a_agregar_en_memoria = malloc(tamanio_buffer); // no cuenta el \n del nombre del Pokemon
	int desplazamiento = 0;

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, &caracteresPokemon, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, pokemon, caracteresPokemon);
	desplazamiento += caracteresPokemon;

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, &posX, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, &posY, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, &cantidad, sizeof(uint32_t));

	t_mensaje_guardado* mensaje_nuevo = malloc(sizeof(t_mensaje_guardado));
	mensaje_nuevo = guardar_mensaje_en_memoria(bloque_a_agregar_en_memoria, tamanio_buffer);

	// Prepararacion y envio de mensaje a suscriptores ---> Una vez que chequeo que se manda, hacerlo con todos

	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = 1;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = tamanio_buffer;
	paquete->buffer->stream = bloque_a_agregar_en_memoria;

	int tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);
	void* a_enviar = serializar_paquete(paquete, tamanio_paquete);
	reenviar_mensaje_a_suscriptores(a_enviar, tamanio_paquete, suscriptores_new_pokemon);

	// Guardo información del mensaje

	guardar_mensaje_en_cola(1, mensajes_de_cola_new_pokemon, mensaje_nuevo, tamanio_buffer, NULL);

	free(bloque_a_agregar_en_memoria);
	free(paquete);
	free(paquete->buffer);
}

void recibir_appeared_pokemon(int socket_cliente){

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

	uint32_t id_mensaje_correlativo;
	recv(socket_cliente, &id_mensaje_correlativo, sizeof(uint32_t), MSG_WAITALL);

	// Creacion de bloque a guardar en memoria

	uint32_t tamanio_buffer_sin_id_mensaje = tamanio_buffer-sizeof(uint32_t);
	void* bloque_a_agregar_en_memoria = malloc(tamanio_buffer_sin_id_mensaje); // no cuenta el \n del nombre del Pokemon
	int desplazamiento = 0;

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, &caracteresPokemon, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, pokemon, caracteresPokemon);
	desplazamiento += caracteresPokemon;

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, &posX, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, &posY, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	t_mensaje_guardado* mensaje_nuevo = malloc(sizeof(t_mensaje_guardado));
	mensaje_nuevo = guardar_mensaje_en_memoria(bloque_a_agregar_en_memoria, tamanio_buffer_sin_id_mensaje);

	// Prepararacion y envio de mensaje a suscriptores

	//memcpy(bloque_a_agregar_en_memoria + desplazamiento, &id_mensaje_correlativo, sizeof(uint32_t));

	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = 2;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = tamanio_buffer_sin_id_mensaje;
	paquete->buffer->stream = bloque_a_agregar_en_memoria;

	int tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);
	void* a_enviar = serializar_paquete(paquete, tamanio_paquete);
	reenviar_mensaje_a_suscriptores(a_enviar, tamanio_paquete, suscriptores_appeared_pokemon);

	// Guardo información del mensaje

	guardar_mensaje_en_cola(2, mensajes_de_cola_appeared_pokemon, mensaje_nuevo, tamanio_buffer, NULL);

	free(bloque_a_agregar_en_memoria);
}

void recibir_catch_pokemon(int socket_cliente){

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

	// Creacion de bloque a guardar en memoria

	void* bloque_a_agregar_en_memoria = malloc(tamanio_buffer); // no cuenta el \n del nombre del Pokemon
	int desplazamiento = 0;

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, &caracteresPokemon, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, pokemon, caracteresPokemon);
	desplazamiento += caracteresPokemon;

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, &posX, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, &posY, sizeof(uint32_t));

	t_mensaje_guardado* mensaje_nuevo = malloc(sizeof(t_mensaje_guardado));
	mensaje_nuevo = guardar_mensaje_en_memoria(bloque_a_agregar_en_memoria, tamanio_buffer);

	// Prepararacion y envio de mensaje a suscriptores ---> Una vez que chequeo que se manda, hacerlo con todos

	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = 3;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = tamanio_buffer;
	paquete->buffer->stream = bloque_a_agregar_en_memoria;

	int tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);
	void* a_enviar = serializar_paquete(paquete, tamanio_paquete);
	reenviar_mensaje_a_suscriptores(a_enviar, tamanio_paquete, suscriptores_catch_pokemon);

	// Guardo información del mensaje

	guardar_mensaje_en_cola(3, mensajes_de_cola_catch_pokemon, mensaje_nuevo, tamanio_buffer, NULL);

	free(bloque_a_agregar_en_memoria);
}

void recibir_caught_pokemon(int socket_cliente){

	uint32_t tamanio_buffer;
	recv(socket_cliente, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

	uint32_t id_mensaje_correlativo;
	recv(socket_cliente, &id_mensaje_correlativo, sizeof(uint32_t), MSG_WAITALL);

	uint32_t se_pudo_atrapar;
	recv(socket_cliente, &se_pudo_atrapar, sizeof(uint32_t), MSG_WAITALL);

	if(se_pudo_atrapar == 0){
		se_pudo_atrapar = -1; // Para que no haya quilombo al momento de leer la memoria
	}

	// Creacion de bloque a guardar en memoria

	int desplazamiento = 0;

	uint32_t tamanio_buffer_sin_id_mensaje = tamanio_buffer-sizeof(uint32_t);
	void* bloque_a_agregar_en_memoria = malloc(tamanio_buffer_sin_id_mensaje); // no cuenta el \n del nombre del Pokemon

	memcpy(bloque_a_agregar_en_memoria, &se_pudo_atrapar, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	t_mensaje_guardado* mensaje_nuevo = malloc(sizeof(t_mensaje_guardado));
	mensaje_nuevo = guardar_mensaje_en_memoria(bloque_a_agregar_en_memoria, tamanio_buffer_sin_id_mensaje);

	// Prepararacion y envio de mensaje a suscriptores

	memcpy(bloque_a_agregar_en_memoria, &id_mensaje_correlativo, sizeof(uint32_t));

	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = 4;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = tamanio_buffer;
	paquete->buffer->stream = bloque_a_agregar_en_memoria;

	int tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);
	void* a_enviar = serializar_paquete(paquete, tamanio_paquete);
	reenviar_mensaje_a_suscriptores(a_enviar, tamanio_paquete, suscriptores_caught_pokemon);

	// Guardo información del mensaje

	guardar_mensaje_en_cola(4, mensajes_de_cola_caught_pokemon, mensaje_nuevo, tamanio_buffer, id_mensaje_correlativo);

	free(bloque_a_agregar_en_memoria);
}

void recibir_get_pokemon(int socket_cliente){

	uint32_t tamanio_buffer;
	recv(socket_cliente, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

	uint32_t caracteresPokemon;
	recv(socket_cliente, &caracteresPokemon, sizeof(uint32_t), MSG_WAITALL);

	char* pokemon = (char*)malloc(caracteresPokemon);
	recv(socket_cliente, pokemon, caracteresPokemon, MSG_WAITALL);

	// Creacion de bloque a guardar en memoria

	void* bloque_a_agregar_en_memoria = malloc(tamanio_buffer);
	int desplazamiento = 0;

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, &caracteresPokemon, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, pokemon, caracteresPokemon);

	t_mensaje_guardado* mensaje_nuevo = malloc(sizeof(t_mensaje_guardado));
	mensaje_nuevo = guardar_mensaje_en_memoria(bloque_a_agregar_en_memoria, tamanio_buffer);

	// Prepararacion y envio de mensaje a suscriptores

	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = 5;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = tamanio_buffer;
	paquete->buffer->stream = bloque_a_agregar_en_memoria;

	int tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);
	void* a_enviar = serializar_paquete(paquete, tamanio_paquete);
	reenviar_mensaje_a_suscriptores(a_enviar, tamanio_paquete, suscriptores_get_pokemon);

	// Guardo información del mensaje

	guardar_mensaje_en_cola(5, mensajes_de_cola_get_pokemon, mensaje_nuevo, tamanio_buffer, NULL);

	free(bloque_a_agregar_en_memoria);
}

void recibir_localized_pokemon(int socket_cliente){

	uint32_t tamanio_buffer;
	recv(socket_cliente, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

	uint32_t caracteresPokemon;
	recv(socket_cliente, &caracteresPokemon, sizeof(uint32_t), MSG_WAITALL);

	char* pokemon = (char*)malloc(caracteresPokemon);
	recv(socket_cliente, pokemon, caracteresPokemon, MSG_WAITALL);

	uint32_t cantidad_de_posiciones;
	recv(socket_cliente, &cantidad_de_posiciones, sizeof(uint32_t), MSG_WAITALL);

	// Creacion de bloque a guardar en memoria

	void* bloque_a_agregar_en_memoria = malloc(tamanio_buffer); // no cuenta el \n del nombre del Pokemon
	int desplazamiento = 0;

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, &caracteresPokemon, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, pokemon, caracteresPokemon);
	desplazamiento += caracteresPokemon;

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, &cantidad_de_posiciones, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	for(int i=0; i<cantidad_de_posiciones; i++){

		uint32_t posX;
		recv(socket_cliente, &posX, sizeof(uint32_t), MSG_WAITALL);

			memcpy(bloque_a_agregar_en_memoria, &posX, sizeof(uint32_t));
			desplazamiento += sizeof(uint32_t);

		uint32_t posY;
		recv(socket_cliente, &posY, sizeof(uint32_t), MSG_WAITALL);

			memcpy(bloque_a_agregar_en_memoria, &posY, sizeof(uint32_t));
			desplazamiento += sizeof(uint32_t);
	}

	guardar_mensaje_en_memoria(bloque_a_agregar_en_memoria, tamanio_buffer);

	free(bloque_a_agregar_en_memoria);
}

void guardar_mensaje_en_cola(op_code cod_op, t_list* lista_mensajes, t_mensaje_guardado* mensaje_en_memoria, uint32_t tamanio_buffer, uint32_t id_mensaje_correlativo){

	t_mensaje_a_guardar* nuevo_mensaje = malloc(sizeof(t_mensaje_a_guardar));
	nuevo_mensaje->codigo_operacion = cod_op;
	nuevo_mensaje->identificador = 1; // tengo q ver como hago esto
	nuevo_mensaje->suscriptores_ack = suscriptores_new_pokemon; // tengo q ver como hago esto - tecnicamente tiene q empezar vacia y llenarse a medida q llegan los ack e ir comparando si es igual a la total, elimino el mensaje de memoria ?¿
	nuevo_mensaje->tamanio_buffer = tamanio_buffer;
	nuevo_mensaje->ubicacion_mensaje = mensaje_en_memoria; // Guardo su posicion en memoria y el tamaño que ocupa, si lo quiero lo busco ahi
	nuevo_mensaje->id_mensaje_correlativo = id_mensaje_correlativo;

	list_add(lista_mensajes, nuevo_mensaje);
	//free?
}

void reenviar_mensaje_a_suscriptores(void* a_enviar, int tamanio_paquete, t_list* suscriptores){

	int tamanio_lista_de_suscriptores = list_size(suscriptores);

	for(int i = 0; i < tamanio_lista_de_suscriptores; i++){

		proceso* suscriptor = list_get(suscriptores, i);
		int socket_suscriptor = suscriptor->socket_cliente;

		char* mensa = string_from_format("El socket al que le vamos a enviar es: %d.", socket_suscriptor);
		completar_logger(mensa, "Broker", LOG_LEVEL_INFO);

		if(send(socket_suscriptor,a_enviar,tamanio_paquete,0) == -1){
			completar_logger("Error en enviar por el socket","BROKER", LOG_LEVEL_INFO);
			exit(3);

		char* mensaje = string_from_format("Se envió un mensaje al suscriptor de socket: %d.", socket_suscriptor);
		completar_logger(mensaje, "Broker", LOG_LEVEL_INFO);

		}
	}
}

/*
/////////////////////////// bardo //////////////////////////////////////////////////

// Recibir mensaje y reenviarlo a suscriptores

void enviar_mensaje_a_suscriptores(int cola_mensaje,int socket_cliente){

	t_paquete* paquete = malloc(sizeof(t_paquete));
	t_list* suscriptores_cola_mensaje;
	t_list* cola_de_mensajes;

	switch(cola_mensaje){
	case 1:
		paquete->codigo_operacion = 1;
		suscriptores_cola_mensaje = suscriptores_new_pokemon;
		cola_de_mensajes = mensajes_de_cola_new_pokemon;
		break;
	case 2:
		paquete->codigo_operacion = 2;
		suscriptores_cola_mensaje = suscriptores_appeared_pokemon;
		cola_de_mensajes = mensajes_de_cola_appeared_pokemon;
		break;
	case 3:
		paquete->codigo_operacion = 3;
		suscriptores_cola_mensaje = suscriptores_catch_pokemon;
		cola_de_mensajes = mensajes_de_cola_catch_pokemon;
		break;
	case 4:
		paquete->codigo_operacion = 4;
		suscriptores_cola_mensaje = suscriptores_caught_pokemon;
		cola_de_mensajes = mensajes_de_cola_caught_pokemon;
		break;
	case 5:
		paquete->codigo_operacion = 5;
		suscriptores_cola_mensaje = suscriptores_get_pokemon;
		cola_de_mensajes = mensajes_de_cola_get_pokemon;
		break;
	case 6:
		paquete->codigo_operacion = 6;
		suscriptores_cola_mensaje = suscriptores_localized_pokemon;
		cola_de_mensajes = mensajes_de_cola_localized_pokemon;
		break;
	}

	uint32_t size;
	paquete->buffer = malloc(sizeof(t_buffer));
	recv(socket_cliente, &size, sizeof(uint32_t), MSG_WAITALL);
	paquete->buffer->size = size;
	paquete->buffer->stream = malloc(paquete->buffer->size);

	void* payload = malloc(paquete->buffer->size);
	recv(socket_cliente, payload, paquete->buffer->size, MSG_WAITALL);
	memcpy(paquete->buffer->stream, payload, paquete->buffer->size);

	paquete->identificador = mensaje_id;
	enviar_mensaje_id(socket_cliente, paquete->identificador);
	mensaje_id ++; //semaforos

						// TAMANIO PAYLOAD + OP_CODE + SIZE + IDENTIFICADOR DEL MENSAJE
	int tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);//+sizeof(uint32_t);

	void* a_enviar = serializar_paquete(paquete, tamanio_paquete);

	int sizelista = list_size(suscriptores_cola_mensaje);

		//logs para chequear
		char* mensaje0 = string_from_format("El op code es: %d.", paquete->codigo_operacion);
		completar_logger(mensaje0, "Broker", LOG_LEVEL_INFO);

		char* mensaje2 = string_from_format("El tamanio del buffer es: %d.", size);
		completar_logger(mensaje2, "Broker", LOG_LEVEL_INFO);

		char* mensaje3 = string_from_format("paquete->buffer->size: %d.", paquete->buffer->size);
		completar_logger(mensaje3, "Broker", LOG_LEVEL_INFO);

		char* mensaje4 = string_from_format("El tamanio de la list es: %d.", sizelista);
		completar_logger(mensaje4, "Broker", LOG_LEVEL_INFO);

		char* mensaje5 = string_from_format("El socket del GameBoy ahora es: %d.", socket_cliente);
		completar_logger(mensaje5, "Broker", LOG_LEVEL_INFO);

	for(int i = 0; i < sizelista; i++){

		proceso* suscriptor = list_get(suscriptores_cola_mensaje, i);
		int socket_suscriptor = suscriptor->socket_cliente;

			char* mensa = string_from_format("El socket al que le vamos a enviar es es: %d.", socket_suscriptor);
			completar_logger(mensa, "Broker", LOG_LEVEL_INFO);

		if(send(socket_cliente,a_enviar,tamanio_paquete,0) == -1){ //socket_suscriptor
			completar_logger("Error en enviar por el socket","BROKER", LOG_LEVEL_INFO);
			exit(3);
		}
	}

	guardar_mensaje(cola_de_mensajes, paquete->codigo_operacion, suscriptores_cola_mensaje);

	completar_logger("Paquete enviado a los suscriptores de la cola.", "Broker", LOG_LEVEL_INFO); //LOG OBLIGATORIO

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
	free(payload);
}
/*
void guardar_mensaje(t_list* cola_de_mensajes, op_code codigo_operacion, t_list* suscriptores){

	t_mensaje* mensaje = malloc(sizeof(t_mensaje));
	mensaje->identificador = mensaje_id;
	mensaje->codigo_operacion = codigo_operacion;
	mensaje->suscriptores = suscriptores;

	list_add(cola_de_mensajes, mensaje);

	free(mensaje);
}
*/

void recibir_ack(int socket_cliente){
//	uint32_t tamanio_buffer;
//	recv(socket_cliente, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

	uint32_t largo_mensaje;
	recv(socket_cliente, &largo_mensaje, sizeof(uint32_t), MSG_WAITALL);

	char* ack = (char*)malloc(largo_mensaje);
	recv(socket_cliente, ack, largo_mensaje, MSG_WAITALL);

	puts(ack);

	char* loggearACK = string_from_format("El suscriptor de socket %d recibió el mensaje.", socket_cliente);
	completar_logger(loggearACK,"BROKER", LOG_LEVEL_INFO); // LOG OBLIGATORIO
/*
	op_code cod_op;
	recv(socket_cliente, &cod_op, sizeof(op_code), MSG_WAITALL);

	uint32_t id_mensaje;
	recv(socket_cliente, &id_mensaje, sizeof(uint32_t), MSG_WAITALL);
*/
	/*
	t_list* cola_de_mensajes;

	switch(cod_op){
		case 1:
			cola_de_mensajes = mensajes_de_cola_new_pokemon;
			break;
		case 2:
			cola_de_mensajes = mensajes_de_cola_appeared_pokemon;
			break;
		case 3:
			cola_de_mensajes = mensajes_de_cola_catch_pokemon;
			break;
		case 4:
			cola_de_mensajes = mensajes_de_cola_caught_pokemon;
			break;
		case 5:
			cola_de_mensajes = mensajes_de_cola_get_pokemon;
			break;
		case 6:
			cola_de_mensajes = mensajes_de_cola_localized_pokemon;
			break;
		case 7:
			break;
		}

	int tamanio_lista = list_size(cola_de_mensajes);
	for(int i = 0; i < tamanio_lista; i++){
		if(list_get(cola_de_mensajes, i)->identificador == id_mensaje){
			list_add(cola_de_mensajes->suscriptores_ack, socket_cliente);
		}
	}
*/
}

void* serializar_paquete(t_paquete* paquete, int bytes)
{
	void* a_enviar = malloc(bytes);
	int offset = 0;

	memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(op_code));
	offset += sizeof(op_code);

	memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(int));
	offset +=sizeof(int);

	memcpy(a_enviar + offset, paquete->buffer->stream, paquete->buffer->size);

	return a_enviar;
}


