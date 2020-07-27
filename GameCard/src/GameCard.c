#include "GameCard.h"


int main(void) {

	iniciar_logger();
	leer_config();

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


//	pthread_create(&hilo_gameboy, NULL, iniciar_espera_mensajes_Gameboy, NULL);
//	pthread_detach(hilo_gameboy);

	pthread_create(&hilo_new_pokemon, NULL, new_pokemon_broker, NULL);
	pthread_detach(hilo_new_pokemon);

	pthread_create(&hilo_catch_pokemon, NULL , catch_pokemon_broker, NULL);
	pthread_detach(hilo_catch_pokemon);

	pthread_create(&hilo_get_pokemon, NULL , get_pokemon_broker, NULL);
	pthread_detach(hilo_get_pokemon);


//	pthread_create(&HILO_PRINCIPAL, NULL , FUNCION_PRINCIPAL_GAMECARD (?) ,NULL);
//	pthread_join(HILO_PRINCIPAL,NULL);

	sem_wait(&MUTEX_PRUEBA);

	*/

	char* punto_montaje = obtener_punto_montaje();
	inicializar_file_system(punto_montaje);

	//mostrar_paths_generados();
	//mostrar_contenido_bitmap();
	//mostrar_paths_generados("1");

	//new_pokemon("Charmander",0,0);
	//mostrar_paths_generados("2");

	t_list* lista_datos = obtener_datos_bloques("/home/utnso/Documentos/Prueba_GameCard/TALL_GRASS/Files/Charmander");
	//mostrar_paths_generados("3");
    int indice = existe_posicion_lista(lista_datos,5,7 );
    if(indice == -1){
    	puts("no se encuentra en la lista");
    }


/*
    agregar_linea(lista_datos, "1-9=1");
	char* datos = obtener_datos_en_string(lista_datos);
	almacenar_datos(datos, "/home/utnso/Documentos/Prueba_GameCard/TALL_GRASS/Files/Charmander");// ULTRA MODIFICADA CON HARDCODEO, ARREGLAR
	mostrar_contenido_lista(lista_datos);
*/
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
	actualizar_path_metadata(path_metadata);
	leer_metadata_tall_grass(path_metadata);


	//CREA BLOQUES
	char* path_bloques = string_new();
	string_append(&path_bloques, path_tall_grass);
	string_append(&path_bloques, "/Bloques");
	if(existe_file(path_bloques)==-1)  {
		mkdir(path_bloques, 0777);
		inicializar_bloques(path_bloques);
	}
	actualizar_path_bloques(path_bloques);


	//CREA FILES
	char* path_files = string_new();
	string_append(&path_files, path_tall_grass);
	string_append(&path_files, "/Files");
	if(existe_file(path_files)==-1){
		mkdir(path_files, 0777);
		inicializar_files(path_files);
	}
	actualizar_path_files(path_files);



	//inicializa bitmap luego de crear los bloques y actualizar los paths
		char* path_archivo_bitmap = string_new();
		string_append(&path_archivo_bitmap,path_metadata);
		string_append(&path_archivo_bitmap,"/bitmap.bin");
		crear_bitmap(path_archivo_bitmap);

	free(path_files);
	free(path_bloques);
	free(path_metadata);
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

	for(int i = 0 ; i < cantidad_bloques ; i++){

		char* numero_bloque = string_new();
		numero_bloque = string_itoa(i+1);

		char* aux = string_new();
		string_append(&aux, path_bloques);
		string_append(&aux, "/");
		string_append(&aux,numero_bloque);
		string_append(&aux, ".bin");

		FILE *f = fopen(aux,"w");
		fclose(f);

	    free(numero_bloque);
	    free(aux);
	}
}


void mostrar_paths_generados(char* iteracion){

	printf("iteracion numero: %s \n", iteracion);
	printf("el path metadata es: %s \n", obtener_path_metadata());
	printf("el path files es: %s \n", obtener_path_files());
	printf("el path bloques es: %s \n", obtener_path_bloques());
}


















