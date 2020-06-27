#include"conexiones.h"
#include"configTeam.h"

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
		completar_logger(mensaje, "TEAM", LOG_LEVEL_INFO);
	return socket_cliente;
}

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


	completar_logger("Se conectó proceso al Team", "TEAM", LOG_LEVEL_INFO); // LOG OBLIGATORIO

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
	op_code cod_op;
	if(recv(*socket, &cod_op, sizeof(op_code), MSG_WAITALL) == -1)
		pthread_exit(NULL);

	char* mensaje = string_from_format("El codigo de operacion es: %d.", cod_op);
	completar_logger(mensaje, "TEAM", LOG_LEVEL_INFO);

	process_request(cod_op, *socket);
}

// ATENDER AL CLIENTE

void process_request(op_code cod_op, int socket_cliente) {

	if(cod_op != 0){
		completar_logger("Llegó un nuevo mensaje a la cola de mensajes", "TEAM", LOG_LEVEL_INFO); // LOG OBLIGATORIO
	}

	switch(cod_op) {
		case 2:
			recibir_AppearedPokemon(socket_cliente);
			//enviar_mensaje_a_suscriptores(2, socket_cliente);
			break;

		case 4:
			recibir_CaughtPokemon(socket_cliente);
			//enviar_mensaje_a_suscriptores(4, socket_cliente);
			break;

		case 6:
			recibir_LocalizedPokemon(socket_cliente);
			//enviar_mensaje_a_suscriptores(6, socket_cliente);
			break;
	}
}


/*void* recibir_mensaje(int socket_cliente, int* size)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}*/

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

void enviar_suscripcion_a_cola(op_code cola)
{

	char* puerto_broker = obtener_puerto();
	char* ip_broker = obtener_ip();
	int socket_broker;
	int tiempo_reconexion = obtener_tiempo_reconexion();

	socket_broker = crear_conexion(ip_broker,puerto_broker);

	/*while(socket_broker == -1){
		sleep(tiempo_reconexion);
		socket_broker = crear_conexion(ip_broker,puerto_broker);
	}*/

	int tamanio_paquete = 0;
	void* a_enviar;
	puts("aca entra1.5");

	a_enviar = suscribirse_a_cola(socket_broker, cola, &tamanio_paquete);

	send(socket_broker,a_enviar,tamanio_paquete,0);

	free(a_enviar);

	char* mensaje_subscripcion = recibir_mensaje(socket_broker);

	puts(mensaje_subscripcion);

	//pthread_mutex_unlock(&mutex_conexion);

}

//recibir id de catch

char* recibir_mensaje(int socket_cliente){

	op_code code_op;
	recv(socket_cliente, &code_op, sizeof(op_code), MSG_WAITALL);

	uint32_t tamanio_buffer;
	recv(socket_cliente, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

	char* mensaje = malloc(tamanio_buffer);
	recv(socket_cliente, mensaje, tamanio_buffer, MSG_WAITALL);

	sem_post(&MUTEX_SUB);

	return mensaje;
}

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

	int socket_broker = crear_conexion(ip_broker,puerto_broker);

	if(socket_broker == -1){
		entrenador->pudo_atrapar_pokemon = 1;
		pthread_mutex_unlock(&mutex_entrenador);
	}

	else{

		int tamanio_paquete = 0;
		void* a_enviar;

		a_enviar = iniciar_paquete_serializado_CatchPokemon(&tamanio_paquete, entrenador);

		if(send(socket_broker,a_enviar,tamanio_paquete,0) == -1){
			printf("Error en enviar por el socket");
			exit(3);
		}
		free(a_enviar);
	}

}

void enviar_GetPokemon_a_broker(op_code codigo_operacion, char* pokemon)
{
	char* puerto_broker = obtener_puerto();
	char* ip_broker = obtener_ip();

	int socket_broker = crear_conexion(ip_broker,puerto_broker);

	if(socket_broker == -1){
		//comportamiento default (NO HAY POKEMONES EN EL MAPAs)
	}
	else{
		int tamanio_paquete = 0;
		void* a_enviar;

		a_enviar = iniciar_paquete_serializado_GetPokemon(&tamanio_paquete, pokemon);

		if(send(socket_broker,a_enviar,tamanio_paquete,0) == -1){
			printf("Error en enviar por el socket");
			exit(3);
		}

		free(a_enviar);
	}
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

		t_pokemon* pokemonNuevo = armarPokemon(pokemon, posX, posY);

		if(es_pokemon_requerido(pokemonNuevo)){
			list_add(lista_de_pokemones_sueltos, pokemonNuevo);


			pthread_mutex_unlock(&mutex_planificador);
		}

		free(pokemonNuevo);

}
/*
void recibir_appeared_pokemon_loggeo(void){

	char* puerto_broker = obtener_puerto();
	char* ip_broker = obtener_ip();

	int socket_cliente = crear_conexion(ip_broker,puerto_broker);

	completar_logger("recibo el pokemon", "TEAM", LOG_LEVEL_INFO);

	uint32_t tamanio_buffer;
	recv(socket_cliente, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

		char* m1 = string_from_format("tamaño buffer: %d.", tamanio_buffer);
		completar_logger(m1, "TEAM", LOG_LEVEL_INFO);

	uint32_t tamanio_leido= 0;

	uint32_t caracteresPokemon;
	recv(socket_cliente, &caracteresPokemon, sizeof(uint32_t), MSG_WAITALL);
	tamanio_leido += sizeof(uint32_t);
			//log
		char* m2 = string_from_format("Los caracteres de pokemon son: %d.", caracteresPokemon);
		completar_logger(m2, "TEAM", LOG_LEVEL_INFO);

	char* pokemon = (char*)malloc(caracteresPokemon);
	recv(socket_cliente, pokemon, caracteresPokemon, MSG_WAITALL);
	tamanio_leido += caracteresPokemon;
			//log
		completar_logger(pokemon, "TEAM", LOG_LEVEL_INFO);

	uint32_t posX;
	recv(socket_cliente, &posX, sizeof(uint32_t), MSG_WAITALL);
	tamanio_leido += sizeof(uint32_t);
		//log
		char* m3 = string_from_format("La posicion en X es: %d", posX);
		completar_logger(m3, "TEAM", LOG_LEVEL_INFO);

	uint32_t posY;
	recv(socket_cliente, &posY, sizeof(uint32_t), MSG_WAITALL);
	tamanio_leido += sizeof(uint32_t);
		//log
		char* m4 = string_from_format("La posicion en Y es: %d", posY);
		completar_logger(m4, "TEAM", LOG_LEVEL_INFO);

	if((tamanio_buffer - tamanio_leido) != 0){
		uint32_t id_correlativo;
		recv(socket_cliente, &id_correlativo, sizeof(uint32_t), MSG_WAITALL);
			//log
			char* m5 = string_from_format("El id correlativo es: %d", id_correlativo);
			completar_logger(m5, "TEAM", LOG_LEVEL_INFO);
	}
		t_pokemon* pokemonNuevo = armarPokemon(pokemon, posX, posY);

		if(es_pokemon_requerido(pokemonNuevo)){
			list_add(lista_de_pokemones_sueltos, pokemonNuevo);


			pthread_mutex_unlock(&mutex_planificador);
		}

		free(pokemonNuevo);
		//armar pokemon con los datos recibidos y mandar el pokemon armado a APARACION_POKEMON()

}*/

t_pokemon* armarPokemon(char* pokemon, int posX, int posY){
	t_pokemon* pokeNuevo = malloc(sizeof(t_pokemon));
	pokeNuevo->especie = pokemon;
	pokeNuevo->posicion.x= posX;
	pokeNuevo->posicion.y= posY;

	char* pokemonNombre = string_from_format("El nombre del pokemon es: %s", pokeNuevo->especie);
					completar_logger(pokemonNombre, "TEAM", LOG_LEVEL_INFO);
	char* xx = string_from_format("posicion en x: %d",pokeNuevo->posicion.x);
					completar_logger(xx, "TEAM", LOG_LEVEL_INFO);
	char* yy = string_from_format("posicion en y: %d", pokeNuevo->posicion.y);
					completar_logger(yy, "TEAM", LOG_LEVEL_INFO);

	return pokeNuevo;
}


void recibir_CaughtPokemon(int socket_cliente){

		uint32_t tamanio_buffer;
		recv(socket_cliente, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

		uint32_t id_correlativo;
		recv(socket_cliente, &id_correlativo, sizeof(uint32_t), MSG_WAITALL);

		uint32_t pudoAtraparlo;
		recv(socket_cliente, &pudoAtraparlo, sizeof(uint32_t), MSG_WAITALL);

		//Falta seguir a partir de aca

		t_entrenador* entrenador = buscar_entrenador_por_id_catch(id_correlativo);

		if(entrenador != NULL){
			entrenador->pudo_atrapar_pokemon = pudoAtraparlo;
			pthread_mutex_unlock(&mutex_entrenador);
		}

}


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

// "Pikachu" 3 4 5 6 3 5 7

void recibir_LocalizedPokemon(int socket_cliente){

		t_pokemon* pokemon_nuevo = malloc(sizeof(t_pokemon));

		uint32_t tamanio_buffer;
		recv(socket_cliente, &tamanio_buffer, sizeof(uint32_t), MSG_WAITALL);

		uint32_t caracteresPokemon;
		recv(socket_cliente, &caracteresPokemon, sizeof(uint32_t), MSG_WAITALL);

		char* pokemon = (char*)malloc(caracteresPokemon);
		recv(socket_cliente, pokemon, caracteresPokemon, MSG_WAITALL);

		uint32_t cantidadPokemones;
		recv(socket_cliente, &cantidadPokemones, sizeof(uint32_t), MSG_WAITALL);

		for(int i =0; i<cantidadPokemones; i++){
			uint32_t posX;
			recv(socket_cliente, &posX, sizeof(uint32_t), MSG_WAITALL);

			uint32_t posY;
			recv(socket_cliente, &posY, sizeof(uint32_t), MSG_WAITALL);

			pokemon_nuevo = armarPokemon(pokemon, posX, posY);//esto no funca todavia pero la logica seria asi
			list_add(lista_de_pokemones_sueltos,pokemon_nuevo);
		}

		pthread_mutex_unlock(&mutex_planificador);
}


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

		//pthread_mutex_unlock(hilo_planificador);
}

//----------------------- RECEPCION DE MENSAJES DE GAMEBOY -----------------------


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


t_entrenador* buscar_entrenador_por_id_catch(uint32_t id){
	t_entrenador* entrenador = NULL;
	t_entrenador* entrenador_aux;
	for(int i = 0; i < cantidad_entrenadores(); i++){
		entrenador_aux = list_get(lista_de_entrenadores, i);
		if(entrenador->ID_catch_pokemon == id){
			entrenador = entrenador_aux;
		}
	}

	return entrenador;

}
