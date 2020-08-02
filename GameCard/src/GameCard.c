#include "GameCard.h"


int main(void) {

	leer_config();
	iniciar_logger();

	char* punto_montaje = obtener_punto_montaje();
	inicializar_file_system(punto_montaje);

	/*
	pthread_mutex_init(&MUTEX_BITMAP, NULL);
	sem_init(&MUTEX_MENSAJES_GB,0,1);
	sem_init(&MUTEX_SUB,0,1);
	sem_init(&MUTEX_PRUEBA,0,0);

	 Funciona la suscripcion y la recepcion de los mensajes del GameBoy, otro dia hago lo del envio a Broker
	suscribirse_globalmente(1);
	suscribirse_globalmente(3);
	suscribirse_globalmente(5);
	iniciar_espera_mensajes();
	// Hay que hacerlo con hilos para que pueda recibir mensajes mientras hace otras cosas
*/
/*
	pthread_create(&hilo_gameboy, NULL, iniciar_espera_mensajes_Gameboy, NULL);
	pthread_detach(hilo_gameboy);

	pthread_create(&hilo_new_pokemon, NULL, new_pokemon_broker, NULL);
	pthread_detach(hilo_new_pokemon);

	pthread_create(&hilo_catch_pokemon, NULL , catch_pokemon_broker, NULL);
	pthread_detach(hilo_catch_pokemon);

	pthread_create(&hilo_get_pokemon, NULL , get_pokemon_broker, NULL);
	pthread_detach(hilo_get_pokemon);


//	pthread_create(&HILO_PRINCIPAL, NULL , FUNCION_PRINCIPAL_GAMECARD (?) ,NULL);
//	pthread_join(HILO_PRINCIPAL,NULL);

	sem_wait(&MUTEX_PRUEBA);

	new_pokemon("Pikachu",10,2,2);
	new_pokemon("Pikachu",10,2,2);
	new_pokemon("Pikachu",5,2,1);
	new_pokemon("Pikachu",4,2,2);
	new_pokemon("Pikachu",6,2,2);


	catch_pokemon("Pikachu",10,2);
	int respuesta = catch_pokemon("Pikachu",5,2);
	printf("%d", respuesta);

*/

	//new_pokemon("Pikachu",10,2,1);
	//catch_pokemon("Pikachu",6,7);

	void* respuesta = get_pokemon("Pikachu");

	return 0;

}


void inicializar_file_system(char* punto_montaje){

	char* path_tall_grass = string_new();
	string_append(&path_tall_grass, punto_montaje);
	string_append(&path_tall_grass, "/TALL_GRASS");
	mkdir(path_tall_grass, 0777);

	//CREA METADATA
	char* path_metadata = string_new();
	string_append(&path_metadata, path_tall_grass);
	string_append(&path_metadata, "/Metadata");
	if(existe_file(path_metadata)==-1){
		mkdir(path_metadata, 0777);
		inicializar_metadata(path_metadata, 64, 16);
	}
	path_metadata_global = path_metadata;
	//actualizar_path_metadata(path_metadata);
	leer_metadata_tall_grass(path_metadata);


	//CREA BLOQUES
	char* path_bloques = string_new();
	string_append(&path_bloques, path_tall_grass);
	string_append(&path_bloques, "/Bloques");
	if(existe_file(path_bloques)==-1)  {
		mkdir(path_bloques, 0777);
		inicializar_bloques(path_bloques);
	}
	path_bloques_global= path_bloques;
	//actualizar_path_bloques(path_bloques);


	//CREA FILES
	char* path_files = string_new();
	string_append(&path_files, path_tall_grass);
	string_append(&path_files, "/Files");
	if(existe_file(path_files)==-1){
		mkdir(path_files, 0777);
		inicializar_files(path_files);
	}
	path_files_global = path_files;
	//actualizar_path_files(path_files);



	//inicializa bitmap luego de crear los bloques y actualizar los paths
		char* path_archivo_bitmap = string_new();
		string_append(&path_archivo_bitmap,path_metadata);
		string_append(&path_archivo_bitmap,"/bitmap.bin");
		crear_bitmap(path_archivo_bitmap);

}

void inicializar_metadata(char* path_metadata, int block_size, int cant_bloques){

//inicializa metadata.bin
	char* path_archivo_metadata = string_new();
	string_append(&path_archivo_metadata, path_metadata);
	string_append(&path_archivo_metadata, "/Metadata.bin");

		FILE* metadata = txt_open_for_append(path_archivo_metadata);

			char* tamanio_bloques = string_new();
			tamanio_bloques = string_itoa(block_size);

			char* cantidad_bloques = string_new();
			cantidad_bloques = string_itoa(cant_bloques);

			char* sentencia_tamanio_bloques = string_new();
			string_append(&sentencia_tamanio_bloques, "BLOCK_SIZE=");
			string_append(&sentencia_tamanio_bloques, tamanio_bloques);
			string_append(&sentencia_tamanio_bloques, "\n");

			char* sentencia_cantidad_bloques = string_new();
			string_append(&sentencia_cantidad_bloques, "BLOCKS=");
			string_append(&sentencia_cantidad_bloques, cantidad_bloques);
			string_append(&sentencia_cantidad_bloques, "\n");

			txt_write_in_file(metadata, sentencia_tamanio_bloques);
			txt_write_in_file(metadata, sentencia_cantidad_bloques);
			txt_write_in_file(metadata, "MAGIC_NUMBER=TALL_GRASS\n");
			txt_close_file(metadata);


			free(tamanio_bloques);
			free(cantidad_bloques);
			free(sentencia_tamanio_bloques);
			free(sentencia_cantidad_bloques);

	free(path_archivo_metadata);


/*
char* path_archivo_bitmap = string_new();
string_append(&path_archivo_bitmap,path_metadata);
string_append(&path_archivo_bitmap,"/bitmap.bin");

//inicializa bitmap.bin
		crear_bitmap(path_archivo_bitmap);

		free(path_archivo_bitmap);
*/
}



void inicializar_files(char* path_files){

	completar_metadata_directorio(path_files);

}


void inicializar_bloques(char* path_bloques){


		completar_metadata_directorio(path_bloques);
		crear_bloques(path_bloques);

}

void completar_metadata_directorio(char* path_directorio){

	char* path_archivo_metadata = string_new();
	string_append(&path_archivo_metadata, path_directorio);
	string_append(&path_archivo_metadata, "/Metadata.bin");

		FILE* metadata = txt_open_for_append(path_archivo_metadata);

		txt_write_in_file(metadata, "DIRECTORY=Y\n");
		txt_close_file(metadata);
}

void crear_bloques(char* path_bloques){

	int cantidad_bloques = obtener_cantidad_bloques();
	int tamanio_bloques = obtener_tamanio_bloques();

	char* numero_bloque = string_new();


	for(int i = 0 ; i < cantidad_bloques ; i++){


		numero_bloque = string_itoa(i+1);
		char* aux = string_new();

		string_append(&aux, path_bloques);
		string_append(&aux, "/");
		string_append(&aux,numero_bloque);
		string_append(&aux, ".bin");

		FILE *f = fopen(aux,"w");
		fclose(f);
		free(aux);


	}
	free(numero_bloque);

}


void mostrar_paths_generados(char* iteracion){

	printf("iteracion numero: %s \n", iteracion);
	printf("el path metadata es: %s \n", obtener_path_metadata());
	printf("el path files es: %s \n", obtener_path_files());
	printf("el path bloques es: %s \n", obtener_path_bloques());
}

void new_pokemon(char* pokemon,int posX,int posY, int cantidad){ //funciona

	char* path_pokemon = obtener_path_pokemon(pokemon);
	char* path_files = obtener_path_files();

		if(existe_file(path_pokemon)==0){

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

			char* linea = generar_linea_a_insertar(posX, posY, cantidad);
			modificar_campo_size_metadata(path_pokemon,strlen(linea));
			almacenar_datos(linea, path_pokemon);


			leer_config();
			int tiempo_retardo = tiempo_retardo_operacion();
			sleep(tiempo_retardo);
			cerrar_archivo_pokemon(path_pokemon);

		}

		free(path_pokemon);
}

int catch_pokemon(char* pokemon,int posX,int posY){

	char* path_pokemon = obtener_path_pokemon(pokemon);
	char* path_files = obtener_path_files();


	if(existe_file(path_pokemon)==0){

		while(archivo_pokemon_esta_abierto(path_pokemon) ==1){
			int tiempo_reintento_operacion = tiempo_de_reintento_operacion();
			sleep(tiempo_reintento_operacion);
		}
		abrir_archivo_pokemon(path_pokemon);

		char* bloques_en_string = obtener_bloques_pokemon_string(path_pokemon);
		if(strlen(bloques_en_string) == 2){ //si no tiene bloques asignados
			// CHEQUEAR SI HAY QUE HACER UN SLEEP DE RETARDO
			cerrar_archivo_pokemon(path_pokemon);
			return -1;
		}
		else{ // si tiene bloques asignados

			t_list* lista_datos = obtener_datos_bloques(path_pokemon);
			int indice = existe_posicion_en_lista(lista_datos,posX,posY);

			if(indice == -1){ // si no existe la posicion
				cerrar_archivo_pokemon(path_pokemon);
				return -1;
			}
			else{ // si existe la posicion
				int flag_cambio_longitud = 0;
				char* linea_a_modificar = list_get(lista_datos, indice);
				char* linea_modificada = disminuir_cantidad_linea(linea_a_modificar, &flag_cambio_longitud);

				if(cantidad_igual_cero(linea_modificada) == 1){
					list_remove_and_destroy_element(lista_datos,indice, free);
					flag_cambio_longitud = 1;

					if(list_size(lista_datos) == 0){
						limpiar_bloques_pokemon(path_pokemon);
						liberar_bloques_pokemon(path_pokemon);
						modificar_campo_bloques_metadata(path_pokemon,"[]");
						modificar_campo_size_metadata(path_pokemon,0);

						int tiempo_retardo = tiempo_retardo_operacion();
						sleep(tiempo_retardo);
						cerrar_archivo_pokemon(path_pokemon);
						return 1;
					}
				}
				else{
					list_replace(lista_datos,indice,linea_modificada);
				}

				char* datos = obtener_datos_en_string(lista_datos);

				if(flag_cambio_longitud == 1 ){// si la longitud de la palabra cambio, actualizo el tamaño del pokemon
					modificar_campo_size_metadata(path_pokemon,strlen(datos));
				}

				almacenar_datos(datos, path_pokemon);

				int tiempo_retardo = tiempo_retardo_operacion();
				sleep(tiempo_retardo);
				cerrar_archivo_pokemon(path_pokemon);
				return 1;
			}
		}
	}
	else{ //si no existe
		return -1;
	}

	free(path_pokemon);
}

void* get_pokemon(char* pokemon){

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

			int tiempo_retardo = tiempo_retardo_operacion();
			sleep(tiempo_retardo);
			cerrar_archivo_pokemon(path_pokemon);

			return respuesta;
				}

		t_list* lista_datos = obtener_datos_bloques(path_pokemon);
		cantidad_posiciones = list_size(lista_datos);

		printf("hay %d posiciones \n",cantidad_posiciones);

		void* respuesta = malloc((cantidad_posiciones*8) + sizeof(uint32_t));
		int offset = 0;

		memcpy(respuesta + offset, &cantidad_posiciones, sizeof(uint32_t));
		offset += sizeof(uint32_t);



		char* digito_actual_X;
		char* digito_actual_Y;
		int k=0;


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
				 while(linea[i] != '='){// BUSCO POSICION Y
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

		return respuesta;

	}
	else{
		cantidad_posiciones = 0;
		void* respuesta = malloc(sizeof(uint32_t));
		memcpy(respuesta, &cantidad_posiciones, sizeof(uint32_t));
		return respuesta;
	}

}












