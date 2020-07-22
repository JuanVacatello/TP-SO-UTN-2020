#include "Broker.h"
#include<time.h>
#include<errno.h>
#include<assert.h>

int main(void)
{
	inicializacion_de_variables_globales();

	iniciar_servidor();

	terminar_programa();

	return 0;
}

void inicializacion_de_variables_globales(void){

	leer_config();
	iniciar_logger();

	tamanio_de_memoria = obtener_tamanio_memoria();
	memoria_principal = malloc(tamanio_de_memoria);
	memset(memoria_principal,0,tamanio_de_memoria);

	contador_fallos = 0;
	timestamp = 0;
	mensaje_id = 0;

	char* esquema_de_administracion = obtener_algoritmo_memoria();

	elementos_en_memoria = list_create();

	if(!(strcmp(esquema_de_administracion, "BS"))){
		elementos_en_buddy = list_create();
		t_particion_buddy* particion = malloc(sizeof(t_particion_buddy));
		particion->tam_particion = obtener_tamanio_memoria();
		particion->comienzo_particion = 0;
		particion->final_de_particion = particion->tam_particion - 1;
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

	sem_init(&MUTEX_MENSAJE,0,0);
	sem_init(&MUTEX_LOGGER,0,1);
	sem_init(&MUTEX_MEM_PRIN,0,1);
	sem_init(&MUTEX_TIMESTAMP,0,1);
	sem_init(&MUTEX_FALLOS,0,1);
	sem_init(&MUTEX_LISTA,0,1);

	pthread_mutex_init(&mutex_suscripcion, NULL);
	pthread_mutex_lock(&mutex_suscripcion);
}

void terminar_programa(void){
	config_destroy(configBroker);
	log_destroy(logger);
	free(memoria_principal);
}
