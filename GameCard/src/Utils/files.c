/*
 * files.c
 *
 *  Created on: 22 jun. 2020
 *      Author: utnso
 */
#include "files.h"

char* obtener_path_pokemon(char * pokemon){

	char* path_pokemon = string_new();
		char* path_files = obtener_path_files();
			string_append(&path_pokemon, path_files);
			string_append(&path_pokemon, "/");
			string_append(&path_pokemon, pokemon);

		return path_pokemon;
}

int existe_posicion_pokemon(char* pokemon, int posX, int posY){
	char* path_pokemon = obtener_path_pokemon(pokemon);
	t_list* lista_datos = obtener_datos_bloques(path_pokemon);
	int indice = existe_posicion_en_lista(lista_datos,5,7);
	return indice;
}

void insertar_nueva_linea(char* path_pokemon, int posX, int posY){

	char* linea = generar_linea_a_insertar(posX, posY, 1);
	///modificar_campo_size_metadata(char * path_pokemon,int tamanio)
	almacenar_datos(linea, path_pokemon);

}

char* generar_linea_a_insertar(int posX, int posY, int cantidad){
	char* linea = string_new();

	char* posX_string = string_new();
	posX_string = string_itoa(posX);

	char* posY_string = string_new();
	posY_string = string_itoa(posY);

	char* cantidad_string = string_new();
	cantidad_string = string_itoa(cantidad);

	string_append(&linea,posX_string);
	string_append(&linea,"-");
	string_append(&linea,posY_string);
	string_append(&linea,"=");
	string_append(&linea,cantidad_string);
	string_append(&linea,"\n");

	free(posX_string);
	free(posY_string);
	free(cantidad_string);

	return linea;
}



char* aumentar_cantidad_linea(char* linea_a_modifcar,int* flag_diferente_largo){

	int valor_modificado;
	char* digito;
	int i = 0;
	int pos_signo_igual;
	char* fusion = string_new();

	while(linea_a_modifcar[i] != '='){ // avanzo hasta el =
		i++;
	}
	pos_signo_igual = i;
	i++;

	while(linea_a_modifcar[i] != '\n'){ //LEO EL VALOR EN STRING
	digito = string_from_format("%c",linea_a_modifcar[i]);
	string_append(&fusion,digito);
 	i++;
    	}

	valor_modificado = atoi(fusion)+1; // CONVIERTO A INT, AUMENTO, CONVIERTO A STRING
	char* valor_aumentado_string = string_itoa(valor_modificado);

	if(strlen(fusion) != strlen(valor_aumentado_string)){
		*flag_diferente_largo = 1;
	}
	char* aux = string_new();
	string_append(&aux, string_substring_until(linea_a_modifcar,pos_signo_igual+1));
	string_append(&aux,valor_aumentado_string );
	string_append(&aux,"\n" );


	return aux;
}

char* disminuir_cantidad_linea(char* linea_a_modifcar){

	int valor_modificado;
	char* digito;
	int i = 0;
	int pos_signo_igual;
	char* fusion = string_new();

	while(linea_a_modifcar[i] != '='){ // avanzo hasta el =
		i++;
	}
	pos_signo_igual = i;
	i++;

	while(linea_a_modifcar[i] != '\n'){ //LEO EL VALOR EN STRING
	digito = string_from_format("%c",linea_a_modifcar[i]);
	string_append(&fusion,digito);
 	i++;
    	}

	valor_modificado = atoi(fusion)-1; // CONVIERTO A INT, AUMENTO, CONVIERTO A STRING
	char* valor_aumentado_string = string_itoa(valor_modificado);

	char* aux = string_new();
	//string_append(&aux, string_substring_until(linea_a_modifcar,pos_signo_igual+1));
	string_append(&aux,valor_aumentado_string );
	string_append(&aux,"\n" );

	return aux;
}



