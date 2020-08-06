#include "GameCard.h"


int main(void) {

	inicializar_variables_globales();

	pthread_create(&hilo_new_pokemon, NULL, new_pokemon_broker, NULL);
	pthread_detach(hilo_new_pokemon);

	pthread_create(&hilo_catch_pokemon, NULL, catch_pokemon_broker, NULL);
	pthread_detach(hilo_catch_pokemon);

	pthread_create(&hilo_get_pokemon, NULL, get_pokemon_broker, NULL);
	pthread_detach(hilo_get_pokemon);

	iniciar_espera_mensajes_Gameboy();

	return 0;

	/*
	new_pokemon("Pikachu",7,2,1);

	catch_pokemon("Pikachu",7,2);

	mostrar_contenido_bitmap();

	new_pokemon("Pikachu",10,2,1);
	new_pokemon("Pikachu",1,2,6);
	catch_pokemon("Pikachu",7,2);
	new_pokemon("Pikachu",200,2,3);
	new_pokemon("Pikachu",10000000,2,1000000);
	new_pokemon("Pikachu",5,2,2);
	new_pokemon("Pikachu",11,9,2);

	uint32_t tamanio_void;
	get_pokemon("Pikachu", &tamanio_void);


	//new_pokemon("Pikachu",7,2,1);
	new_pokemon("Pikachu",7,2,5);

	catch_pokemon("Pikachu",7,2);
	catch_pokemon("Pikachu",7,2);
	catch_pokemon("Pikachu",7,2);
	catch_pokemon("Pikachu",7,2);
	catch_pokemon("Pikachu",7,2);


	new_pokemon("Pikachu",10,2,1);
	new_pokemon("Pikachu",1,2,6);
	new_pokemon("Pikachu",200,2,3);
	new_pokemon("Pikachu",10000000,2,1000000);
	new_pokemon("Pikachu",5,2,2);
	new_pokemon("Pikachu",11,9,2);
	new_pokemon("Pikachu",15,21,23);
	new_pokemon("Pikachu",7,2,2);
	new_pokemon("Pikachu",7,8,2);

	catch_pokemon("Pikachu",10,2);

	new_pokemon("Pikachu",10,2,1);

	catch_pokemon("Pikachu",10,2);

	catch_pokemon("Pikachu",6,7);
	*/


	//uint32_t tamanio_void;
	//get_pokemon("Charmander", &tamanio_void);





	/*
	pthread_mutex_init(&MUTEX_BITMAP, NULL);

	sem_init(&MUTEX_SUB,0,1);
	sem_init(&MUTEX_PRUEBA,0,0);


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


	//uint32_t tamanio_void = 0;
	//void* respuesta = get_pokemon("Pikachu", &tamanio_void);
}

void inicializar_variables_globales(){
	leer_config();
	iniciar_logger();

	sem_init(&MUTEX_MENSAJES_GB,0,1);
	sem_init(&semaforo_bitmap,0,1);
	sem_init(&MUTEX_SUB,0,1);

	char* punto_montaje = obtener_punto_montaje();
	inicializar_file_system(punto_montaje);
}


void inicializar_file_system(char* punto_montaje){

	char* path_tall_grass = string_new();
	string_append(&path_tall_grass, punto_montaje);
	char* magic_number = obtener_magic_number();
	string_append(&path_tall_grass, "/");
	string_append(&path_tall_grass, magic_number);
	mkdir(path_tall_grass, 0777);

	//CREA METADATA
	char* path_metadata = string_new();
	string_append(&path_metadata, path_tall_grass);
	string_append(&path_metadata, "/Metadata");
	if(existe_file(path_metadata)==-1){
		mkdir(path_metadata, 0777);
		inicializar_metadata(path_metadata, obtener_tamanio_bloques(), obtener_cantidad_bloques());
	}
	path_metadata_global = path_metadata;
	leer_metadata_tall_grass(path_metadata);


	//CREA BLOQUES
	char* path_bloques = string_new();
	string_append(&path_bloques, path_tall_grass);
	string_append(&path_bloques, "/Bloques");
	if(existe_file(path_bloques)==-1)  {
		mkdir(path_bloques, 0777);
		inicializar_bloques(path_bloques);
	}
	path_bloques_global = path_bloques;


	//CREA FILES
	char* path_files = string_new();
	string_append(&path_files, path_tall_grass);
	string_append(&path_files, "/Files");
	if(existe_file(path_files)==-1){
		mkdir(path_files, 0777);
		inicializar_files(path_files);
	}
	path_files_global = path_files;

	//inicializa bitmap luego de crear los bloques y actualizar los paths
	char* path_archivo_bitmap = string_new();
	string_append(&path_archivo_bitmap,path_metadata);
	string_append(&path_archivo_bitmap,"/bitmap.bin");
	crear_bitmap(path_archivo_bitmap);

	free(path_tall_grass);

}

void inicializar_metadata(char* path_metadata, int block_size, int cant_bloques){

//inicializa metadata.bin
	char* path_archivo_metadata = string_new();
	string_append(&path_archivo_metadata, path_metadata);
	string_append(&path_archivo_metadata, "/Metadata.bin");

	FILE* metadata = txt_open_for_append(path_archivo_metadata);

	char* tamanio_bloques = string_itoa(block_size);

	char* cantidad_bloques = string_itoa(cant_bloques);

	char* sentencia_tamanio_bloques = string_new();
	string_append(&sentencia_tamanio_bloques, "BLOCK_SIZE=");
	string_append(&sentencia_tamanio_bloques, tamanio_bloques);
	string_append(&sentencia_tamanio_bloques, "\n");

	char* sentencia_cantidad_bloques = string_new();
	string_append(&sentencia_cantidad_bloques, "BLOCKS=");
	string_append(&sentencia_cantidad_bloques, cantidad_bloques);
	string_append(&sentencia_cantidad_bloques, "\n");

	char* sentencia_magic_number = string_new();
	string_append(&sentencia_cantidad_bloques, "MAGIC_NUMBER=");
	string_append(&sentencia_cantidad_bloques, obtener_magic_number());
	string_append(&sentencia_cantidad_bloques, "\n");

	txt_write_in_file(metadata, sentencia_tamanio_bloques);
	txt_write_in_file(metadata, sentencia_cantidad_bloques);
	txt_write_in_file(metadata, sentencia_magic_number);
	txt_close_file(metadata);

	free(tamanio_bloques);
	free(cantidad_bloques);
	free(sentencia_tamanio_bloques);
	free(sentencia_cantidad_bloques);
	free(sentencia_magic_number);
	free(path_archivo_metadata);

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
	free(path_archivo_metadata);
}

void crear_bloques(char* path_bloques){

	int cantidad_bloques = obtener_cantidad_bloques();
	int tamanio_bloques = obtener_tamanio_bloques();

	for(int i = 0 ; i < cantidad_bloques ; i++){

		char* numero_bloque = string_itoa(i+1);
		char* aux = string_new();

		string_append(&aux, path_bloques);
		string_append(&aux, "/");
		string_append(&aux,numero_bloque);
		string_append(&aux, ".bin");

		FILE *f = fopen(aux,"w");
		fclose(f);
		free(aux);
		free(numero_bloque);
	}
}


void mostrar_paths_generados(char* iteracion){

	printf("iteracion numero: %s \n", iteracion);
	printf("el path metadata es: %s \n", obtener_path_metadata());
	printf("el path files es: %s \n", obtener_path_files());
	printf("el path bloques es: %s \n", obtener_path_bloques());
}
