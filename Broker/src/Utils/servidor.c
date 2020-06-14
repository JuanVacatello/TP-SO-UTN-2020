#include "servidor.h"

#include <pthread.h>

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

// CONEXION CON CLIENTE

void iniciar_servidor(void)
{
	iniciar_logger();
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

	completar_logger("Se conectó proceso al Broker", "BROKER", LOG_LEVEL_INFO); // LOG OBLIGATORIO

    freeaddrinfo(servinfo);

    while(1)
    	esperar_cliente(socket_servidor); // hacerlo con select, NUNCA ESPERA ACTIVA
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
	op_code cod_op;
	if(recv(*socket, &cod_op, sizeof(op_code), MSG_WAITALL) == -1)
		pthread_exit(NULL);

	char* mensaje = string_from_format("El codigo de operacion es: %d.", cod_op);
	completar_logger(mensaje, "Broker", LOG_LEVEL_INFO);

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
			enviar_mensaje(socket_cliente, "Suscripto.");
			break;
		case 1:
			//recibir_new_pokemon_loggeo(socket_cliente);
			//a_enviar = recibir_new_pokemon(socket_cliente,&tamanio_paquete);

			enviar_mensaje_a_suscriptores(1, socket_cliente);
			break;
		case 2:
			//recibir_appeared_pokemon_loggeo(socket_cliente);
			//a_enviar = recibir_appeared_pokemon(socket_cliente,&tamanio_paquete);
			completar_logger("En el switch de appeared pokemon", "Broker", LOG_LEVEL_INFO);
			enviar_mensaje_a_suscriptores(2, socket_cliente);
			break;
		case 3:
			//recibir_catch_pokemon_log(socket_cliente);
			//a_enviar = recibir_catch_pokemon_(socket_cliente,&tamanio_paquete);
			enviar_mensaje_a_suscriptores(3, socket_cliente);
			break;
		case 4:
			//recibir_caught_pokemon_loggeo(socket_cliente);
			//a_enviar = recibir_caught_pokemon(socket_cliente,&tamanio_paquete);
			enviar_mensaje_a_suscriptores(4, socket_cliente);
			break;
		case 5:
			//recibir_get_pokemon_loggeo(socket_cliente);
			//a_enviar = recibir_get_pokemon(socket_cliente,&tamanio_paquete);
			enviar_mensaje_a_suscriptores(5, socket_cliente);
			break;
		case 6:
			// a_enviar = Mensaje que recibe del GameCard
			//enviar_mensaje_a_suscriptores(6, a_enviar, tamanio_paquete);
			break;
		case 7:
			break;
	}
}

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

	if(tamanio_buffer == 12){ // Entonces la suscripción es del GameBoy
		uint32_t tiempo_de_suscripcion; // no se que hacer con esto
		recv(socket, &tiempo_de_suscripcion, sizeof(uint32_t), MSG_WAITALL);

		char* m = string_from_format("El tiempo de suscripcion es: %d.", tiempo_de_suscripcion);
		completar_logger(m, "BROKER", LOG_LEVEL_INFO);
	}

	switch(cola_a_suscribirse){
		case 0:
			break;
		case 1:
			list_add(suscriptores_new_pokemon, suscriptor);
			break;
		case 2:
			list_add(suscriptores_appeared_pokemon, suscriptor);
			break;
		case 3:
			list_add(suscriptores_catch_pokemon, suscriptor);
			break;
		case 4:
			list_add(suscriptores_caught_pokemon, suscriptor);
			break;
		case 5:
			list_add(suscriptores_get_pokemon, suscriptor);
			break;
		case 6:
			list_add(suscriptores_localized_pokemon, suscriptor);
			break;
	}

	char* log = string_from_format("Se suscribió el proceso con id %d y con socket %d a la cola de mensajes %d", suscriptor->id, suscriptor->socket_cliente, cola_a_suscribirse);
	completar_logger(log, "BROKER", LOG_LEVEL_INFO); //LOG OBLIGATORIO

}



void enviar_mensaje_a_suscriptores(int cola_mensaje,int socket_cliente){

	completar_logger("Enviando mensaje a suscriptor", "Broker", LOG_LEVEL_INFO);
	t_paquete* paquete = malloc(sizeof(t_paquete));
	t_list* suscriptores_cola_mensaje;

	switch(cola_mensaje){
	case 1:
		paquete->codigo_operacion = NEW_POKEMON;
		suscriptores_cola_mensaje = suscriptores_new_pokemon;
		break;
	case 2:
		completar_logger("En el switch del op code", "Broker", LOG_LEVEL_INFO);
		paquete->codigo_operacion = APPEARED_POKEMON;
		suscriptores_cola_mensaje = suscriptores_appeared_pokemon;
		break;
	case 3:
		paquete->codigo_operacion = CATCH_POKEMON;
		suscriptores_cola_mensaje = suscriptores_catch_pokemon;
		break;
	case 4:
		paquete->codigo_operacion = CAUGHT_POKEMON;
		suscriptores_cola_mensaje = suscriptores_caught_pokemon;
		break;
	case 5:
		paquete->codigo_operacion = GET_POKEMON;
		suscriptores_cola_mensaje = suscriptores_get_pokemon;
		break;
	case 6:
		paquete->codigo_operacion = LOCALIZED_POKEMON;
		suscriptores_cola_mensaje = suscriptores_localized_pokemon;
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

	int tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);

	void* a_enviar = serializar_paquete(paquete, tamanio_paquete);

	int sizelista = list_size(suscriptores_cola_mensaje);

		//logs para chequear
		char* mensaje2 = string_from_format("El tamanio del buffer es: %d.", size);
		completar_logger(mensaje2, "Broker", LOG_LEVEL_INFO);

		char* mensaje3 = string_from_format("paquete->buffer->size: %d.", paquete->buffer->size);
		completar_logger(mensaje3, "Broker", LOG_LEVEL_INFO);

		char* mensaje = string_from_format("El tamanio de la list es: %d.", sizelista);
		completar_logger(mensaje, "Broker", LOG_LEVEL_INFO);

	for(int i = 0; i < sizelista; i++){

		proceso* suscriptor = list_get(suscriptores_cola_mensaje, i);
		int socket_suscriptor = suscriptor->socket_cliente;

		if(send(socket_cliente,a_enviar,tamanio_paquete,0) == -1){
				completar_logger("Error en enviar por el socket","BROKER", LOG_LEVEL_INFO);
				exit(3);
		}
	}

	completar_logger("Paquete enviado a los suscriptores de la cola.", "Broker", LOG_LEVEL_INFO); //LOG OBLIGATORIO

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
	free(payload);
}

// Por si necesita hacer algo además del pasamanos:

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

}

void recibir_catch_pokemon_loggeo(int socket_cliente){

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
}


void recibir_caught_pokemon(int socket_cliente){

	uint32_t tamanio_buffer;
	recv(socket_cliente, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

	uint32_t id_mensaje_correlativo;
	recv(socket_cliente, &id_mensaje_correlativo, sizeof(uint32_t), MSG_WAITALL);

	uint32_t se_pudo_atrapar;
	recv(socket_cliente, &se_pudo_atrapar, sizeof(uint32_t), MSG_WAITALL);
}

void recibir_get_pokemon(int socket_cliente){

	uint32_t tamanio_buffer;
	recv(socket_cliente, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

	uint32_t caracteresPokemon;
	recv(socket_cliente, &caracteresPokemon, sizeof(uint32_t), MSG_WAITALL);

	char* pokemon = (char*)malloc(caracteresPokemon);
	recv(socket_cliente, pokemon, caracteresPokemon, MSG_WAITALL);

}
