#include "configGameBoy.h"

void leer_config(void)
{
	configGameBoy = config_create("/home/utnso/Documentos/Delibird/GameBoy/tp-2020-1c-wRAPPERS/GameBoy/GameBoy.config");

	if(configGameBoy == NULL){
		printf("No se pudo leer el archivo de configuración del GameBoy.");
		exit(2);
	}
}

char* obtener_puerto_broker(void){
	int puerto_b;
	puerto_b = config_get_int_value(configGameBoy, "PUERTO_BROKER");
	char* puerto_en_string = string_itoa(puerto_b);
	return puerto_en_string;
}

char* obtener_ip_broker(void){
	char* ip_b;
	ip_b = config_get_string_value(configGameBoy, "IP_BROKER");
	return ip_b;
}

char* obtener_puerto_team(void){
	int puerto_t;
	puerto_t = config_get_int_value(configGameBoy, "PUERTO_TEAM");
	char* puerto_en_string = string_itoa(puerto_t);
	return puerto_en_string;
}

char* obtener_ip_team(void){
	char* ip_t;
	ip_t = config_get_string_value(configGameBoy, "IP_TEAM");
	return ip_t;
}

char* obtener_puerto_gamecard(void){
	int puerto_g;
	puerto_g = config_get_int_value(configGameBoy, "PUERTO_GAMECARD");
	char* puerto_en_string = string_itoa(puerto_g);
	return puerto_en_string;
}

char* obtener_ip_gamecard(void){
	char* ip_g;
	ip_g = config_get_string_value(configGameBoy, "IP_GAMECARD");
	return ip_g;
}

char* obtener_log_file(void){
	char* ruta_log;
	ruta_log = config_get_string_value(configGameBoy, "LOG_FILE");
	return ruta_log;
}

int obtener_id_propio(void){
	int process_id;
	process_id = config_get_int_value(configGameBoy, "PROCESS_ID");
	return process_id;
}


