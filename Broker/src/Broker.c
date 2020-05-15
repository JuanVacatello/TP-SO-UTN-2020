#include "Broker.h"

int main(void)
{
	// solo para probar el config: (deje el archivo lleno, hay q borrarlo desp de probar
	leer_config();
	printf("%d\n", obtener_tamanio_memoria());
	printf("%d\n", obtener_tamanio_minimo_particion());
	puts(obtener_algoritmo_memoria());
	puts(obtener_algoritmo_reemplazo());
	puts(obtener_algoritmo_particion_libre());
	puts(obtener_ip_broker());
	printf("%d\n", obtener_puerto_broker());
	printf("%d\n", obtener_frecuencia_compactacion());

	//puts(log_file); no hay ejemplo

	iniciar_servidor();

	return 0;
}

void terminar_programa(void)
{
	config_destroy(configBroker);
}
