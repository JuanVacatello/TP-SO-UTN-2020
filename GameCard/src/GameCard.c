#include "GameCard.h"


int main(void) {


	leer_config();

	/* Funciona la suscripcion y la recepcion de los mensajes del GameBoy, otro dia hago lo del envio a Broker
	suscribirse_globalmente(1);
	suscribirse_globalmente(3);
	suscribirse_globalmente(5);
	iniciar_espera_mensajes();
	// Hay que hacerlo con hilos para que pueda recibir mensajes mientras hace otras cosas
	*/

	char* punto_montaje = obtener_punto_montaje();
	inicializar_file_system(punto_montaje);

	return 0;

}


void inicializar_file_system(char* punto_montaje){

	char* path_tall_grass = string_new();
	string_append(&path_tall_grass, punto_montaje);
	string_append(&path_tall_grass, "/TALL_GRASS");
	mkdir(path_tall_grass, 0777);

	//CREA METADATA//FALTA
	char* path_metadata = string_new();
	string_append(&path_metadata, path_tall_grass);
	string_append(&path_metadata, "/Metadata");
	if(existe_file(path_metadata)==-1){
		mkdir(path_metadata, 0777);
		inicializar_metadata(path_metadata, 64, 16);
	}

	//CREA BLOQUES
	char* path_bloques = string_new();
	string_append(&path_bloques, path_tall_grass);
	string_append(&path_bloques, "/Bloques");
	if(existe_file(path_bloques)==-1)  {
		mkdir(path_bloques, 0777);
		inicializar_bloques(path_bloques);
	}

	//inicializa bitmap luego de crear los bloques
	char* path_archivo_bitmap = string_new();
	string_append(&path_archivo_bitmap,path_metadata);
	string_append(&path_archivo_bitmap,"/bitmap.bin");
	crear_bitmap(path_archivo_bitmap);

	//CREA FILES
	char* path_files = string_new();
	string_append(&path_files, path_tall_grass);
	string_append(&path_files, "/Files");
	if(existe_file(path_bloques)==-1){
		mkdir(path_files, 0777);
		inicializar_files(path_files);

	}

	actualizar_paths_config(path_files, path_bloques, path_metadata );





	//obtener_bloque_pokemon("Charmander", path_files);

	//agregar_pokemon("Charmander",1,1,2, path_files); //Funciona

	//int a = existe_bloque(path_bloques,"1");
	//printf("%d",a);
	//obtener_bloque_pokemon( "Pikachu", path_files);
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


	leer_metadata_tall_grass(path_metadata);
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
	free(path_bloques);
}





















