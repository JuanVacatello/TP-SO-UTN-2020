#include "config.h"


t_config* config = leer_config();

t_config leer_config(void)
{
	t_config* config;
	if((config = config_create("/home/utnso/Documentos/tp-2020-1c-wRAPPERS/Team/Team.config") == NULL){
			printf("Error en La conexion");
			exit(2);
	}
	return config;
}

char obtener_ip(t_config* config){
	char* ip;
	ip = config_get_string_value(config, "IP_BROKER");
	return ip;
}
char obtener_puerto(t_config* config){
	char* puerto;
	puerto = config_get_string_value(config, "PUERTO_BROKER");
	return puerto;
}
int obtener_retardo_ciclo_cpu(t_config* config){
	int retardo;
	retardo = config_get_string_value(config, "RETARDO_CICLO_CPU");
	return retardo;
}





char* ip;
char* puerto;



ip = config_get_string_value(config, "IP");
