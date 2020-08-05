#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "metadata.h"
#include "string.h"
#include <commons/collections/dictionary.h>

//leer metadata tall grass
void leer_metadata_tall_grass(char * path_metadata){

	char* path_metadata_tall_grass = string_new();
	string_append(&path_metadata_tall_grass, path_metadata);
	string_append(&path_metadata_tall_grass, "/Metadata.bin");

	metadata_tall_grass = config_create(path_metadata_tall_grass);

		if(metadata_tall_grass == NULL){
			printf("No se pudo leer el archivo metadata.");
			exit(2);
		}

	free(path_metadata_tall_grass);
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

	free(path_archivo_metadata);
	return metadata;
}

char** obtener_bloques_pokemon(t_config* metadata_pokemon){ //DEVUELVE EN FORMATO: ["1","2",....,"N"]

	char** bloques =  config_get_array_value(metadata_pokemon, "BLOCKS");
	return bloques;

}

char* obtener_bloques_pokemon_string(t_config* metadata_pokemon){

	char* bloques_string = config_get_string_value(metadata_pokemon, "BLOCKS");

	return bloques_string;
}

int obtener_cantidad_bloques_pokemon(t_config* metadata_pokemon){

	char* bloques;

	bloques = config_get_string_value(metadata_pokemon, "BLOCKS");
	int largo = strlen(bloques);
	int comas = (largo-2)/2;
	int cantidad_elementos = largo - comas - 2;
	return cantidad_elementos;
}

int archivo_pokemon_esta_abierto(t_config* metadata_pokemon){
	char* open = config_get_string_value(metadata_pokemon, "OPEN");
	if(string_equals_ignore_case(open, "Y")){
		return 1;
	}
	else {
		return 0;
	}
}

void cerrar_archivo_pokemon(t_config* metadata_pokemon){
	config_set_value(metadata_pokemon,"OPEN","N");
	config_save(metadata_pokemon);
}

void abrir_archivo_pokemon(t_config* metadata_pokemon){
	config_set_value(metadata_pokemon,"OPEN","Y");
	config_save(metadata_pokemon);
}

int obtener_tamanio_archivo(t_config* metadata_pokemon){
	int tamanio;
	tamanio = config_get_int_value(metadata_pokemon,"SIZE");
	return tamanio;
}

void modificar_campo_bloques_metadata(t_config* metadata_pokemon,char* bloques){

	pthread_mutex_lock(&MUTEX_ELSOLUCIONES); // Si estoy adentro del chequeo de "open" voy a estar solo yo, no necesito otros semafotros
	config_set_value(metadata_pokemon,"BLOCKS",bloques);
	config_save(metadata_pokemon);

	pthread_mutex_unlock(&MUTEX_ELSOLUCIONES);
}

void modificar_campo_size_metadata(t_config* metadata_pokemon,int tamanio){

	//pthread_mutex_lock(&MUTEX_ELSOLUCIONES);// Si estoy adentro del chequeo de "open" voy a estar solo yo, no necesito otros semafotros
	char* tamanio_string = string_itoa(tamanio);
	config_set_value(metadata_pokemon,"SIZE",tamanio_string);
	config_save(metadata_pokemon);
	//pthread_mutex_unlock(&MUTEX_ELSOLUCIONES);
	free(tamanio_string);
}

int tamanio_array_en_string(char* array){ //falopa
	int largo = strlen(array);
	int comas = (largo-2)/2;
	int cantidad_elementos = largo - comas -2;
	return cantidad_elementos;
}


