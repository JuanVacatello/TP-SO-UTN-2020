/*
 * bloques.c
 *
 *  Created on: 21 jun. 2020
 *      Author: utnso
 */

#include "bloques.h"

int bloque_esta_vacio(int bloque){
	if(bloque_tamanio_libre(bloque) == tamanio_bloque()){
	return 1;
	}
	return 0;
}

int existe_bloque(char* path_bloques,char* numero_de_bloque){
	char* path_bloque = string_new();
	string_append(&path_bloque, path_bloques);
	string_append(&path_bloque,"/" );
	string_append(&path_bloque,numero_de_bloque);
	string_append(&path_bloque, ".bin");
	struct stat buffer;
	return stat(path_bloque,&buffer);
}


