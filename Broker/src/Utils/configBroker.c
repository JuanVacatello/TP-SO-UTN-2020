#include "configBroker.h"

void leer_config(void)
{
	configBroker = config_create("../Broker.config");

	if(configBroker == NULL){
		printf("No se pudo leer el archivo de configuración del Broker.");
		exit(2);
	}
}

int obtener_tamanio_memoria(void){
	int tam_mem;
	tam_mem = config_get_int_value(configBroker, "TAMANO_MEMORIA");
	return tam_mem;
}

int obtener_tamanio_minimo_particion(void){
	int tam_min_particion;
	tam_min_particion = config_get_int_value(configBroker, "TAMANO_MINIMO_PARTICION");
	return tam_min_particion;
}

char* obtener_puerto_broker(void){
	int puerto_b;
	puerto_b = config_get_int_value(configBroker, "PUERTO_BROKER");
	char* puerto_en_string = string_itoa(puerto_b);
	return puerto_en_string;
}

int obtener_frecuencia_compactacion(void){
	int frecuencia_compactacion;
	frecuencia_compactacion = config_get_int_value(configBroker, "FRECUENCIA_COMPACTACION");
	return frecuencia_compactacion;
}

char* obtener_algoritmo_memoria(void){
	char* algoritmo_mem;
	algoritmo_mem = config_get_string_value(configBroker, "ALGORITMO_MEMORIA");
	return algoritmo_mem;
}

char* obtener_algoritmo_reemplazo(void){
	char* algoritmo_reemplazo;
	algoritmo_reemplazo = config_get_string_value(configBroker, "ALGORITMO_REEMPLAZO");
	return algoritmo_reemplazo;
}

char* obtener_algoritmo_particion_libre(void){
	char* algoritmo_particion_libre;
	algoritmo_particion_libre = config_get_string_value(configBroker, "ALGORITMO_PARTICION_LIBRE");
	return algoritmo_particion_libre;
}

char* obtener_ip_broker(void){
	char* ip_b;
	ip_b = config_get_string_value(configBroker, "IP_BROKER");
	return ip_b;
}

char* obtener_dump_file(void){
	char* ruta_dump;
	ruta_dump = config_get_string_value(configBroker, "PATH_ARCHIVO_DUMP");
	return ruta_dump;
}

char* obtener_log_file(void){
	char* ruta_log;
	ruta_log = config_get_string_value(configBroker, "LOG_FILE");
	return ruta_log;
}


