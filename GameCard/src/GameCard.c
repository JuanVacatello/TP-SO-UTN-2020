/*
 ============================================================================
 Name        : GameCard.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "GameCard.h"

int main(void) {
	leer_config();
	char* punto_montaje = obtener_punto_montaje();

	inicializar_file_system(punto_montaje);


	return 0;
}


void inicializar_file_system(char* punto_montaje){


	char* path_tall_grass = string_new();
	puts(path_tall_grass);
	string_append(&path_tall_grass, punto_montaje);
	string_append(&path_tall_grass, "/TALL_GRASS");
	mkdir(path_tall_grass, 0777);

	//CREA METADATA
	char* path_metadata = string_new();
	string_append(&path_metadata, path_tall_grass);
	string_append(&path_metadata, "/Metadata");
	mkdir(path_metadata, 0777);

	//CREA BLOQUES
	char* path_bloques = string_new();
	string_append(&path_bloques, path_tall_grass);
	string_append(&path_bloques, "/Bloques");
	mkdir(path_bloques, 0777);

	//CREA FILES
	char* path_files = string_new();
	string_append(&path_files, path_tall_grass);
	string_append(&path_files, "/Files");
	mkdir(path_files, 0777);

	puts(path_metadata);
	puts(path_bloques);
	puts(path_files);


	inicializar_metadata(path_metadata, 4, 2);
	inicializar_files(path_files);
	inicializar_bloques(path_bloques);

	agregar_pokemon("Pikachu", path_files);
}

void inicializar_metadata(char* path_metadata, int block_size, int cant_bloques){

	char* path_archivo_metadata = string_new();
	string_append(&path_archivo_metadata, path_metadata);
	string_append(&path_archivo_metadata, "/Metadata.bin");

	FILE* metadata = txt_open_for_append(path_archivo_metadata);

	txt_write_in_file(metadata, "BLOCK_SIZE=64\n");
	txt_write_in_file(metadata, "BLOCKS=5192\n");
	txt_write_in_file(metadata, "MAGIC_NUMBER=TALL_GRASS\n");
	txt_close_file(metadata);




	char* path_archivo_bitmap= string_new();
	string_append(&path_archivo_bitmap, path_metadata);
	string_append(&path_archivo_bitmap, "/Bitmap.bin");

	FILE* bitmap = txt_open_for_append(path_archivo_bitmap);


	void* array= malloc(1);
	t_bitarray* bit_array = bitarray_create_with_mode(array,1, MSB_FIRST);
	size_t tamanio = bitarray_get_max_bit(bitmap);
	printf("%d\n",tamanio);

	bitarray_set_bit(bit_array, 4);
	//bitarray_clean_bit(bit_array, 4);

	puts((bit_array->bitarray));
		txt_write_in_file(bitmap,(bit_array->bitarray));
		txt_close_file(bitmap);
		//TERMINAR BITMAP, NO LEE BIEN
}



void inicializar_files(char* path_files){

	char* path_archivo_metadata = string_new();
		string_append(&path_archivo_metadata, path_files);
		string_append(&path_archivo_metadata, "/Metadata.bin");

		FILE* metadata = txt_open_for_append(path_archivo_metadata);

		txt_write_in_file(metadata, "DIRECTORY=Y\n");
		txt_close_file(metadata);
}


void inicializar_bloques(path_bloques){
	char* path_archivo_metadata = string_new();
	string_append(&path_archivo_metadata, path_bloques);
	string_append(&path_archivo_metadata, "/Metadata.bin");

	FILE* metadata = txt_open_for_append(path_archivo_metadata);

	txt_write_in_file(metadata, "DIRECTORY=Y\n");
	txt_close_file(metadata);
}

void agregar_pokemon(char* pokemon, char* path_files){

	char* path_pokemon = string_new();
		string_append(&path_pokemon, path_files);
		string_append(&path_pokemon, "/");
		string_append(&path_pokemon, pokemon);
		mkdir(path_pokemon, 0777);

		//FALTA COMPLETAR EL METADATA
}



