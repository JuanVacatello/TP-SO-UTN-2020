#include "Broker.h"


int main(void)
{
	int tam_mem, tam_min_particion, puerto_b, frecuencia_compactacion;
	char* algoritmo_mem;
	char* algoritmo_reemplazo;
	char* algoritmo_particion_libre;
	char* ip_b;
	// char* log_file;

	t_config* config;

	config = leer_config();
	tam_mem = config_get_int_value(config, "TAMANO_MEMORIA");
	tam_min_particion = config_get_int_value(config, "TAMANO_MINIMO_PARTICION");
	puerto_b = config_get_int_value(config, "PUERTO_BROKER");
	frecuencia_compactacion = config_get_int_value(config, "FRECUENCIA_COMPACTACION");
	algoritmo_mem = config_get_string_value(config, "ALGORITMO_MEMORIA");
	algoritmo_reemplazo = config_get_string_value(config, "ALGORITMO_REEMPLAZO");
	algoritmo_particion_libre = config_get_string_value(config, "ALGORITMO_PARTICION_LIBRE");
	ip_b = config_get_string_value(config, "IP_BROKER");
	//log_file = config_get_string_value(config, "LOG_FILE"); --> en el ejemplo esto no esta

	// solo para probarlo: (deje el config lleno, hay q borrarlo desp de probar
	printf("%d\n", tam_mem);
	printf("%d\n", tam_min_particion);
	printf("%d\n", puerto_b);
	printf("%d\n", frecuencia_compactacion);
	puts(algoritmo_mem);
	puts(algoritmo_reemplazo);
	puts(algoritmo_particion_libre);
	puts(ip_b);
	//printf(log_file);

	iniciar_servidor();


	return 0;
}

t_config* leer_config(void)
{
	// chequear si el config ya existe?

	t_config* config;
	config = config_create("/home/utnso/Documentos/tp-2020-1c-wRAPPERS/Broker.config"); //ta bien asi el path?
	if(config == NULL){
		printf("No se pudo leer el archivo de configuración.");
		exit(2);
	}

	return config;
}

void terminar_programa(t_config* config)
{
	config_destroy(config);
}
