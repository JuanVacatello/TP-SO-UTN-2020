/*
 * files.c
 *
 *  Created on: 22 jun. 2020
 *      Author: utnso
 */
#include "files.h"

void new_pokemon(char* pokemon,int posX,int posY){ //funciona

	char* path_pokemon = string_new();
	char* path_files = obtener_path_files();
		string_append(&path_pokemon, path_files);
		string_append(&path_pokemon, "/");
		string_append(&path_pokemon, pokemon);


		if(existe_file(path_pokemon)==0){
			//actualizar_valores_pokemon(path_pokemon,posX,posY,1); //FALTA IMPLEMENTAR, TENDRIA QUE FUNCIONAR TANTO PARA SUMAR COMO RESTAR
			puts("existe");
		}
		else{
			puts("no existe");
			mkdir(path_pokemon, 0777);
			string_append(&path_pokemon,"/");
			string_append(&path_pokemon,"Metadata.bin");


			FILE* metadata = txt_open_for_append(path_pokemon);

			txt_write_in_file(metadata, "DIRECTORY=N\n");
			txt_write_in_file(metadata, "SIZE=\n");

			char* sentencia_blocks = string_new();
			string_append(&sentencia_blocks,"BLOCKS=");
			//char* bloque_asignado = obtener_siguiente_bloque_libre(); //FALTA IMPLEMENTAR.OBTENER PROX BLOQUE FORMATO:[]
			//string_append(&sentencia_blocks,bloque_asignado);
			//txt_write_in_file(metadata, sentencia_blocks);
			txt_write_in_file(metadata, "BLOCKS=\n");

			txt_write_in_file(metadata, "OPEN=N");
			txt_close_file(metadata);

			//actualizar_valores_pokemon(path_metadata_pokemon,posX,posY,cantidad);

			free(sentencia_blocks);
		}

		free(path_pokemon);
		free(path_files);
}

