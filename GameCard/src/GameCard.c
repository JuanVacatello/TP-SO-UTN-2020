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

	char* path_metadata = string_new();
	string_append(&path_metadata, path_tall_grass);
	string_append(&path_metadata, "/Metadata");
	mkdir(path_metadata, 0777);


	char* path_bloques = string_new();
	string_append(&path_bloques, path_tall_grass);
	string_append(&path_bloques, "/Bloques");
	mkdir(path_bloques, 0777);


	char* path_files = string_new();
	string_append(&path_files, path_tall_grass);
	string_append(&path_files, "/Files");
	mkdir(path_files, 0777);

	puts(path_metadata);
	puts(path_bloques);
	puts(path_files);


	int block_size = 8;
	int cant_bloques = 16;

	inicializar_metadata(path_metadata, block_size, cant_bloques);




	//inicializar_files(path_files);
	//inicializar_bloques(path_bloques);
}

void inicializar_metadata(char* path_metadata, int block_size, int cant_bloques){

	char* path_archivo_metadata = string_new();
	string_append(&path_archivo_metadata, path_metadata);
	string_append(&path_archivo_metadata, "/Metadata.bin");

	FILE* metadata = fopen(path_archivo_metadata,"wb");

		if(metadata!=NULL){

			char* linea_block_size = string_new();
			string_append(&linea_block_size, "Hola");
			char* numero = string_itoa(block_size);

			//string_append(&linea_block_size, numero);
			unsigned short largo = strlen(linea_block_size);
			fwrite (&largo,sizeof(unsigned short),1,metadata);
			fwrite (&linea_block_size,largo,1,metadata);
	}

	fclose(metadata);

	metadata = fopen(path_archivo_metadata,"rb");

	unsigned short stringLength = 0;
	fread(&stringLength, sizeof(unsigned short),1,metadata);

	char* linea_texto = malloc(sizeof(char)*stringLength);
	fread(linea_texto, sizeof(char), stringLength, metadata);

	puts(linea_texto);

}


















