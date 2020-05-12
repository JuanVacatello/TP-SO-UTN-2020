/*
 ============================================================================
 Name        : GameBoy.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <commons/temporal.h>
#include "GameBoy.h"

int main(void) {
	int puerto = 4444;
	char* ip = "127.0.0.1";
	char* puertoEnString = string_itoa(puerto);
	int conexion = crear_conexion(ip,puertoEnString);


	enviar_mensaje(conexion,NEW_POKEMON); //hardcodeado NEWPOKEMON
	recibir_mensaje(conexion);
	terminar_programa(conexion);
}

void terminar_programa(int conexion){
	liberar_conexion(conexion);
}
