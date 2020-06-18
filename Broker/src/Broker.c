#include "Broker.h"

int main(void)
{
	leer_config();
	iniciar_logger();

	int tamanio_de_memoria = obtener_tamanio_memoria();
	void* memoria_principal = malloc(tamanio_de_memoria);
	memset(memoria_principal,0,64);

	int numero = 4;
	memcpy(memoria_principal,&numero, sizeof(int));

	int numero2=0;

	memcpy(&numero2, memoria_principal, sizeof(int));

	printf("%d", numero2);
	free(memoria_principal);
	/*
	 creacion_colas_de_mensajes();
	iniciar_servidor();

	 */

	return 0;
}

void terminar_programa(void)
{
	config_destroy(configBroker);
}

void creacion_colas_de_mensajes(void){

	suscriptores_new_pokemon = list_create();
	suscriptores_appeared_pokemon = list_create();
	suscriptores_catch_pokemon = list_create();
	suscriptores_caught_pokemon = list_create();
	suscriptores_get_pokemon = list_create();
	suscriptores_localized_pokemon = list_create();

	mensajes_de_cola_new_pokemon = list_create();
	mensajes_de_cola_appeared_pokemon = list_create();
	mensajes_de_cola_catch_pokemon = list_create();
	mensajes_de_cola_caught_pokemon = list_create();
	mensajes_de_cola_get_pokemon = list_create();
	mensajes_de_cola_localized_pokemon = list_create();
}
