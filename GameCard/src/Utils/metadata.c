#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "metadata.h"
#include "string.h"
#include <commons/collections/dictionary.h>

//ñeer metadata tall grass
void leer_metadata_tall_grass(){

	char* path_metadata = obtener_path_metadata();
	string_append(&path_metadata, "/Metadata.bin");

	metadata_tall_grass = config_create(path_metadata);

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
t_config* leer_metadata_pokemon(char* path_metadata_pokemon){

	t_config* metadata;

	/*
	char* path_archivo_metadata = string_new();
	string_append(&path_archivo_metadata, path_metadata);
	string_append(&path_archivo_metadata, "/Metadata.bin")
	*/

	metadata = config_create(path_metadata_pokemon);

		if(metadata == NULL){
			printf("No se pudo leer el archivo metadata.");
			exit(2);
		}
	return metadata;
}

char** obtener_bloques_pokemon(t_config* metadata){ //DEVUELVE EN FORMATO: ["1","2",....,"N"]
	char* bloques;
	bloques = config_get_string_value(metadata, "BLOCKS");
	char** array_bloques = string_get_string_as_array(bloques);
	return array_bloques;

	/*
	for(int i =0; i<obtener_cantidad_bloques_pokemon() ; i++){
	printf("%s,",array_bloques[i]);
	}
	*/
}


int obtener_cantidad_bloques_pokemon(t_config* metadata){

	char* bloques;
	bloques = config_get_string_value(metadata, "BLOCKS");
	int largo = strlen(bloques);
	int comas = (largo-2)/2;
	int cantidad_elementos = largo - comas - 2;
	return cantidad_elementos;
}

int archivo_esta_abierto(t_config* metadata){

	char* open = config_get_string_value(metadata, "OPEN");
	if(string_equals_ignore_case(open, "Y")){
		return 1;
	}
	else {
		return 0;
	}
}

int obtener_tamanio_archivo(t_config* metadata){
	int tamanio;
	tamanio = config_get_int_value(metadata,"SIZE");
	return tamanio;
}

