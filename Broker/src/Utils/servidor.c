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
	//sem_wait(&MUTEX_MENSAJE);
	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	pthread_create(&thread,NULL,(void*)serve_client,&socket_cliente);
	pthread_detach(thread);
}

void serve_client(int* socket_proceso)
{
	log_conexion(*socket_proceso);

	op_code cod_op;
	if(recv(*socket_proceso, &cod_op, sizeof(op_code), MSG_WAITALL) == -1){
		pthread_exit(NULL);
	}

	process_request(cod_op, *socket_proceso);
}

// ATENDER AL CLIENTE

void process_request(op_code cod_op, int socket_cliente) {

	if(cod_op != 0){
		log_mensaje_nuevo();
	}

	switch (cod_op) {
		case 0:
			pthread_create(&hilo_suscripcion, NULL , atender_suscripcion ,socket_cliente);
			pthread_detach(hilo_suscripcion);
			enviar_mensaje(socket_cliente, "ACK: Suscripto.");
			break;
		case 1:
			pthread_create(&hilo_newPokemon, NULL , recibir_new_pokemon ,socket_cliente);
			pthread_detach(hilo_newPokemon);
			break;
		case 2:
			pthread_create(&hilo_appearedPokemon, NULL, recibir_appeared_pokemon, socket_cliente);
			pthread_detach(hilo_appearedPokemon);
			break;
		case 3:
			pthread_create(&hilo_catchPokemon, NULL, recibir_catch_pokemon, socket_cliente);
			pthread_detach(hilo_catchPokemon);
			break;
		case 4:
			pthread_create(&hilo_caughtPokemon, NULL, recibir_caught_pokemon, socket_cliente);
			pthread_detach(hilo_caughtPokemon);
			break;
		case 5:
			pthread_create(&hilo_getPokemon, NULL, recibir_get_pokemon, socket_cliente);
			pthread_detach(hilo_getPokemon);
			break;
		case 6:
			pthread_create(&hilo_localizedPokemon, NULL, recibir_localized_pokemon, socket_cliente);
			pthread_detach(hilo_localizedPokemon);
			break;
		case 7:
			pthread_create(&hilo_ack, NULL, recibir_ack, socket_cliente);
			pthread_detach(hilo_ack);
			break;
	}
	//sem_post(&MUTEX_MENSAJE);
}


// ATENDER SUSCRIPCION

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


void suscribirse_a_cola(proceso* suscriptor, int socket, uint32_t tamanio_buffer){

	uint32_t cola_a_suscribirse;
	recv(socket, &cola_a_suscribirse, sizeof(uint32_t), MSG_WAITALL);

	switch(cola_a_suscribirse){
		case 0:
			break;
		case 1:
			list_add(suscriptores_new_pokemon, suscriptor);
			log_suscripcion_nueva(suscriptor->socket_cliente, suscriptor->id, 1);
			enviar_mensajes_al_nuevo_suscriptor_NP(mensajes_de_cola_new_pokemon, suscriptor->socket_cliente);
			break;
		case 2:
			list_add(suscriptores_appeared_pokemon, suscriptor);
			log_suscripcion_nueva(suscriptor->socket_cliente, suscriptor->id, 2);
			enviar_mensajes_al_nuevo_suscriptor_AP(mensajes_de_cola_appeared_pokemon, suscriptor->socket_cliente);
			break;
		case 3:
			list_add(suscriptores_catch_pokemon, suscriptor);
			log_suscripcion_nueva(suscriptor->socket_cliente, suscriptor->id, 3);
			enviar_mensajes_al_nuevo_suscriptor_CATP(mensajes_de_cola_catch_pokemon, suscriptor->socket_cliente);
			break;
		case 4:
			list_add(suscriptores_caught_pokemon, suscriptor);
			log_suscripcion_nueva(suscriptor->socket_cliente, suscriptor->id, 4);
			enviar_mensajes_al_nuevo_suscriptor_CAUP(mensajes_de_cola_caught_pokemon, suscriptor->socket_cliente);
			break;
		case 5:
			list_add(suscriptores_get_pokemon, suscriptor);
			log_suscripcion_nueva(suscriptor->socket_cliente, suscriptor->id, 5);
			enviar_mensajes_al_nuevo_suscriptor_GP(mensajes_de_cola_get_pokemon, suscriptor->socket_cliente);
			break;
		case 6:
			list_add(suscriptores_localized_pokemon, suscriptor);
			log_suscripcion_nueva(suscriptor->socket_cliente, suscriptor->id, 6);
			//enviar_mensajes_al_nuevo_suscriptor(mensajes_de_cola_localized_pokemon, suscriptor->socket_cliente);
			break;
	}

	if(tamanio_buffer == 12){ // Entonces la suscripción es del GameBoy

		uint32_t tiempo_de_suscripcion;
		recv(socket, &tiempo_de_suscripcion, sizeof(uint32_t), MSG_WAITALL);

		t_suscripcion* suscripcion = malloc(sizeof(t_suscripcion));
		suscripcion->cola = cola_a_suscribirse;
		suscripcion->socket_cliente = suscriptor->socket_cliente;
		suscripcion->tiempo = tiempo_de_suscripcion;

		pthread_t hilo_recibir;
		pthread_create(&hilo_recibir, NULL , correr_tiempo_suscripcion , suscripcion);
		pthread_join(hilo_recibir, NULL);

		pthread_mutex_lock(&mutex_suscripcion);
	}
}

void correr_tiempo_suscripcion(t_suscripcion* suscripcion){
	sleep(suscripcion->tiempo);
	proceso* suscriptor;
	int index = -1;

	switch(suscripcion->cola){
		case 1:
			index = encontrar_suscriptor_por_posicion(suscripcion->socket_cliente, suscriptores_new_pokemon);
			suscriptor = list_remove(suscriptores_new_pokemon, index);
			break;
		case 2:
			index = encontrar_suscriptor_por_posicion(suscripcion->socket_cliente, suscriptores_appeared_pokemon);
			suscriptor = list_remove(suscriptores_appeared_pokemon, index);
			break;
		case 3:
			index = encontrar_suscriptor_por_posicion(suscripcion->socket_cliente, suscriptores_catch_pokemon);
			suscriptor = list_remove(suscriptores_catch_pokemon, index);
			break;
		case 4:
			index = encontrar_suscriptor_por_posicion(suscripcion->socket_cliente, suscriptores_caught_pokemon);
			suscriptor = list_remove(suscriptores_caught_pokemon, index);
			break;
		case 5:
			index = encontrar_suscriptor_por_posicion(suscripcion->socket_cliente, suscriptores_get_pokemon);
			suscriptor = list_remove(suscriptores_get_pokemon, index);
			break;
		case 6:
			index = encontrar_suscriptor_por_posicion(suscripcion->socket_cliente, suscriptores_localized_pokemon);
			suscriptor = list_remove(suscriptores_localized_pokemon, index);
			break;
		}

	free(suscriptor);
	free(suscripcion);

	pthread_mutex_unlock(&mutex_suscripcion);

}

int encontrar_suscriptor_por_posicion(int socket_cliente, t_list* lista){
	proceso* suscriptor;
	for(int i = 0; i<list_size(lista); i++){
		suscriptor = list_get(lista, i);
		if(suscriptor->socket_cliente == socket_cliente){
			return i;
		}
	}
	return -1;
}

//--------------------------------------------------------------------

void enviar_mensajes_al_nuevo_suscriptor_NP(t_list* mensajes_de_dicha_cola, int socket_suscriptor){
	int tamanio_lista = list_size(mensajes_de_dicha_cola);
	t_mensaje_en_cola* mensaje_a_enviar;

	for(int i = 0; i<tamanio_lista; i++){

		mensaje_a_enviar = list_get(mensajes_de_dicha_cola, i);

		sem_wait(&MUTEX_TIMESTAMP);
		timestamp++;
		mensaje_a_enviar->ubicacion_mensaje->ultima_referencia = timestamp; // Aumento el timestamp
		sem_post(&MUTEX_TIMESTAMP);

		char* log = string_from_format("Con timestamp %d.", timestamp);
		completar_logger(log, "BROKER", LOG_LEVEL_INFO);

		t_paquete* paquete = malloc(sizeof(t_paquete));
		paquete->codigo_operacion = mensaje_a_enviar->ubicacion_mensaje->cola;
		paquete->buffer = malloc(sizeof(t_buffer));
		paquete->buffer->size = mensaje_a_enviar->tamanio_buffer;

		void* stream = malloc(paquete->buffer->size);
		int offset_stream = 0;
		int inicio_mensaje = mensaje_a_enviar->ubicacion_mensaje->byte_comienzo_ocupado;
		int offset_memoria = inicio_mensaje;
		int tamanio_mensaje = mensaje_a_enviar->ubicacion_mensaje->tamanio_ocupado;

		uint32_t caracteres;
		memcpy(&caracteres, memoria_principal + offset_memoria, sizeof(uint32_t));
		memcpy(stream + offset_stream, &caracteres, sizeof(uint32_t));
		offset_stream += sizeof(uint32_t);
		offset_memoria += sizeof(uint32_t);

		char* pokemon = malloc(caracteres);
		pokemon = mensaje_a_enviar->pokemon;
		memcpy(stream + offset_stream, pokemon, caracteres);
		offset_stream += caracteres;
		offset_memoria += (caracteres-1);

		uint32_t posx;
		memcpy(&posx, memoria_principal + offset_memoria, sizeof(uint32_t));
		memcpy(stream + offset_stream, &posx, sizeof(uint32_t));
		offset_stream += sizeof(uint32_t);
		offset_memoria += sizeof(uint32_t);

		uint32_t posy;
		memcpy(&posy, memoria_principal + offset_memoria, sizeof(uint32_t));
		memcpy(stream + offset_stream, &posy, sizeof(uint32_t));
		offset_stream += sizeof(uint32_t);
		offset_memoria += sizeof(uint32_t);

		uint32_t cantidad;
		memcpy(&cantidad, memoria_principal + offset_memoria, sizeof(uint32_t));
		memcpy(stream + offset_stream, &cantidad, sizeof(uint32_t));
		offset_stream += sizeof(uint32_t);
		offset_memoria += sizeof(uint32_t);

		paquete->buffer->stream = stream; //mensaje_a_enviar->contenido_del_mensaje;
		int tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);
		void* a_enviar = serializar_paquete(paquete, tamanio_paquete);

		if(send(socket_suscriptor,a_enviar,tamanio_paquete,0) == -1){
			completar_logger("Error en enviar por el socket","BROKER", LOG_LEVEL_INFO);
			exit(3);
		}

		log_envio_mensaje(socket_suscriptor);

		free(paquete);
		free(paquete->buffer);
		free(stream);
		free(pokemon);
	}
}

void enviar_mensajes_al_nuevo_suscriptor_AP(t_list* mensajes_de_dicha_cola, int socket_suscriptor){
	int tamanio_lista = list_size(mensajes_de_dicha_cola);
	t_mensaje_en_cola* mensaje_a_enviar;

	for(int i = 0; i<tamanio_lista; i++){

		mensaje_a_enviar = list_get(mensajes_de_dicha_cola, i);

		sem_wait(&MUTEX_TIMESTAMP);
		timestamp++;
		mensaje_a_enviar->ubicacion_mensaje->ultima_referencia = timestamp; // Aumento el timestamp
		sem_post(&MUTEX_TIMESTAMP);

		char* log = string_from_format("Con timestamp %d.", timestamp);
		completar_logger(log, "BROKER", LOG_LEVEL_INFO);

		t_paquete* paquete = malloc(sizeof(t_paquete));
		paquete->codigo_operacion = mensaje_a_enviar->ubicacion_mensaje->cola;
		paquete->buffer = malloc(sizeof(t_buffer));
		paquete->buffer->size = mensaje_a_enviar->tamanio_buffer;

		void* stream = malloc(paquete->buffer->size);
		int offset_stream = 0;
		int inicio_mensaje = mensaje_a_enviar->ubicacion_mensaje->byte_comienzo_ocupado;
		int offset_memoria = inicio_mensaje;
		int tamanio_mensaje = mensaje_a_enviar->ubicacion_mensaje->tamanio_ocupado;

		uint32_t caracteres;
		memcpy(&caracteres, memoria_principal + offset_memoria, sizeof(uint32_t));
		memcpy(stream + offset_stream, &caracteres, sizeof(uint32_t));
		offset_stream += sizeof(uint32_t);
		offset_memoria += sizeof(uint32_t);

		char* pokemon = malloc(caracteres);
		pokemon = mensaje_a_enviar->pokemon;
		memcpy(stream + offset_stream, pokemon, caracteres);
		offset_stream += caracteres;
		offset_memoria += (caracteres-1);

		uint32_t posx;
		memcpy(&posx, memoria_principal + offset_memoria, sizeof(uint32_t));
		memcpy(stream + offset_stream, &posx, sizeof(uint32_t));
		offset_stream += sizeof(uint32_t);
		offset_memoria += sizeof(uint32_t);

		uint32_t posy;
		memcpy(&posy, memoria_principal + offset_memoria, sizeof(uint32_t));
		memcpy(stream + offset_stream, &posy, sizeof(uint32_t));
		offset_stream += sizeof(uint32_t);
		offset_memoria += sizeof(uint32_t);

		paquete->buffer->stream = stream; //mensaje_a_enviar->contenido_del_mensaje;
		int tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);
		void* a_enviar = serializar_paquete(paquete, tamanio_paquete);

		if(send(socket_suscriptor,a_enviar,tamanio_paquete,0) == -1){
			completar_logger("Error en enviar por el socket","BROKER", LOG_LEVEL_INFO);
			exit(3);
		}

		log_envio_mensaje(socket_suscriptor);


		free(paquete);
		free(paquete->buffer);
		free(stream);
		free(pokemon);
	}
}

void enviar_mensajes_al_nuevo_suscriptor_CATP(t_list* mensajes_de_dicha_cola, int socket_suscriptor){
	int tamanio_lista = list_size(mensajes_de_dicha_cola);
	t_mensaje_en_cola* mensaje_a_enviar;

	for(int i = 0; i<tamanio_lista; i++){

		mensaje_a_enviar = list_get(mensajes_de_dicha_cola, i);

		sem_wait(&MUTEX_TIMESTAMP);
		timestamp++;
		mensaje_a_enviar->ubicacion_mensaje->ultima_referencia = timestamp; // Aumento el timestamp
		sem_post(&MUTEX_TIMESTAMP);

		char* log = string_from_format("Con timestamp %d.", timestamp);
		completar_logger(log, "BROKER", LOG_LEVEL_INFO);

		t_paquete* paquete = malloc(sizeof(t_paquete));
		paquete->codigo_operacion = mensaje_a_enviar->ubicacion_mensaje->cola;
		paquete->buffer = malloc(sizeof(t_buffer));
		paquete->buffer->size = mensaje_a_enviar->tamanio_buffer;

		void* stream = malloc(paquete->buffer->size);
		int offset_stream = 0;
		int inicio_mensaje = mensaje_a_enviar->ubicacion_mensaje->byte_comienzo_ocupado;
		int offset_memoria = inicio_mensaje;
		int tamanio_mensaje = mensaje_a_enviar->ubicacion_mensaje->tamanio_ocupado;

		uint32_t caracteres;
		memcpy(&caracteres, memoria_principal + offset_memoria, sizeof(uint32_t));
		memcpy(stream + offset_stream, &caracteres, sizeof(uint32_t));
		offset_stream += sizeof(uint32_t);
		offset_memoria += sizeof(uint32_t);

		char* pokemon = malloc(caracteres);
		pokemon = mensaje_a_enviar->pokemon;
		memcpy(stream + offset_stream, pokemon, caracteres);
		offset_stream += caracteres;
		offset_memoria += (caracteres-1);

		uint32_t posx;
		memcpy(&posx, memoria_principal + offset_memoria, sizeof(uint32_t));
		memcpy(stream + offset_stream, &posx, sizeof(uint32_t));
		offset_stream += sizeof(uint32_t);
		offset_memoria += sizeof(uint32_t);

		uint32_t posy;
		memcpy(&posy, memoria_principal + offset_memoria, sizeof(uint32_t));
		memcpy(stream + offset_stream, &posy, sizeof(uint32_t));
		offset_stream += sizeof(uint32_t);
		offset_memoria += sizeof(uint32_t);

		paquete->buffer->stream = stream;

		int tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);
		void* a_enviar = serializar_paquete(paquete, tamanio_paquete);

		if(send(socket_suscriptor,a_enviar,tamanio_paquete,0) == -1){
			completar_logger("Error en enviar por el socket","BROKER", LOG_LEVEL_INFO);
			exit(3);
		}

		log_envio_mensaje(socket_suscriptor);

		free(paquete);
		free(paquete->buffer);
		free(stream);
		free(pokemon);
	}
}

void enviar_mensajes_al_nuevo_suscriptor_CAUP(t_list* mensajes_de_dicha_cola, int socket_suscriptor){

	int tamanio_lista = list_size(mensajes_de_dicha_cola);
	t_mensaje_en_cola* mensaje_a_enviar;

	for(int i = 0; i<tamanio_lista; i++){

		mensaje_a_enviar = list_get(mensajes_de_dicha_cola, i);

		sem_wait(&MUTEX_TIMESTAMP);
		timestamp++;
		mensaje_a_enviar->ubicacion_mensaje->ultima_referencia = timestamp; // Aumento el timestamp
		sem_post(&MUTEX_TIMESTAMP);

		char* log = string_from_format("Con timestamp %d.", timestamp);
		completar_logger(log, "BROKER", LOG_LEVEL_INFO);

		t_paquete* paquete = malloc(sizeof(t_paquete));
		paquete->codigo_operacion = mensaje_a_enviar->ubicacion_mensaje->cola;
		paquete->buffer = malloc(sizeof(t_buffer));
		paquete->buffer->size = mensaje_a_enviar->tamanio_buffer;

		void* stream = malloc(paquete->buffer->size);
		int inicio_mensaje = mensaje_a_enviar->ubicacion_mensaje->byte_comienzo_ocupado;
		int tamanio_mensaje = mensaje_a_enviar->ubicacion_mensaje->tamanio_ocupado;

		memcpy(stream, memoria_principal + inicio_mensaje, tamanio_mensaje);
		memcpy(stream + tamanio_mensaje, &(mensaje_a_enviar->id_mensaje_correlativo), sizeof(uint32_t));

		paquete->buffer->stream = stream; //mensaje_a_enviar->contenido_del_mensaje;

		int tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);
		void* a_enviar = serializar_paquete(paquete, tamanio_paquete);

		if(send(socket_suscriptor,a_enviar,tamanio_paquete,0) == -1){
			completar_logger("Error en enviar por el socket","BROKER", LOG_LEVEL_INFO);
			exit(3);
		}

		log_envio_mensaje(socket_suscriptor);

		free(paquete);
		free(paquete->buffer);
		free(stream);
	}
}

void enviar_mensajes_al_nuevo_suscriptor_GP(t_list* mensajes_de_dicha_cola, int socket_suscriptor){
	int tamanio_lista = list_size(mensajes_de_dicha_cola);
	t_mensaje_en_cola* mensaje_a_enviar;

	for(int i = 0; i<tamanio_lista; i++){

		mensaje_a_enviar = list_get(mensajes_de_dicha_cola, i);

		sem_wait(&MUTEX_TIMESTAMP);
		timestamp++;
		mensaje_a_enviar->ubicacion_mensaje->ultima_referencia = timestamp; // Aumento el timestamp
		sem_post(&MUTEX_TIMESTAMP);

		char* log = string_from_format("Con timestamp %d.", timestamp);
		completar_logger(log, "BROKER", LOG_LEVEL_INFO);

		t_paquete* paquete = malloc(sizeof(t_paquete));
		paquete->codigo_operacion = mensaje_a_enviar->ubicacion_mensaje->cola;
		paquete->buffer = malloc(sizeof(t_buffer));
		paquete->buffer->size = mensaje_a_enviar->tamanio_buffer;

		void* stream = malloc(paquete->buffer->size);
		int offset_stream = 0;
		int inicio_mensaje = mensaje_a_enviar->ubicacion_mensaje->byte_comienzo_ocupado;
		int offset_memoria = inicio_mensaje;
		int tamanio_mensaje = mensaje_a_enviar->ubicacion_mensaje->tamanio_ocupado;

		uint32_t caracteres;
		memcpy(&caracteres, memoria_principal + offset_memoria, sizeof(uint32_t));
		memcpy(stream + offset_stream, &caracteres, sizeof(uint32_t));
		offset_stream += sizeof(uint32_t);
		offset_memoria += sizeof(uint32_t);

		char* pokemon = malloc(caracteres);
		pokemon = mensaje_a_enviar->pokemon;
		memcpy(stream + offset_stream, pokemon, caracteres);

		paquete->buffer->stream = stream;

		int tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);
		void* a_enviar = serializar_paquete(paquete, tamanio_paquete);

		if(send(socket_suscriptor,a_enviar,tamanio_paquete,0) == -1){
			completar_logger("Error en enviar por el socket","BROKER", LOG_LEVEL_INFO);
			exit(3);
		}

		log_envio_mensaje(socket_suscriptor);

		free(paquete);
		free(paquete->buffer);
		free(stream);
		free(pokemon);
	}
}

// RECEPCION, ALMACENAMIENTO EN MEMORIA Y REENVIO DE MENSAJES

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

	int buffer_sin_barra_n = tamanio_buffer - 1;

	void* bloque_a_agregar_en_memoria = malloc(buffer_sin_barra_n); // no cuenta el \n del nombre del Pokemon
	int desplazamiento = 0;

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, &caracteresPokemon, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, pokemon, caracteresPokemon-1);
	desplazamiento += caracteresPokemon-1;

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, &posX, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, &posY, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, &cantidad, sizeof(uint32_t));

	t_mensaje_guardado* mensaje_nuevo;
	mensaje_nuevo = guardar_mensaje_en_memoria(bloque_a_agregar_en_memoria, buffer_sin_barra_n, 1);

	// ENVIAR ID

	// Prepararacion y envio de mensaje a suscriptores ---> Una vez que chequeo que se manda, hacerlo con todos

	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = 1;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = tamanio_buffer;

	void* bloque_con_barra_n = malloc(tamanio_buffer);
	int desplazamiento_con_barra_n = 0;

	memcpy(bloque_con_barra_n + desplazamiento_con_barra_n, &caracteresPokemon, sizeof(uint32_t));
	desplazamiento_con_barra_n += sizeof(uint32_t);

	memcpy(bloque_con_barra_n + desplazamiento_con_barra_n, pokemon, caracteresPokemon);
	desplazamiento_con_barra_n += caracteresPokemon;

	memcpy(bloque_con_barra_n + desplazamiento_con_barra_n, &posX, sizeof(uint32_t));
	desplazamiento_con_barra_n += sizeof(uint32_t);

	memcpy(bloque_con_barra_n + desplazamiento_con_barra_n, &posY, sizeof(uint32_t));
	desplazamiento_con_barra_n += sizeof(uint32_t);

	memcpy(bloque_con_barra_n + desplazamiento_con_barra_n, &cantidad, sizeof(uint32_t));

	paquete->buffer->stream = bloque_con_barra_n;

	int tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);
	void* a_enviar = serializar_paquete(paquete, tamanio_paquete);

	reenviar_mensaje_a_suscriptores(a_enviar, tamanio_paquete, suscriptores_new_pokemon);

	// Guardo información del mensaje

	guardar_mensaje_en_cola(mensajes_de_cola_new_pokemon, mensaje_nuevo, tamanio_buffer, NULL, pokemon, suscriptores_new_pokemon);

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

	uint32_t tamanio_buffer_sin_barra_n_ni_id = tamanio_buffer-sizeof(uint32_t)-1;
	void* bloque_a_agregar_en_memoria = malloc(tamanio_buffer_sin_barra_n_ni_id); // no cuenta el \n del nombre del Pokemon
	int desplazamiento = 0;

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, &caracteresPokemon, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, pokemon, caracteresPokemon-1);
	desplazamiento += caracteresPokemon-1;

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, &posX, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, &posY, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	t_mensaje_guardado* mensaje_nuevo;
	mensaje_nuevo = guardar_mensaje_en_memoria(bloque_a_agregar_en_memoria, tamanio_buffer_sin_barra_n_ni_id, 2);

	// Prepararacion y envio de mensaje a suscriptores

	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = 2;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = tamanio_buffer;

	void* bloque_con_barra_n = malloc(tamanio_buffer);
	int desplazamiento_con_barra_n = 0;

	memcpy(bloque_con_barra_n + desplazamiento_con_barra_n, &caracteresPokemon, sizeof(uint32_t));
	desplazamiento_con_barra_n += sizeof(uint32_t);

	memcpy(bloque_con_barra_n + desplazamiento_con_barra_n, pokemon, caracteresPokemon);
	desplazamiento_con_barra_n += caracteresPokemon;

	memcpy(bloque_con_barra_n + desplazamiento_con_barra_n, &posX, sizeof(uint32_t));
	desplazamiento_con_barra_n += sizeof(uint32_t);

	memcpy(bloque_con_barra_n + desplazamiento_con_barra_n, &posY, sizeof(uint32_t));

	paquete->buffer->stream = bloque_con_barra_n;

	int tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);
	void* a_enviar = serializar_paquete(paquete, tamanio_paquete);
	reenviar_mensaje_a_suscriptores(a_enviar, tamanio_paquete, suscriptores_appeared_pokemon);

	// Guardo información del mensaje

	int tamanio_buffer_sin_id = tamanio_buffer_sin_barra_n_ni_id + 1;

	guardar_mensaje_en_cola(mensajes_de_cola_appeared_pokemon, mensaje_nuevo, tamanio_buffer_sin_id, NULL, pokemon, suscriptores_appeared_pokemon);

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

	int buffer_sin_barra_n = tamanio_buffer - 1;
	void* bloque_a_agregar_en_memoria = malloc(buffer_sin_barra_n); // no cuenta el \n del nombre del Pokemon
	int desplazamiento = 0;

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, &caracteresPokemon, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, pokemon, caracteresPokemon-1);
	desplazamiento += caracteresPokemon-1;

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, &posX, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, &posY, sizeof(uint32_t));

	t_mensaje_guardado* mensaje_nuevo;
	mensaje_nuevo = guardar_mensaje_en_memoria(bloque_a_agregar_en_memoria, buffer_sin_barra_n, 3);

	// Prepararacion y envio de mensaje a suscriptores ---> Una vez que chequeo que se manda, hacerlo con todos

	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = 3;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = tamanio_buffer;

	void* bloque_con_barra_n = malloc(tamanio_buffer);
	int desplazamiento_con_barra_n = 0;

	memcpy(bloque_con_barra_n + desplazamiento_con_barra_n, &caracteresPokemon, sizeof(uint32_t));
	desplazamiento_con_barra_n += sizeof(uint32_t);

	memcpy(bloque_con_barra_n + desplazamiento_con_barra_n, pokemon, caracteresPokemon);
	desplazamiento_con_barra_n += caracteresPokemon;

	memcpy(bloque_con_barra_n + desplazamiento_con_barra_n, &posX, sizeof(uint32_t));
	desplazamiento_con_barra_n += sizeof(uint32_t);

	memcpy(bloque_con_barra_n + desplazamiento_con_barra_n, &posY, sizeof(uint32_t));

	paquete->buffer->stream = bloque_con_barra_n;

	int tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);
	void* a_enviar = serializar_paquete(paquete, tamanio_paquete);
	reenviar_mensaje_a_suscriptores(a_enviar, tamanio_paquete, suscriptores_catch_pokemon);

	// Guardo información del mensaje

	guardar_mensaje_en_cola(mensajes_de_cola_catch_pokemon, mensaje_nuevo, tamanio_buffer, NULL, pokemon, suscriptores_catch_pokemon);

	free(bloque_a_agregar_en_memoria);
}

void recibir_caught_pokemon(int socket_cliente){

	uint32_t tamanio_buffer;
	recv(socket_cliente, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

	uint32_t id_mensaje_correlativo;
	recv(socket_cliente, &id_mensaje_correlativo, sizeof(uint32_t), MSG_WAITALL);

	uint32_t se_pudo_atrapar;
	recv(socket_cliente, &se_pudo_atrapar, sizeof(uint32_t), MSG_WAITALL);

	// Creacion de bloque a guardar en memoria

	int desplazamiento = 0;

	uint32_t tamanio_buffer_sin_id_mensaje = tamanio_buffer-sizeof(uint32_t);
	void* bloque_a_agregar_en_memoria = malloc(tamanio_buffer_sin_id_mensaje); // no cuenta el \n del nombre del Pokemon

	memcpy(bloque_a_agregar_en_memoria, &se_pudo_atrapar, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	t_mensaje_guardado* mensaje_nuevo;
	mensaje_nuevo = guardar_mensaje_en_memoria(bloque_a_agregar_en_memoria, tamanio_buffer_sin_id_mensaje, 4);

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

	guardar_mensaje_en_cola(mensajes_de_cola_caught_pokemon, mensaje_nuevo, tamanio_buffer, id_mensaje_correlativo, NULL, suscriptores_caught_pokemon);

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

	int buffer_sin_barra_n = tamanio_buffer - 1;
	void* bloque_a_agregar_en_memoria = malloc(buffer_sin_barra_n);
	int desplazamiento = 0;

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, &caracteresPokemon, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, pokemon, caracteresPokemon-1);

	t_mensaje_guardado* mensaje_nuevo;
	mensaje_nuevo = guardar_mensaje_en_memoria(bloque_a_agregar_en_memoria, buffer_sin_barra_n, 5);

	// Prepararacion y envio de mensaje a suscriptores

	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = 5;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = tamanio_buffer;

	void* bloque_con_barra_n = malloc(tamanio_buffer);
	int desplazamiento_con_barra_n = 0;

	memcpy(bloque_con_barra_n + desplazamiento_con_barra_n, &caracteresPokemon, sizeof(uint32_t));
	desplazamiento_con_barra_n += sizeof(uint32_t);

	memcpy(bloque_con_barra_n + desplazamiento_con_barra_n, pokemon, caracteresPokemon);

	int tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);
	void* a_enviar = serializar_paquete(paquete, tamanio_paquete);
	reenviar_mensaje_a_suscriptores(a_enviar, tamanio_paquete, suscriptores_get_pokemon);

	// Guardo información del mensaje

	guardar_mensaje_en_cola(mensajes_de_cola_get_pokemon, mensaje_nuevo, tamanio_buffer, NULL, pokemon, suscriptores_get_pokemon);

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

	int buffer_sin_barra_n = tamanio_buffer - 1;
	void* bloque_a_agregar_en_memoria = malloc(buffer_sin_barra_n); // no cuenta el \n del nombre del Pokemon
	int desplazamiento = 0;

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, &caracteresPokemon, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, pokemon, caracteresPokemon-1);
	desplazamiento += caracteresPokemon-1;

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, &cantidad_de_posiciones, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	int tam_posxposy = cantidad_de_posiciones*8;
	void* posxposy = malloc(tam_posxposy);
	int desp_posxposy = 0;

	for(int i=0; i<cantidad_de_posiciones; i++){

		uint32_t posX;
		recv(socket_cliente, &posX, sizeof(uint32_t), MSG_WAITALL);

			memcpy(posxposy + desp_posxposy, &posX, sizeof(uint32_t));
			desp_posxposy += sizeof(uint32_t);

		uint32_t posY;
		recv(socket_cliente, &posY, sizeof(uint32_t), MSG_WAITALL);

			memcpy(posxposy + desp_posxposy, &posY, sizeof(uint32_t));
			desp_posxposy += sizeof(uint32_t);
	}

	memcpy(bloque_a_agregar_en_memoria + desplazamiento, posxposy, tam_posxposy);

	t_mensaje_guardado* mensaje_nuevo;
	mensaje_nuevo = guardar_mensaje_en_memoria(bloque_a_agregar_en_memoria, buffer_sin_barra_n, 6);

	// Prepararacion y envio de mensaje a suscriptores ---> Una vez que chequeo que se manda, hacerlo con todos

	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = 1;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = tamanio_buffer;

	void* bloque_con_barra_n = malloc(tamanio_buffer);
	int desplazamiento_con_barra_n = 0;

	memcpy(bloque_con_barra_n + desplazamiento_con_barra_n, &caracteresPokemon, sizeof(uint32_t));
	desplazamiento_con_barra_n += sizeof(uint32_t);

	memcpy(bloque_con_barra_n + desplazamiento_con_barra_n, pokemon, caracteresPokemon);
	desplazamiento_con_barra_n += caracteresPokemon;

	memcpy(bloque_con_barra_n + desplazamiento_con_barra_n, &cantidad_de_posiciones, sizeof(uint32_t));
	desplazamiento_con_barra_n += sizeof(uint32_t);

	memcpy(bloque_con_barra_n + desplazamiento_con_barra_n, posxposy, tam_posxposy);

	paquete->buffer->stream = bloque_con_barra_n;

	int tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);
	void* a_enviar = serializar_paquete(paquete, tamanio_paquete);

	reenviar_mensaje_a_suscriptores(a_enviar, tamanio_paquete, suscriptores_localized_pokemon);

	// Guardo información del mensaje

	guardar_mensaje_en_cola(mensajes_de_cola_localized_pokemon, mensaje_nuevo, tamanio_buffer, NULL, pokemon, suscriptores_localized_pokemon);

	free(bloque_a_agregar_en_memoria);
	free(posxposy);
	free(paquete);
	free(paquete->buffer);
}

void guardar_mensaje_en_cola(t_list* lista_mensajes, t_mensaje_guardado* mensaje_en_memoria, uint32_t tamanio_buffer, uint32_t id_mensaje_correlativo, char* pokemon, t_list* lista_de_suscriptores){

	t_mensaje_en_cola* nuevo_mensaje = malloc(sizeof(t_mensaje_en_cola));
	nuevo_mensaje->suscriptores_ack = lista_de_suscriptores;
	nuevo_mensaje->tamanio_buffer = tamanio_buffer;
	nuevo_mensaje->ubicacion_mensaje = mensaje_en_memoria;
	nuevo_mensaje->id_mensaje_correlativo = id_mensaje_correlativo;
	nuevo_mensaje->pokemon = pokemon;

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
		}

		log_envio_mensaje(socket_suscriptor);
	}
}

void recibir_ack(int socket_cliente){

	uint32_t tamanio_buffer;
	recv(socket_cliente, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

	char* ack = malloc(tamanio_buffer);
	recv(socket_cliente, ack, tamanio_buffer, MSG_WAITALL);

	uint32_t mensaje_id;
	// Habría que recibir el mensaje id

	log_confirmacion(socket_cliente, mensaje_id);

	// ahora hay que buscar a que mensaje de que cola pertenece ese mensaje id, y eliminar al suscriptor de la lista de ack

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

// AUXILIAR

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

int recibir_tamanio_buffer(int socket){
	uint32_t tamanio_buffer;
	recv(socket, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);
	return tamanio_buffer;
}

// ENVIAR MENSAJE

void enviar_mensaje(int socket_cliente, char* mensaje){
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = 7;
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
	paquete->codigo_operacion = 7;
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

