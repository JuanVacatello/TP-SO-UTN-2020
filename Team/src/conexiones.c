#include"conexiones.h"

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
		return -1;
	}

	freeaddrinfo(server_info);

	char* mensaje = string_from_format("el socket de conexion es: %d.", socket_cliente);
		completar_logger(mensaje, "TEAM", LOG_LEVEL_INFO);
	return socket_cliente;
}

void iniciar_espera_mensajes_Gameboy(void)
{
	int socket_servidor;
    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    char* ip = obtener_ip_team();
    char* puerto = obtener_puerto_team();

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

    while(1)

    	esperar_cliente(socket_servidor);
}

void esperar_cliente(int socket_servidor)
{
	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);
	sem_wait(&MUTEX_MENSAJES_GB);
	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	pthread_create(&thread,NULL,(void*)serve_client,&socket_cliente);
	pthread_detach(thread);
}

void serve_client(int* socket)
{
	op_code cod_op;
	if(recv(*socket, &cod_op, sizeof(op_code), MSG_WAITALL) == -1)
		pthread_exit(NULL);

	process_request(cod_op, *socket);
}

// ATENDER AL CLIENTE

void process_request(op_code cod_op, int socket_cliente) {

	switch(cod_op){
		case 2:
			recibir_AppearedPokemon(socket_cliente);
			break;
	}
	sem_post(&MUTEX_MENSAJES_GB);
}

void* serializar_paquete(t_paquete* paquete, int *bytes)
{
	void* a_enviar = malloc(*bytes);
	int offset = 0;

	memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(op_code));
	offset += sizeof(op_code);

	memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(int));
	offset +=sizeof(int);

	memcpy(a_enviar + offset, paquete->buffer->stream, paquete->buffer->size);

	return a_enviar;
}

//----------------------- COLAS A SUSCRIBIRSE -----------------------
void appeared_pokemon_broker(){

	int socket_broker = enviar_suscripcion_a_cola(APPEARED_POKEMON);
	//sem_post(&GET);
	while(1){

		op_code cod_op = 10;
		recv(socket_broker, &cod_op, sizeof(op_code), MSG_WAITALL);

		if(cod_op == APPEARED_POKEMON){

			recibir_AppearedPokemon(socket_broker);

		}
	}
}

void caught_pokemon_broker(){
	int socket_broker = enviar_suscripcion_a_cola(CAUGHT_POKEMON);
	//sem_post(&GET);
	while(1){

		op_code cod_op = 10;
		recv(socket_broker, &cod_op, sizeof(op_code), MSG_WAITALL);

		if(cod_op == CAUGHT_POKEMON){

			recibir_CaughtPokemon(socket_broker);

		}
	}
}

void localized_pokemon_broker(){
	int socket_broker = enviar_suscripcion_a_cola(LOCALIZED_POKEMON);
	//sem_post(&GET);
	while(1){

		op_code cod_op = 10;
		recv(socket_broker, &cod_op, sizeof(op_code), MSG_WAITALL);

		if(cod_op == LOCALIZED_POKEMON){

			recibir_LocalizedPokemon(socket_broker);

		}
	}
}

//----------------------- SUSCRIPCIONES -----------------------
int enviar_suscripcion_a_cola(op_code cola)
{

	char* puerto_broker = obtener_puerto();
	char* ip_broker = obtener_ip();

	int socket_broker = -1;
	int tiempo_reconexion = obtener_tiempo_reconexion();
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
	void* a_enviar = suscribirse_a_cola(socket_broker, cola, &tamanio_paquete);

	send(socket_broker,a_enviar,tamanio_paquete,0);

	char* mensaje_subscripcion = recibir_mensaje(socket_broker);

	puts(mensaje_subscripcion);


	free(a_enviar);

	return socket_broker;
}

void* suscribirse_a_cola(int socket_cliente, uint32_t cola, int* tamanio_paquete){

	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = SUSCRIPTOR;
	paquete->buffer = malloc(sizeof(t_buffer));

	uint32_t proccess_id = obtener_id_propio();

	paquete->buffer->size = sizeof(uint32_t)+sizeof(uint32_t);
	void* stream = malloc(paquete->buffer->size);
	int offset = 0;

		memcpy(stream + offset, &proccess_id, sizeof(uint32_t));
		offset += sizeof(uint32_t);

		memcpy(stream + offset, &cola, sizeof(uint32_t));


	paquete->buffer->stream = stream;

						// TAMAÑO STREAM + OP CODE + VARIABLE SIZE
	*tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);
	void* a_enviar = serializar_paquete(paquete,tamanio_paquete);

	free(paquete->buffer);
	free(stream);
	free(paquete);

	return a_enviar;
}

//----------------------- COMUNICACION CON BROKER -----------------------
void enviar_CatchPokemon_a_broker(op_code codigo_operacion, t_entrenador* entrenador )
{
	char* puerto_broker = obtener_puerto();
	char* ip_broker = obtener_ip();
	int id_correlativo = -1;

	int socket_broker = crear_conexion(ip_broker,puerto_broker);

	if(socket_broker == -1){
		entrenador->pudo_atrapar_pokemon = 1;
		entrenador->estado = EXEC;
		ciclosCpuTotales++;
		atrapar_pokemon(entrenador);
	}

	else{

		int tamanio_paquete = 0;
		void* a_enviar;

		a_enviar = iniciar_paquete_serializado_CatchPokemon(&tamanio_paquete, entrenador);

		if(send(socket_broker,a_enviar,tamanio_paquete,0) == -1){
			printf("Error en enviar por el socket");
			exit(3);
		}

		id_correlativo = recibir_id_correlativo(socket_broker);
		entrenador->ID_catch_pokemon = id_correlativo;

		ciclos_de_cpu(1);
		ciclosCpuTotales++;
		free(a_enviar);
	}

}

void enviar_GetPokemon_a_broker(op_code codigo_operacion, char* pokemon)
{
	char* puerto_broker = obtener_puerto();
	char* ip_broker = obtener_ip();
	int id_correlativo = -1;
	sem_wait(&GET);
	int socket_broker = crear_conexion(ip_broker,puerto_broker);

	if(socket_broker == -1){
		char* mensaje = string_from_format("No se encontraron pokemones de la especie %s en el mapa", pokemon);
		loguearMensaje(mensaje);
	}
	else{
		int tamanio_paquete = 0;
		void* a_enviar;

		a_enviar = iniciar_paquete_serializado_GetPokemon(&tamanio_paquete, pokemon);

		if(send(socket_broker,a_enviar,tamanio_paquete,0) == -1){
			printf("Error en enviar por el socket");
			exit(3);
		}
		pthread_t* hilo_id;
		pthread_create(&hilo_id, NULL, recibir_id_correlativo_get, socket_broker);
		pthread_detach(hilo_id);

		free(a_enviar);
	}

	ciclosCpuTotales++;

}


//////////////// CATCH POKEMON ////////////////

void* iniciar_paquete_serializado_CatchPokemon(int* tamanio_paquete,t_entrenador* entrenador){ //Deberia de recibir el entrenador pero no estoy seguro si como parametro o VG.

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = CATCH_POKEMON;

	paquete->buffer = malloc(sizeof(t_buffer));

	char* pokemon = entrenador->pokemon_a_atrapar->especie;
	uint32_t caracteresPokemon = strlen(pokemon) + 1;
	uint32_t posX = entrenador->pokemon_a_atrapar->posicion.x;
	uint32_t posY = entrenador->pokemon_a_atrapar->posicion.y;

						//INT CARACTERES + POKEMON + POSX + POSY
	paquete->buffer->size = sizeof(uint32_t) + caracteresPokemon + sizeof(uint32_t) + sizeof(uint32_t);
	void* stream = malloc(paquete->buffer->size);
	int offset = 0;

		memcpy(stream + offset, &caracteresPokemon, sizeof(uint32_t));
		offset += sizeof(uint32_t);

		memcpy(stream + offset, pokemon, caracteresPokemon);
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

void* iniciar_paquete_serializado_GetPokemon(int* tamanio_paquete,char* pokemon_pedido){

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = GET_POKEMON;

	paquete->buffer = malloc(sizeof(t_buffer));

	char* pokemon = pokemon_pedido;
	uint32_t caracteresPokemon = strlen(pokemon) + 1;

						//INT CARACTERES + POKEMON
	paquete->buffer->size = sizeof(uint32_t) + caracteresPokemon;
	void* stream = malloc(paquete->buffer->size);
	int offset = 0;

		memcpy(stream + offset, &caracteresPokemon, sizeof(uint32_t));
		offset += sizeof(uint32_t);

		memcpy(stream + offset, pokemon, caracteresPokemon);
		offset +=caracteresPokemon;

		paquete->buffer->stream = stream;

						// TAMAÑO STREAM + OP CODE + VARIABLE SIZE
	*tamanio_paquete = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);

	void* a_enviar = serializar_paquete(paquete, tamanio_paquete);
	/*
	int offsetDeSerializacion = 0;

		memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(op_code));
		offsetDeSerializacion += sizeof(op_code);

		memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(uint32_t));
		offsetDeSerializacion +=sizeof(uint32_t);

		memcpy(a_enviar + offset, &(paquete->buffer->stream), paquete->buffer->size);
	*/

	free(stream);
	free(paquete->buffer);
	free(paquete);

	return a_enviar;

}

//----------------------- RECEPCION DE MENSAJES -----------------------
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

int recibir_id_correlativo(int socket_broker){

	op_code code_op;
	recv(socket_broker, &code_op, sizeof(op_code), MSG_WAITALL);

	uint32_t tamanio_buffer;
	recv(socket_broker, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

	uint32_t id_correlativo;
	recv(socket_broker, &id_correlativo, tamanio_buffer, MSG_WAITALL);

	return id_correlativo;
}

void recibir_id_correlativo_get(int socket_broker){

	op_code code_op;
	recv(socket_broker, &code_op, sizeof(op_code), MSG_WAITALL);

	uint32_t tamanio_buffer;
	recv(socket_broker, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

	uint32_t id_correlativo;
	recv(socket_broker, &id_correlativo, tamanio_buffer, MSG_WAITALL);

	sem_wait(&ID);
	list_add(lista_ids_getPokemon, id_correlativo);
	sem_post(&ID);
}

//----------------------- RECEPCION DE MENSAJES DE GAMEBOY -----------------------


void recibir_AppearedPokemon(int socket_broker){

		uint32_t tamanio_buffer;
		recv(socket_broker, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

		uint32_t mensajeid;
		recv(socket_broker, &mensajeid, sizeof(uint32_t), MSG_WAITALL);

		uint32_t caracteresPokemon;
		recv(socket_broker, &caracteresPokemon, sizeof(uint32_t), MSG_WAITALL);

		char* pokemon = (char*)malloc(caracteresPokemon);
		recv(socket_broker, pokemon, caracteresPokemon, MSG_WAITALL);

		uint32_t posX;
		recv(socket_broker, &posX, sizeof(uint32_t), MSG_WAITALL);

		uint32_t posY;
		recv(socket_broker, &posY, sizeof(uint32_t), MSG_WAITALL);

		responder_ack(socket_broker,mensajeid);

		if(es_pokemon_requerido(pokemon)){
			sem_wait(&CONTADOR_ENTRENADORES);
			log_llego_mensaje_nuevo_appeared_pokemon(mensajeid,pokemon, posX, posY);
			t_pokemon* pokemonNuevo = armarPokemon(pokemon, posX, posY);
			aparicion_pokemon(pokemonNuevo);
			pthread_mutex_unlock(&mutex_planificador);
		}

}



void recibir_CaughtPokemon(int socket_broker){

		uint32_t tamanio_buffer;
		recv(socket_broker, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

		uint32_t mensajeid;
		recv(socket_broker, &mensajeid, sizeof(uint32_t), MSG_WAITALL);

		uint32_t id_correlativo;
		recv(socket_broker, &id_correlativo, sizeof(uint32_t), MSG_WAITALL);

		uint32_t pudoAtraparlo;
		recv(socket_broker, &pudoAtraparlo, sizeof(uint32_t), MSG_WAITALL);

		log_llego_mensaje_nuevo_caught_pokemon(mensajeid, id_correlativo, pudoAtraparlo);
		responder_ack(socket_broker,mensajeid);

		t_entrenador* entrenador = NULL;
		entrenador = buscar_entrenador_por_id_catch(id_correlativo);

		if(entrenador != NULL){
			entrenador->pudo_atrapar_pokemon = pudoAtraparlo;
			entrenador->estado = EXEC;
			atrapar_pokemon(entrenador);
		}


}

//	POKEMON	ID	 ID_CORRE	CANT	POSICIONES
// "Pikachu" 1  	10		 3   4 5 6 3 5 7

void recibir_LocalizedPokemon(int socket_broker){

		uint32_t tamanio_buffer;
		recv(socket_broker, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

		uint32_t mensajeid;
		recv(socket_broker, &mensajeid, sizeof(uint32_t), MSG_WAITALL);

		uint32_t id_correlativo;
		recv(socket_broker, &id_correlativo, sizeof(uint32_t), MSG_WAITALL);

		uint32_t caracteresPokemon;
		recv(socket_broker, &caracteresPokemon, sizeof(uint32_t), MSG_WAITALL);

		char* pokemon = (char*)malloc(caracteresPokemon);
		recv(socket_broker, pokemon, caracteresPokemon, MSG_WAITALL);

		uint32_t cantidadPokemones;
		recv(socket_broker, &cantidadPokemones, sizeof(uint32_t), MSG_WAITALL);

		t_list* posicionesX = list_create();
		t_list* posicionesY = list_create();

		for(int i =0; i<cantidadPokemones; i++){

			uint32_t posX;
			recv(socket_broker, &posX, sizeof(uint32_t), MSG_WAITALL);
			list_add(posicionesX, posX);

			uint32_t posY;
			recv(socket_broker, &posY, sizeof(uint32_t), MSG_WAITALL);
			list_add(posicionesY, posY);

		}

		log_llego_mensaje_nuevo_localized_pokemon(mensajeid, id_correlativo, pokemon, cantidadPokemones, posicionesX, posicionesY);
		responder_ack(socket_broker,mensajeid);

		if(existe_id_en_lista(id_correlativo)){

			for(int i =0; i<cantidadPokemones; i++){

				uint32_t posX;
				posX = list_get(posicionesX,i);
				uint32_t posY;
				posY = list_get(posicionesY,i);

				t_pokemon* pokemon_nuevo = malloc(sizeof(t_pokemon));

				if(es_pokemon_requerido(pokemon)){
					sem_wait(&CONTADOR_ENTRENADORES);
					//log_llego_mensaje_nuevo_localized_pokemon(mensajeid, id_correlativo, pokemon, cantidadPokemones, posicionesX, posicionesY);
					t_pokemon* pokemonNuevo = armarPokemon(pokemon, posX, posY);
					aparicion_pokemon(pokemonNuevo);
					pthread_mutex_unlock(&mutex_planificador);
				}
				else{
					t_pokemon* pokemonNuevo = armarPokemon(pokemon, posX, posY);
					list_add(lista_pokemonesNoRequeridos_enElMapa, pokemonNuevo);
				}
			}

		}
		else{
			free(pokemon);
		}

			list_destroy(posicionesX);
			list_destroy(posicionesY);

}


t_pokemon* armarPokemon(char* pokemon, int posX, int posY){
	t_pokemon* pokeNuevo = malloc(sizeof(t_pokemon));
	pokeNuevo->especie = pokemon;
	pokeNuevo->posicion.x= posX;
	pokeNuevo->posicion.y= posY;

	return pokeNuevo;
}

t_entrenador* buscar_entrenador_por_id_catch(uint32_t id){
	t_entrenador* entrenador = NULL;
	t_entrenador* entrenador_aux;
	for(int i = 0; i < cantidad_entrenadores(); i++){
		entrenador_aux = list_get(lista_de_entrenadores, i);
		if(entrenador_aux->ID_catch_pokemon == id){
			entrenador = entrenador_aux;
		}
	}
	return entrenador;
}

void* enviar_ACK(int socket_broker, int* tamanio, uint32_t mensaje_id){

	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = 7;
	paquete->buffer = malloc(sizeof(t_buffer));

	char* mensaje = "ACK";
	uint32_t caracteres_mensaje = strlen(mensaje) + 1;
	paquete->buffer->size = sizeof(uint32_t) + sizeof(uint32_t) + caracteres_mensaje + sizeof(uint32_t);
	void* stream = malloc(paquete->buffer->size);
	int offset = 0;

	memcpy(stream + offset, &caracteres_mensaje, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(stream + offset, mensaje, caracteres_mensaje);
	offset += caracteres_mensaje;

	memcpy(stream + offset, &mensaje_id, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	uint32_t id_team = obtener_id_propio();
	memcpy(stream + offset, &id_team, sizeof(uint32_t));

	paquete->buffer->stream = stream;

	*tamanio = (paquete->buffer->size)+sizeof(op_code)+sizeof(uint32_t);
	void* a_enviar = serializar_paquete(paquete,tamanio);

	return a_enviar;
}

void responder_ack(int socket_ack,uint32_t mensaje_id){

	int tamanio = 0;
	void* a_enviar = enviar_ACK(socket_ack, &tamanio, mensaje_id);
	send(socket_ack,a_enviar,tamanio,0);

	free(a_enviar);
}
