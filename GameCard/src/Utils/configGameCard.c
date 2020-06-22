/*
 * configGameBoy.c
 *
 *  Created on: 15 may. 2020
 *      Author: utnso
 */

#include "configGameCard.h"

#include <commons/string.h>

void leer_config(void)
{
	configGameCard = config_create("/home/utnso/Documentos/Delibird/GameCard/tp-2020-1c-wRAPPERS/GameCard/GameCard.config");

	if(configGameCard == NULL){
		printf("No se pudo leer el archivo de configuración de la GameCard.");
		exit(2);
	}
}

char* obtener_puerto_broker(void){
	int puerto_b;
	puerto_b = config_get_int_value(configGameCard, "PUERTO_BROKER");
	char* puerto_en_string = string_itoa(puerto_b);
	return puerto_en_string;
}

char* obtener_ip_broker(void){
	char* ip_b;
	ip_b = config_get_string_value(configGameCard, "IP_BROKER");
	return ip_b;
}

char* obtener_punto_montaje(){
	char* punto_montaje;
	punto_montaje = config_get_string_value(configGameCard, "PUNTO_MONTAJE");
	return punto_montaje;
}

int tiempo_de_reintento_conexion(){
	int tiempocon;
	tiempocon = config_get_int_value(configGameCard, "TIEMPO_DE_REINTENTO_CONEXION");
	return tiempocon;
}

int tiempo_de_reintento_operacion(){
	int tiempoop;
	tiempoop = config_get_int_value(configGameCard, "TIEMPO_DE_REINTENTO_OPERACION");
	return tiempoop;
}

int tiempo_retardo_operacion(){
	int tiempore;
	tiempore = config_get_int_value(configGameCard, "TIEMPO_DE_RETARDO_OPERACION");
	return tiempore;
}

int obtener_id_propio(void){
	int process_id;
	process_id = config_get_int_value(configGameCard,"PROCESS_ID");
	return process_id;
}

void actualizar_paths_config(char* path_files,char* path_bloques,char* path_metadata){
	FILE* config = txt_open_for_append("/home/utnso/Documentos/Delibird/GameCard/tp-2020-1c-wRAPPERS/GameCard/GameCard.config");

	char* sentencia_files = string_new();
	string_append(&sentencia_files, "PATH_FILES=");
	string_append(&sentencia_files, path_files);
	string_append(&sentencia_files, "\n");

	char* sentencia_bloques = string_new();
	string_append(&sentencia_bloques, "PATH_BLOQUES=");
	string_append(&sentencia_bloques, path_bloques);
	string_append(&sentencia_bloques, "\n");

	char* sentencia_metadata = string_new();
	string_append(&sentencia_metadata, "PATH_METADATA=");
	string_append(&sentencia_metadata, path_metadata);
	string_append(&sentencia_metadata, "\n");

	txt_write_in_file(config,sentencia_files);
	txt_write_in_file(config,sentencia_bloques);
	txt_write_in_file(config,sentencia_metadata);

	txt_close_file(config);

	free(sentencia_files);
	free(sentencia_bloques);
	free(sentencia_metadata);

	leer_config();

}

char* obtener_path_files(){
	char* path_files;
	path_files = config_get_string_value(configGameCard, "PATH_FILES");
	return path_files;
}

char* obtener_path_bloques(){
	char* path_bloques;
	path_bloques = config_get_string_value(configGameCard, "PATH_BLOQUES");
	return path_bloques;
}

char* obtener_path_metadata(){
	char* path_metadata;
	path_metadata = config_get_string_value(configGameCard, "PATH_METADATA");
	return path_metadata;
}

