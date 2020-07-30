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

int bloque_esta_vacio(int bloque);
int existe_bloque(char* path_bloques,char* numero_de_bloque);
int existe_file(char* path);
void actualizar_valores_pokemon(char* path_metadata_pokemon,int posX,int posY,int cantidad);
char* obtener_path_bloque(char* bloque);
int diferente_largo(char* numero1,char * numero2);
int tamanio_libre_bloque(char* bloque);
int bloque_esta_lleno(char* bloque);
t_list* obtener_datos_bloques(char* path_pokemon );
void insertar_datos_a_lista(char *datos, t_list *lista_datos);
void guardar_data_en_bloque(char* data, char* path_bloque);
char* asignar_bloque(char* bloques);
char* liberar_ultimo_bloque(char* bloques);
char* obtener_path_bloque_individual(int numero_bloque);

void mostrar_contenido_lista(t_list* datos);

#endif /* UTILS_BLOQUES_H_ */
