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

	completar_logger("Nuevo mensaje", "GAMEBOY", LOG_LEVEL_INFO); // LOG OBLIGATORIO

	controlar_cant_argumentos(argc);

	iniciar_logger();
	leer_config();
	const char* proceso = argv[1];

	if(!(strcmp(proceso, "BROKER"))){
		enviarMensajeBroker(argc, argv);
	}

	if(!(strcmp(proceso, "TEAM"))){
		enviarMensajeTeam(argc, argv);
	}

	if(!(strcmp(proceso, "GAMECARD"))){
		enviarMensajeGameCard(argc, argv);
	}
}

void enviarMensajeBroker(int argc, char *argv[]){

	char* puerto = obtener_puerto_broker();
	char* ip = obtener_ip_broker();

	int socket_conexion = crear_conexion(ip,puerto);

	completar_logger("GameBoy se conectó a Broker", "GAMEBOY", LOG_LEVEL_INFO); // LOG OBLIGATORIO

	char* codigo_mensaje = argv[2];

	if(!(strcmp(codigo_mensaje, "NEW_POKEMON"))){
		cumple_cant_parametros(argc, 7);
		enviar_mensaje_a_broker(socket_conexion, 1, argv); // 1 es el op_code de NEW_POKEMON
	}

	if(!(strcmp(codigo_mensaje, "APPEARED_POKEMON"))){ // HAY QUE VER COMO HACEMOS LO DEL ID PARA QUE NO SE REPITA
		cumple_cant_parametros(argc, 7);
		enviar_mensaje_a_broker(socket_conexion, 2, argv); // 2 es el op_code de APPEARED_POKEMON
	}

	if(!(strcmp(codigo_mensaje, "CATCH_POKEMON"))){
		cumple_cant_parametros(argc, 6);
		enviar_mensaje_a_broker(socket_conexion, 3, argv); // 3 es el op_code de CATCH_POKEMON
	}

	if(!(strcmp(codigo_mensaje, "CAUGHT_POKEMON"))){
		cumple_cant_parametros(argc, 5);
		enviar_mensaje_a_broker(socket_conexion, 4, argv); // 4 es el op_code de CAUGHT_POKEMON
	}

	if(!(strcmp(codigo_mensaje, "GET_POKEMON"))){
		cumple_cant_parametros(argc, 4);
		enviar_mensaje_a_broker(socket_conexion, 5, argv); // 5 es el op_code de GET_POKEMON
	}

}

void enviarMensajeTeam(int argc, char *argv[]){

	char* puerto = obtener_puerto_team();
	char* ip = obtener_ip_team();

	int socket_conexion = crear_conexion(ip,puerto);

	completar_logger("GameBoy se conectó a Team", "GAMEBOY", LOG_LEVEL_INFO); // LOG OBLIGATORIO

	char* codigo_mensaje = argv[2];

	if(!(strcmp(codigo_mensaje, "APPEARED_POKEMON"))){

		cumple_cant_parametros(argc, 6);
		enviar_mensaje_a_team(socket_conexion, 2, argv); // 2 es el op_code de APPEARED_POKEMON

	} else {
		error_show("El proceso Team no puede recibir ese mensaje");
		exit(5);
	}
}

void enviarMensajeGameCard(int argc, char *argv[]){
	char* puerto = obtener_puerto_gamecard();
	char* ip = obtener_ip_gamecard();

	int socket_conexion = crear_conexion(ip,puerto);

	completar_logger("GameBoy se conectó a GameCard", "GAMEBOY", LOG_LEVEL_INFO); // LOG OBLIGATORIO

	char* codigo_mensaje = argv[2];

	if(!(strcmp(codigo_mensaje, "NEW_POKEMON"))){
		cumple_cant_parametros(argc, 8);
		enviar_mensaje_a_gamecard(socket_conexion, 1, argv); // 1 es el op_code de NEW_POKEMON
	}

	if(!(strcmp(codigo_mensaje, "CATCH_POKEMON"))){
		cumple_cant_parametros(argc, 7);
		enviar_mensaje_a_gamecard(socket_conexion, 3, argv); // 3 es el op_code de CATCH_POKEMON
	}

	if(!(strcmp(codigo_mensaje, "GET_POKEMON"))){
		cumple_cant_parametros(argc, 5);
		enviar_mensaje_a_gamecard(socket_conexion, 5, argv); // 5 es el op_code de GET_POKEMON
	}

}

void cumple_cant_parametros(int argc, int cantidad_necesaria){
	if(argc != cantidad_necesaria){
		error_show("Cantidad de parametros incorrectos. Se requieren %d parámetros para este mensaje pero se encontraron %d \n", cantidad_necesaria, argc);
		exit(3);
	}
}

void controlar_cant_argumentos(int argc){
	if(argc < 4 ){
		error_show("Cantidad de parametros insuficientes para efectuar cualquier accion \n");
		exit(2);
	}
}

void terminar_programa(int conexion){
	liberar_conexion(conexion);
	log_destroy(logger);
	config_destroy(configGameBoy);
}


/*MENSAJE PRUEBA
int main(){
	iniciar_logger();
	leer_config();


		completar_logger("estoy vivo" ,"GAMEBOY",LOG_LEVEL_INFO);

		char* ip = obtener_ip_broker();
		char* puerto = obtener_puerto_broker();

		int socket_conexion = crear_conexion(ip,puerto);
		completar_logger("Me conecte","GAMEBOY",LOG_LEVEL_INFO);

		int tamanio_paquete=0;

		void* a_enviar = iniciar_paquete_prueba(&tamanio_paquete);
		completar_logger("Cree un paquete","GAMEBOY",LOG_LEVEL_INFO);

		if(send(socket_conexion,a_enviar,tamanio_paquete,0) == -1){
			printf("error en enviar por el socket");
			exit(3);
		}
		completar_logger("Envie un paquete","GAMEBOY",LOG_LEVEL_INFO);
		free(a_enviar);
}
*/

