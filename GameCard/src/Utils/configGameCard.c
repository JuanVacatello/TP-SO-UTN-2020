#include "configGameCard.h"

#include <commons/string.h>

void leer_config(void)
{
	configGameCard = config_create("../GameCard.config");

	if(configGameCard == NULL){
		printf("No se pudo leer el archivo de configuración de la GameCard.");
		exit(2);
	}
}

char* obtener_puerto_broker(void){
	char* puerto;
	puerto = config_get_string_value(configGameCard, "PUERTO_BROKER");
	return puerto;
}

char* obtener_ip_broker(void){
	char* ip_b;
	ip_b = config_get_string_value(configGameCard, "IP_BROKER");
	return ip_b;
}

char* obtener_punto_montaje(){
	char* punto_montaje;
	punto_montaje = config_get_string_value(configGameCard, "PUNTO_MONTAJE_TALLGRASS");
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

void actualizar_path_files(char* path_files){

	config_set_value(configGameCard, "PATH_FILES",path_files);
	config_save(configGameCard);

	leer_config();
}

void actualizar_path_metadata(char* path_metadata){

	config_set_value(configGameCard, "PATH_METADATA",path_metadata);
	config_save(configGameCard);

	leer_config();
}

void actualizar_path_bloques(char* path_bloques){

	config_set_value(configGameCard, "PATH_BLOQUES",path_bloques);
	config_save(configGameCard);

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

char* obtener_log_file(void){
	char* ruta_log;
	ruta_log = config_get_string_value(configGameCard, "LOG_FILE");
	return ruta_log;
}

char* obtener_puerto_gamecard(void){
	int puerto_g;
	puerto_g = config_get_int_value(configGameCard, "PUERTO_GAMECARD");
	char* puerto_en_string = string_itoa(puerto_g);
	return puerto_en_string;
}

char* obtener_ip_gamecard(void){
	char* ip_g;
	ip_g = config_get_string_value(configGameCard, "IP_GAMECARD");
	return ip_g;
}
