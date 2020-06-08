/*
 ============================================================================
 Name        : GameCard.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "GameCard.h"

int main(void) {
	leer_config();
	char* punto_montaje = obtener_punto_montaje();

	inicializar_file_sistem(punto_montaje);


}


void inicializar_file_sistem(punto_montaje){
	char *unaPalabra = string_new();
		string_append(&unaPalabra, punto_montaje);
		string_append(&unaPalabra, "/TALL_GRASS");
	mkdir(unaPalabra, 0777);

	string_append(&unaPalabra, "/Files");
	mkdir(unaPalabra, 0777);
}
