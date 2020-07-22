/*
 * configGameBoy.h
 *
 *  Created on: 15 may. 2020
 *      Author: utnso
 */

#ifndef CONFIGGAMEBOY_H_
#define CONFIGGAMEBOY_H_

#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<commons/string.h>

#include<commons/string.h>

t_config* configGameCard;

void leer_config(void);
char* obtener_puerto_broker(void);
char* obtener_ip_broker(void);
char* obtener_puerto_gamecard(void);
char* obtener_ip_gamecard(void);
char* obtener_log_file(void);
char* obtener_punto_montaje(void);
int tiempo_de_reintento_conexion(void);
int tiempo_de_reintento_operacion(void);
int tiempo_retardo_operacion(void);
int obtener_id_propio(void);
void actualizar_paths_config(char* path_files,char* path_bloques,char* path_metadata );



#endif /* CONFIGGAMEBOY_H_ */
