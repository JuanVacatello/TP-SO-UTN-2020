#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "metadata.h"
#include "string.h"
#include <commons/collections/dictionary.h>

//ñeer metadata tall grass
void leer_metadata_tall_grass(char * path_metadata){

	char* path_metadata_tall_grass = string_new();
	string_append(&path_metadata_tall_grass, path_metadata);
	string_append(&path_metadata_tall_grass, "/Metadata.bin");

	metadata_tall_grass = config_create(path_metadata_tall_grass);

		if(metadata_tall_grass == NULL){
			printf("No se pudo leer el archivo metadata.");
			exit(2);
		}
}


int obtener_cantidad_bloques(){
	int cantidad;
	cantidad = config_get_int_value(metadata_tall_grass,"BLOCKS");
	return cantidad;
}

int obtener_tamanio_bloques(){
	int tamanio;
	tamanio = config_get_int_value(metadata_tall_grass,"BLOCK_SIZE");
	return tamanio;
}



//leer metadata pokemon
t_config* leer_metadata_pokemon(char* path_pokemon){

	t_config* metadata;


	char* path_archivo_metadata = string_new();
	string_append(&path_archivo_metadata, path_pokemon);
	string_append(&path_archivo_metadata, "/Metadata.bin");


	metadata = config_create(path_archivo_metadata);

		if(metadata == NULL){
			printf("No se pudo leer el archivo metadata.");
			exit(2);
		}
	return metadata;
}

char** obtener_bloques_pokemon(char* path_pokemon){ //DEVUELVE EN FORMATO: ["1","2",....,"N"]

	t_config* metadata_pokemon = leer_metadata_pokemon(path_pokemon);
	char* bloques_string;
	bloques_string = config_get_string_value(metadata_pokemon, "BLOCKS");

	char** bloques = string_get_string_as_array(bloques_string);
	config_destroy(metadata_pokemon);
	return bloques;

	/*
	for(int i =0; i<obtener_cantidad_bloques_pokemon() ; i++){
	printf("%s,",array_bloques[i]);
	}
	*/
}

char* obtener_bloques_pokemon_string(char* path_pokemon){

	t_config* metadata_pokemon = leer_metadata_pokemon(path_pokemon);
	char* bloques_string;
	bloques_string = config_get_string_value(metadata_pokemon, "BLOCKS");
	config_destroy(metadata_pokemon);
	return bloques_string;

	/*
	for(int i =0; i<obtener_cantidad_bloques_pokemon() ; i++){
	printf("%s,",array_bloques[i]);
	}
	*/
}


int obtener_cantidad_bloques_pokemon(char* path_pokemon){

	char* bloques;
	t_config* metadata_pokemon = leer_metadata_pokemon(path_pokemon);

	bloques = config_get_string_value(metadata_pokemon, "BLOCKS");
	int largo = strlen(bloques);
	int comas = (largo-2)/2;
	int cantidad_elementos = largo - comas - 2;
	return cantidad_elementos;
}

int archivo_esta_abierto(char* path_file){
	t_config* metadata = leer_metadata_file(path_file);
	char* open = config_get_string_value(metadata, "OPEN");
	if(string_equals_ignore_case(open, "Y")){
		return 1;
	}
	else {
		return 0;
	}
}

int obtener_tamanio_archivo(char* path_file){
	t_config* metadata = leer_metadata_file(path_file);
	int tamanio;
	tamanio = config_get_int_value(metadata,"SIZE");
	return tamanio;
}


t_config* leer_metadata_file(char* path_file){
	t_config* metadata;


		char* path_archivo_metadata = string_new();
		string_append(&path_archivo_metadata, path_file);
		string_append(&path_archivo_metadata, "/Metadata.bin");


		metadata = config_create(path_archivo_metadata);

			if(metadata == NULL){
				printf("No se pudo leer el archivo metadata.");
				exit(2);
			}
		return metadata;

}


void modificar_campo_bloques_metadata(char * path,char* bloques){

	pthread_mutex_lock(&MUTEX_ELSOLUCIONES);
		t_config *metadata = config_create(path);
		config_set_value(metadata,"BLOCKS",bloques);
		config_save(metadata);
		config_destroy(metadata);
		pthread_mutex_unlock(&MUTEX_ELSOLUCIONES);

}


int tamanio_array_en_string(char* array){
	int largo = strlen(array);
	int comas = (largo-2)/2;
	int cantidad_elementos = largo - comas -2;
	return cantidad_elementos;
}


