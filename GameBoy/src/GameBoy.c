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
#include<commons/string.h>

int main(int argc, char* argv[]) {

	iniciar_logger();
	completar_logger("estoy vivo","GAMEBOY",LOG_LEVEL_INFO);



	controlar_cant_argumentos(argc);
	puts(argv[0]);

	//leer_config();

	char* proceso = argv[0];

	if(proceso == "BROKER")
			completar_logger("estoy en switch proceso","GAMEBOY", LOG_LEVEL_INFO);
			//enviarMensajeBroker(argc, argv);

	if(proceso == "TEAM")
			enviarMensajeTeam(argc, argv);

	if(proceso == "GAMECARD")
			enviarMensajeGameCard(argc, argv);
	}
	/*int conexion = crear_conexion(ip,puertoEnString);


	enviar_mensaje(conexion,NEW_POKEMON); //hardcodeado NEWPOKEMON
	recibir_mensaje(conexion);
	terminar_programa(conexion);
	*/

void terminar_programa(int conexion){
	liberar_conexion(conexion);
	log_destroy(logger);
	config_destroy(configGameBoy);
}

void controlar_cant_argumentos(int argc){
	if(argc <= 3 ){
			error_show("Error: cantidad de parametros insuficientes para efectuar cualquier accion \n");
			exit(2);
		}
}

void enviarMensajeBroker(int argc, char *argv[]){

	char* puerto = obtener_puerto_broker();
	char* ip = obtener_ip_broker();

	int socket_conexion = crear_conexion(ip,puerto);

	char* codigo_mensaje = argv[1];

	if(codigo_mensaje == "NEW_POKEMON"){
		cumple_cant_parametros(argc, 6);
		enviar_mensaje_a_broker(socket_conexion, 1, argv); // 1 es el op_code de NEW_POKEMON
	}

	if(codigo_mensaje == "APPEARED_POKEMON"){ //HAY QUE VER COMO HACEMOS LO DEL ID PARA QUE NO SE REPITA
		cumple_cant_parametros(argc, 6);
		enviar_mensaje_a_broker(socket_conexion, 2, argv); // 2 es el op_code de APPEARED_POKEMON
	}

	if(codigo_mensaje == "CATCH_POKEMON"){
		cumple_cant_parametros(argc, 5);
		enviar_mensaje_a_broker(socket_conexion, 3, argv); // 3 es el op_code de CATCH_POKEMON
	}

	if(codigo_mensaje == "CAUGHT_POKEMON"){
		cumple_cant_parametros(argc, 4);
		enviar_mensaje_a_broker(socket_conexion, 4, argv); // 4 es el op_code de CAUGHT_POKEMON
	}

	if(codigo_mensaje == "GET_POKEMON"){
		cumple_cant_parametros(argc, 3);
		enviar_mensaje_a_broker(socket_conexion, 5, argv); // 5 es el op_code de GET_POKEMON
	}

	// Broker solo recibe estos 5 mensajes del GameBoy
}

void enviarMensajeTeam(int argc, char *argv[]){

	char* puerto = obtener_puerto_team();
	char* ip = obtener_ip_team();

	int socket_conexion = crear_conexion(ip,puerto);

	char* codigo_mensaje = argv[1];

	if(codigo_mensaje == "APPEARED_POKEMON"){

		cumple_cant_parametros(argc, 5);
		enviar_mensaje_a_team(socket_conexion, 2, argv); // 2 es el op_code de APPEARED_POKEMON

	} else {
		error_show("El proceso Team no puede recibir ese mensaje");
		exit(5);
	}

	// Team solo recibe este mensaje del GameBoy
}

void enviarMensajeGameCard(int argc, char *argv[]){
	char* puerto = obtener_puerto_gamecard();
	char* ip = obtener_ip_gamecard();

	int socket_conexion = crear_conexion(ip,puerto);

	char* codigo_mensaje = argv[1];

	if(codigo_mensaje == "NEW_POKEMON"){
		cumple_cant_parametros(argc, 7);
		enviar_mensaje_a_gamecard(socket_conexion, 1, argv); // 1 es el op_code de NEW_POKEMON
	}

	if(codigo_mensaje == "CATCH_POKEMON"){
		cumple_cant_parametros(argc, 6);
		enviar_mensaje_a_gamecard(socket_conexion, 3, argv); // 3 es el op_code de CATCH_POKEMON
	}

	if(codigo_mensaje == "GET_POKEMON"){
		cumple_cant_parametros(argc, 4);
		enviar_mensaje_a_gamecard(socket_conexion, 5, argv); // 5 es el op_code de GET_POKEMON
	}
	// GameCard solo recibe estos 3 mensajes del GameBoy
}

void cumple_cant_parametros(int argc, int cantidad_necesaria){
	if(argc != cantidad_necesaria){
		error_show("Cantidad de parametros incorrectos. Se requieren %d parámetros para este mensaje pero se encontraron %d \n", cantidad_necesaria, argc);
		exit(3);
	}
}
