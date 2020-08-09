/*
 * bloques.h
 *
 *  Created on: 21 jun. 2020
 *      Author: utnso
 */

#ifndef UTILS_BLOQUES_H_
#define UTILS_BLOQUES_H_

#include<stdio.h>
#include<commons/string.h>
#include<sys/stat.h>
#include<string.h>

#include"metadata.h"
#include"configGameCard.h"

int existe_bloque(char* path_bloques,char* numero_de_bloque);
int existe_file(char* path);

int tamanio_libre_bloque(char* bloque);
int bloque_esta_lleno(char* bloque);
int bloque_esta_vacio(int bloque);

void insertar_datos_a_lista(char *datos, t_list *lista_datos);
void guardar_data_en_bloque(char* data, char* path_bloque);
t_list* obtener_datos_bloques(t_config* metadata_pokemon);
char* obtener_datos_en_string(t_list* lista_datos);
void almacenar_datos(char *data,t_config* metadata_pokemon);

char* obtener_path_bloque_individual(char* bloque);
char* asignar_bloque(char* bloques);
char* asignar_primer_bloque();
char* liberar_ultimo_bloque(char* bloques);
void agregar_linea(t_list* lista, char* linea);
void mostrar_contenido_lista(t_list* datos);
int existe_posicion_en_lista(t_list* lista, int posX, int posY);
void liberar_bloques_pokemon(t_config* metadata_pokemon);
void limpiar_bloques_pokemon(t_config* metadata_pokemon);
void limpiar_bloque(char* bloque);

int tamanio_array(char **array);

#endif /* UTILS_BLOQUES_H_ */
