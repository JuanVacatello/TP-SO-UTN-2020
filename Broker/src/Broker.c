#include "Broker.h"

int main(void)
{
	leer_config();
	iniciar_logger();
	inicializacion_de_variables_globales();

	iniciar_servidor();
//	pthread_create(&hilo_servidor, NULL , iniciar_servidor ,NULL);
//	pthread_join(hilo_servidor, NULL);

	terminar_programa();

	return 0;
}

void terminar_programa(void){
	config_destroy(configBroker);
	free(memoria_principal);
}

void inicializacion_de_variables_globales(void){

	tamanio_de_memoria = obtener_tamanio_memoria();
	memoria_principal = malloc(tamanio_de_memoria);
	memset(memoria_principal,0,tamanio_de_memoria);

	contador_fallos = 0;
	timestamp = 0;

	char* esquema_de_administracion = obtener_algoritmo_memoria();
	if(!(strcmp(esquema_de_administracion, "PARTICIONES"))){
		elementos_en_memoria = list_create();
	}
	if(!(strcmp(esquema_de_administracion, "BS"))){
		elementos_en_buddy = list_create();
		t_particion_buddy* particion = malloc(sizeof(t_particion_buddy));
		particion->tam_particion = obtener_tamanio_memoria();
		particion->comienzo_particion = 0;
		list_add(elementos_en_buddy, particion);
	}

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

	sem_init(&MUTEX_MENSAJE,0,1);
	sem_init(&MUTEX_LOGGER,0,1);
	sem_init(&MUTEX_MEM_PRIN,0,1);
	sem_init(&MUTEX_TIMESTAMP,0,1);
	sem_init(&MUTEX_FALLOS,0,1);
	sem_init(&MUTEX_LISTA,0,1);
}

