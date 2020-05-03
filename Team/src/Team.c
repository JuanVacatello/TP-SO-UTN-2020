#include "Team.h"


#include <stdio.h>
#include <stdlib.h>


int main(void) {

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

}
