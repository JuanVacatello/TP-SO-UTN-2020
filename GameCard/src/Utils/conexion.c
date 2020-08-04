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

void* serializar_paquete(t_paquete* paquete, int *bytes){

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

	free(pokemon);

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

	free(pokemon);
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

	free(pokemon);
}

//

void new_pokemon(char* pokemon,int posX,int posY, int cantidad){ //funciona

	char* path_pokemon = obtener_path_pokemon(pokemon);
	char* path_files = obtener_path_files();

	if(existe_file(path_pokemon)==0){ // existe el file del pokemon

		while(archivo_pokemon_esta_abierto(path_pokemon) ==1){
			int tiempo_reintento_operacion = tiempo_de_reintento_operacion();
			sleep(tiempo_reintento_operacion);
		}
		abrir_archivo_pokemon(path_pokemon);

		char* bloques_en_string = obtener_bloques_pokemon_string(path_pokemon);
		if(strlen(bloques_en_string) == 2){ // si no tiene bloques asignados, asigno 1 y agrego linea

			bloques_en_string = asignar_primer_bloque();
			modificar_campo_bloques_metadata(path_pokemon,bloques_en_string);
			char* linea = generar_linea_a_insertar(posX, posY, 1);
			modificar_campo_size_metadata(path_pokemon,strlen(linea));
			almacenar_datos(linea, path_pokemon);

			int tiempo_retardo = tiempo_retardo_operacion();
			sleep(tiempo_retardo);
			cerrar_archivo_pokemon(path_pokemon);
			free(linea);
		}
		else{ // si tiene bloques asignados

			t_list* lista_datos = obtener_datos_bloques(path_pokemon);
			int indice = existe_posicion_en_lista(lista_datos,posX,posY);

			if(indice == -1){ // si no existe la posicion indice == -1
				char* linea = generar_linea_a_insertar(posX, posY, cantidad);
				agregar_linea(lista_datos, linea);
				char* datos = obtener_datos_en_string(lista_datos);

				modificar_campo_size_metadata(path_pokemon,strlen(datos));
				almacenar_datos(datos, path_pokemon);

				int tiempo_retardo = tiempo_retardo_operacion();
				sleep(tiempo_retardo);
				cerrar_archivo_pokemon(path_pokemon);
				//free(linea);
				free(datos);
				list_destroy(lista_datos);
			}

			else{ // si existe la posicion
				int flag_cambio_longitud = 0;
				char* linea_a_modificar = list_get(lista_datos, indice);
				char* linea_modificada = aumentar_cantidad_linea(linea_a_modificar,cantidad, &flag_cambio_longitud);
				list_replace(lista_datos,indice,linea_modificada);
				char* datos = obtener_datos_en_string(lista_datos);

				if(flag_cambio_longitud == 1 ){// si la longitud de la palabra cambio, actualizo el tamaño del pokemon
					modificar_campo_size_metadata(path_pokemon,strlen(datos));
				}

				almacenar_datos(datos, path_pokemon);

				int tiempo_retardo = tiempo_retardo_operacion();
				sleep(tiempo_retardo);
				free(datos);
				cerrar_archivo_pokemon(path_pokemon);

			}
		}
	}
	else{ // si no existe el pokemon en el filesystem
		mkdir(path_pokemon, 0777);
		char* path_metadata_pokemon = string_new();
		string_append(&path_metadata_pokemon,path_pokemon);
		string_append(&path_metadata_pokemon,"/");
		string_append(&path_metadata_pokemon,"Metadata.bin");

		FILE* metadata = txt_open_for_append(path_metadata_pokemon);

		txt_write_in_file(metadata, "DIRECTORY=N\n");
		txt_write_in_file(metadata, "SIZE=\n");

		int nuevo_bloque = obtener_nuevo_bloque();
		char* bloque_string = string_itoa(nuevo_bloque);

		txt_write_in_file(metadata, "BLOCKS=[");
		txt_write_in_file(metadata, bloque_string);
		txt_write_in_file(metadata, "]\n");
		txt_write_in_file(metadata, "OPEN=Y");
		txt_close_file(metadata);

		free(bloque_string);

		char* linea = generar_linea_a_insertar(posX, posY, cantidad);
		modificar_campo_size_metadata(path_pokemon,strlen(linea));
		char* bloques_string = obtener_bloques_pokemon_string(path_pokemon);
		almacenar_datos(linea, path_pokemon);

		int tiempo_retardo = tiempo_retardo_operacion();
		sleep(tiempo_retardo);
		cerrar_archivo_pokemon(path_pokemon);

		free(path_metadata_pokemon);
		free(linea);
	}

	free(path_pokemon);
}

int catch_pokemon(char* pokemon,int posX,int posY){

	char* path_pokemon = obtener_path_pokemon(pokemon);
	char* path_files = obtener_path_files();


	if(existe_file(path_pokemon)==0){// si existe el file pokemon

		while(archivo_pokemon_esta_abierto(path_pokemon) ==1){
			int tiempo_reintento_operacion = tiempo_de_reintento_operacion();
			sleep(tiempo_reintento_operacion);
		}
		abrir_archivo_pokemon(path_pokemon);

		char* bloques_en_string = obtener_bloques_pokemon_string(path_pokemon);

		if(strlen(bloques_en_string) == 2){ //si no tiene bloques asignados: "[]" // No hay retardo operacion ya que estos simulan el acceso a disco de los bloques
			cerrar_archivo_pokemon(path_pokemon);								  // y en este caso no se accede a bloques
			completar_logger("Error: Se trato de hacer un catch a un pokemon que no tiene bloques asignados","GAMECARD", LOG_LEVEL_INFO);
			free(path_pokemon);
			return -1;
		}
		else{ // si tiene bloques asignados

			t_list* lista_datos = obtener_datos_bloques(path_pokemon);
			int indice = existe_posicion_en_lista(lista_datos,posX,posY);

			if(indice == -1){ // si no existe la posicion
				int tiempo_retardo = tiempo_retardo_operacion();// chequear si va un retardo operacion, creo que si porque hay accesos a bloques, solo que no encuentra la pos buscada
				sleep(tiempo_retardo);

				cerrar_archivo_pokemon(path_pokemon);
				free(path_pokemon);
				completar_logger("Error: Se trato de hacer un catch a un pokemon en una posicion en la cual no está","GAMECARD", LOG_LEVEL_INFO);
				return -1;
			}
			else{ // si existe la posicion
				int flag_cambio_longitud = 0;
				char* linea_a_modificar = list_get(lista_datos, indice);
				char* linea_modificada = disminuir_cantidad_linea(linea_a_modificar, &flag_cambio_longitud);

				if(cantidad_igual_cero(linea_modificada) == 1){// si la linea modificada tiene como cantidad:0 debo eliminarla, tambien cambia el Size de mi file pokemon
					list_remove_and_destroy_element(lista_datos,indice, free);
					flag_cambio_longitud = 1;

					if(list_size(lista_datos) == 0){ // si ademas con aliminar esa linea la lista de pos queda vacia, libero y limpio los bloques asignados
						limpiar_bloques_pokemon(path_pokemon);
						liberar_bloques_pokemon(path_pokemon);
						modificar_campo_bloques_metadata(path_pokemon,"[]");
						modificar_campo_size_metadata(path_pokemon,0);

						int tiempo_retardo = tiempo_retardo_operacion();
						sleep(tiempo_retardo);
						cerrar_archivo_pokemon(path_pokemon);

						list_destroy(lista_datos);
						free(path_pokemon);
						free(linea_modificada);
						return 1;
					}
				}
				else{ // si con el catch la cantidad no se hace 0, acualizo el valor decrementado en 1
					list_replace(lista_datos,indice,linea_modificada);
				}

				char* datos = obtener_datos_en_string(lista_datos);

				if(flag_cambio_longitud == 1 ){// si la longitud de la linea cambio, actualizo el tamaño del pokemon
					modificar_campo_size_metadata(path_pokemon,strlen(datos));
				}

				almacenar_datos(datos, path_pokemon);

				int tiempo_retardo = tiempo_retardo_operacion();
				sleep(tiempo_retardo);

				cerrar_archivo_pokemon(path_pokemon);

				free(datos);
				list_destroy(lista_datos);
				free(path_pokemon);
				free(linea_modificada);

				return 1;
			}
		}
	}
	else{ //si no existe
		free(path_pokemon);
		completar_logger("Error: Se trato de hacer un catch a un pokemon inexistente en el filesystem","GAMECARD", LOG_LEVEL_INFO);

		return -1;
	}
}

void* get_pokemon(char* pokemon, uint32_t *tamanio_void){

	char* path_pokemon = obtener_path_pokemon(pokemon);
	char* path_files = obtener_path_files();
	uint32_t cantidad_posiciones;

	if(existe_file(path_pokemon)==0){

		while(archivo_pokemon_esta_abierto(path_pokemon) ==1){
			int tiempo_reintento_operacion = tiempo_de_reintento_operacion();
			sleep(tiempo_reintento_operacion);
		}
		abrir_archivo_pokemon(path_pokemon);

		char* bloques_en_string = obtener_bloques_pokemon_string(path_pokemon);

		if(strlen(bloques_en_string) == 2){ //si no tiene bloques asignados, entonces no tiene posiciones

			cantidad_posiciones = 0;
			void* respuesta = malloc(sizeof(uint32_t));
			memcpy(respuesta, &cantidad_posiciones, sizeof(uint32_t));
			*tamanio_void = sizeof(uint32_t);

			int tiempo_retardo = tiempo_retardo_operacion();
			sleep(tiempo_retardo);
			cerrar_archivo_pokemon(path_pokemon);
			free(path_pokemon);

			return respuesta;
		}

		t_list* lista_datos = obtener_datos_bloques(path_pokemon);
		cantidad_posiciones = list_size(lista_datos);

		printf("hay %d posiciones \n",cantidad_posiciones);

		void* respuesta = malloc((cantidad_posiciones*8) + sizeof(uint32_t));
		*tamanio_void = (cantidad_posiciones*8) + sizeof(uint32_t);
		int offset = 0;

		memcpy(respuesta + offset, &cantidad_posiciones, sizeof(uint32_t));
		offset += sizeof(uint32_t);

		char* digito_actual_X;
		char* digito_actual_Y;
		int k;


		for(int i=0; i<cantidad_posiciones; i++){

			k=0;

			char* linea = list_get(lista_datos, i);
			char* valor_final_X = string_new();
			char* valor_final_Y = string_new();

			while(linea[k] != '-'){ // BUSCO POSICION X
				digito_actual_X =  string_from_format("%c",linea[k]);
				string_append(&valor_final_X,digito_actual_X);
				k++;
			}

			k++;

			while(linea[k] != '='){// BUSCO POSICION Y
				digito_actual_Y =  string_from_format("%c",linea[k]);
				string_append(&valor_final_Y,digito_actual_Y);
				k++;
			}

			uint32_t posX = atoi(valor_final_X); // TRANSFORMO EN UINT_32 AMBAS POSICIONES
			uint32_t posY = atoi(valor_final_Y);

			printf("la posicion en X del par numero %d es: %d \n",i+1,posX);
			printf("la posicion en Y del par numero %d es: %d \n",i+1,posY);

			memcpy(respuesta + offset, &posX, sizeof(uint32_t));
			offset += sizeof(uint32_t);


			memcpy(respuesta + offset, &posY, sizeof(uint32_t));
			offset += sizeof(uint32_t);
		}

		int tiempo_retardo = tiempo_retardo_operacion();
		sleep(tiempo_retardo);
		cerrar_archivo_pokemon(path_pokemon);

		list_destroy(lista_datos);
		free(path_pokemon);
		return respuesta;
	}
	else{
		cantidad_posiciones = 0;
		void* respuesta = malloc(sizeof(uint32_t));
		memcpy(respuesta, &cantidad_posiciones, sizeof(uint32_t));
		*tamanio_void = sizeof(uint32_t);
		free(path_pokemon);
		return respuesta;
	}
}

// Enviar mensaje a Broker

void enviar_appeared_pokemon(char* pokemon, uint32_t posX, uint32_t posY, uint32_t id_mensaje_correlativo){

	char* puerto_broker = obtener_puerto_broker();
	char* ip_broker = obtener_ip_broker();
	int socket_broker = crear_conexion(ip_broker,puerto_broker);

	if(socket_broker == -1){

		completar_logger("Error: No se puede conectar al broker para enviar un appeared, se continua la ejecucion","GAMECARD", LOG_LEVEL_INFO);
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

		completar_logger("Error: No se puede conectar al broker para enviar un caught, se continua la ejecucion","GAMECARD", LOG_LEVEL_INFO);
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

		completar_logger("Error: No se puede conectar al broker para enviar un localized, se continua la ejecucion","GAMECARD", LOG_LEVEL_INFO);
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
		completar_logger("Error en enviar por el socket","GAMECARD", LOG_LEVEL_INFO);
		exit(3);
	}

	free(stream);
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
