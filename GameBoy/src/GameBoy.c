#include "GameBoy.h"

int main(int argc, char* argv[]) {

	controlar_cant_argumentos(argc);
	iniciar_logger();

	leer_config();
	uint32_t process_id = process_getpid();
	char* process_id_string = string_itoa(process_id);
	config_set_value(configGameBoy, "PROCESS_ID", process_id_string); // aunque haga esto me sigue dando pids distintos -> arreglar

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

	if(!(strcmp(proceso, "SUSCRIPTOR"))){ // no es proceso sino "codigo de operacion" pero bueno
		cumple_cant_parametros(argc, 4);

		char* puerto = obtener_puerto_broker();
		char* ip = obtener_ip_broker();
		int socket_conexion = crear_conexion(ip,puerto);

		enviar_mensaje_a_broker(socket_conexion, 0, argv); // 0 es el op_code de SUSCRIPTOR

		completar_logger("GameBoy se suscribió a la cola.", "GAMEBOY", LOG_LEVEL_INFO);

		//while(1){

		//recibir_mensaje(socket_conexion);
			//pthread_create(&hilo_recibir, NULL , recibir_mensaje ,socket_conexion);
			//pthread_join(hilo_recibir, NULL);
		//}

	}
	return 0;
}

void enviarMensajeBroker(int argc, char *argv[]){

	char* puerto = obtener_puerto_broker();
	char* ip = obtener_ip_broker();

	int socket_conexion = crear_conexion(ip,puerto);

	completar_logger("GameBoy se conectó a Broker.", "GAMEBOY", LOG_LEVEL_INFO); // LOG OBLIGATORIO

	char* codigo_mensaje = argv[2];

	if(!(strcmp(codigo_mensaje, "NEW_POKEMON"))){
		cumple_cant_parametros(argc, 7);
		enviar_mensaje_a_broker(socket_conexion, 1, argv); // 1 es el op_code de NEW_POKEMON
	}

	if(!(strcmp(codigo_mensaje, "APPEARED_POKEMON"))){
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

	//recibir_mensaje(socket_conexion);
}

void enviarMensajeTeam(int argc, char *argv[]){

	char* puerto = obtener_puerto_team();
	char* ip = obtener_ip_team();

	int socket_conexion = crear_conexion(ip,puerto);

	completar_logger("GameBoy se conectó a Team.", "GAMEBOY", LOG_LEVEL_INFO); // LOG OBLIGATORIO

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

	completar_logger("GameBoy se conectó a GameCard.", "GAMEBOY", LOG_LEVEL_INFO); // LOG OBLIGATORIO

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


