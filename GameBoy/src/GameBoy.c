/*
 ============================================================================
 Name        : GameBoy.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include "GameBoy.h"

int main(int argc, char* argv[]) {

	controlar_cant_argumentos(argc);

	leer_config();

	char* proceso = argv[0];
	switch(proceso){
		case "BROKER":
			atenderMensajeBroker(argc, argv);
			break;
		case "TEAM":
			atenderMensajeTeam(argc, argv);
			break;
		case "GAMECARD":
			atenderMensajeGameCard(argc, argv);
			break;
	}

	/*int conexion = crear_conexion(ip,puertoEnString);


	enviar_mensaje(conexion,NEW_POKEMON); //hardcodeado NEWPOKEMON
	recibir_mensaje(conexion);
	terminar_programa(conexion);
	*/
}

void terminar_programa(int conexion){
	liberar_conexion(conexion);
}

void controlar_cant_argumentos(int argc){
	if(argc < 3){
			error_show("Error: cantidad de parametros insuficientes para efectuar cualquier accion \n");
			exit(2);
		}
}

void atenderMensajeBroker(argc, argv[]){
	char* puerto = obtener_puerto_broker();
	char* ip = obtener_ip_broker();

	int socket_conexion = crear_conexion(ip,puerto);

	char* codigo_mensaje = argv[1];
	switch(codigo_mensaje){

	case "NEW_POKEMON":
		cumple_cant_parametros(argc, 6);
		enviar_mensaje(socket_conexion, 1, argv); // 1 es el op_code de NEW_POKEMON

	}
}

void cumple_cant_parametros(int argc, int cantidad_necesaria){
	if(argc != cantidad_necesaria){
		error_show("Cantidad de parametros incorrectos. Se requieren %d para este mensaje pero se encontraron %d \n", cantidad_necesaria, argc);
		exit(3);
	}
}
