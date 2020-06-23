#include "Broker.h"

int main(void)
{
	leer_config();
	iniciar_logger();

	int tamanio_de_memoria = obtener_tamanio_memoria();

	algoritmo_reemplazo = obtener_algoritmo_reemplazo();
	tamanio_minimo_particion = obtener_tamanio_minimo_particion();

	frecuencia_compactacion = obtener_frecuencia_compactacion();

	memoria_principal = malloc(tamanio_de_memoria); //variable global
	memset(memoria_principal,0,tamanio_de_memoria);

	//int numero = 4;
	//memcpy(memoria_principal,&numero, sizeof(int));

	//int numero2=0;
	//memcpy(&numero2, memoria_principal, sizeof(int));

	creacion_colas_de_mensajes();
	iniciar_servidor();

	free(memoria_principal);
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
