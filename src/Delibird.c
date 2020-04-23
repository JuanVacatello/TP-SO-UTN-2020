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
		enviar_mensaje("funciona!!!!!", conexion);

		//recibir mensaje
		char* mensaje = recibir_mensaje(conexion);

		//loguear mensaje recibido
		logger = completar_logger(mensaje, "servidor", LOG_LEVEL_INFO);

		terminar_programa(conexion, logger, config);
	}


	//TODO
	void terminar_programa(int conexion, t_log* logger, t_config* config)
	{
		log_destroy(logger);
		config_destroy(config);
		liberar_conexion(conexion);

		//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
	}

