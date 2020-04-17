/*
 ============================================================================
 Name        : Delibird.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description :
 ============================================================================
 */

#include "Delibird.h"

#include <stdio.h>
#include <stdlib.h>


int main(void) {
	//---------------------------------------------------PARTE 2-------------------------------------------------------------/
		int conexion;
		char* ip;
		char* puerto;

		t_log* logger;
		t_config* config;

		//logger = iniciar_logger();
		//log_destroy(logger);
		//Loggear "soy un log"

		config = leer_config();
		ip = config_get_string_value(config,"IP");
		puerto = config_get_string_value(config,"PUERTO");
		puts("/n");
		puts(ip);
		puts(puerto);



		//---------------------------------------------------PARTE 3-------------------------------------------------------------/

		//antes de continuar, tenemos que asegurarnos que el servidor esté corriendo porque lo necesitaremos para lo que sigue.

		//crear conexion
		conexion = crear_conexion(ip, puerto);

		//enviar mensaje
		enviar_mensaje("los negros son iguales que todos los demas", conexion);

		//recibir mensaje
		char* mensaje = recibir_mensaje(conexion);

		//loguear mensaje recibido
		logger = completar_logger(mensaje, "servidor");

		terminar_programa(conexion, logger, config);
	}

	//TODO
	t_log* iniciar_logger(void)
	{
			t_log* logger;
			logger = log_create("tp0.log","tu vieja", true , LOG_LEVEL_INFO);
			log_info(logger,"soy un log");
			return logger;
	}

	t_log* completar_logger(char* mensaje,char* programa)
	{
		t_log* logger;
		logger = log_create("tp0.log",programa, true , LOG_LEVEL_INFO);
		log_info(logger,mensaje);
		return logger;
	}

	//TODO
	t_config* leer_config(void)
	{
		t_config* config;
		config = config_create("/home/utnso/Documentos/Tp0/Game-watch-client/tp0.config");
		//config_set_value(config,"IP", "127.0.0.1");
		//config_set_value(config,"PUERTO", "4444");
		//config_save_in_file(config,"/home/utnso/Documentos/Tp0/Game-watch-client/tp0.config");
		return config;
	}

	//TODO
	void terminar_programa(int conexion, t_log* logger, t_config* config)
	{
		log_destroy(logger);
		config_destroy(config);
		liberar_conexion(conexion);

		//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
	}

}
