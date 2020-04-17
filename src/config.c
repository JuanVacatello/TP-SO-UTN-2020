#include "config.h"

t_config* leer_config(void)
{
	t_config* config;
	config = config_create("/home/utnso/Documentos/tp-2020-1c-wRAPPERS");
	//config_set_value(config,"IP", "127.0.0.1");
	//config_set_value(config,"PUERTO", "4444");
	//config_save_in_file(config,"/home/utnso/Documentos/Tp0/Game-watch-client/tp0.config");
	return config;
}
