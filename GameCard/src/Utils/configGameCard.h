#ifndef CONFIGGAMEBOY_H_
#define CONFIGGAMEBOY_H_

#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<commons/string.h>

#include<commons/string.h>

char* path_bloques_global;
char* path_metadata_global;
char* path_files_global;

t_config* configGameCard;

void leer_config(void);
char* obtener_puerto_broker(void);
char* obtener_ip_broker(void);
char* obtener_log_file(void);
char* obtener_punto_montaje(void);
int tiempo_de_reintento_conexion(void);
int tiempo_de_reintento_operacion(void);
int tiempo_retardo_operacion(void);
int obtener_id_propio(void);
void actualizar_path_files(char* path_files);
void actualizar_path_metadata(char* path_metadata);
void actualizar_path_bloques(char* path_bloques);
char* obtener_path_files();
char* obtener_path_bloques();
char* obtener_path_metadata();
char* obtener_puerto_gamecard(void);
char* obtener_ip_gamecard(void);
int obtener_tamanio_bloques(void);
int obtener_cantidad_bloques(void);

#endif /* CONFIGGAMEBOY_H_ */
