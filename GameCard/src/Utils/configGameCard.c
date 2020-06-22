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

