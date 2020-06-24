#include "Broker.h"

int main(void)
{
	leer_config();
	iniciar_logger();

	tamanio_de_memoria = obtener_tamanio_memoria();
	elementos_en_memoria = list_create();

	algoritmo_reemplazo = obtener_algoritmo_reemplazo();
	tamanio_minimo_particion = obtener_tamanio_minimo_particion();

	frecuencia_compactacion = obtener_frecuencia_compactacion();

	memoria_principal = malloc(tamanio_de_memoria);
	memset(memoria_principal,0,tamanio_de_memoria);

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

/* para probar con la memoria

	int elemento = 2;
	memcpy(memoria_principal, &elemento, sizeof(int));

	int elemento2 = 1;
	memcpy(memoria_principal + sizeof(int), &elemento2, sizeof(int));

	char* men = "Hola!!!";
	int largo = strlen(men);
	memcpy(memoria_principal + sizeof(int) + sizeof(int), &largo, sizeof(int));
	memcpy(memoria_principal + sizeof(int) + sizeof(int) + sizeof(int), men, largo);

	int mostrar;
	memcpy(&mostrar, memoria_principal, sizeof(int));

	int mostrar2;
	memcpy(&mostrar2, memoria_principal+sizeof(int), sizeof(int));

	int mostrar3;
	memcpy(&mostrar3, memoria_principal+sizeof(int)+sizeof(int), sizeof(int));
	char* mostrarmen = (char*)malloc(mostrar3+1);
	memcpy(mostrarmen, memoria_principal+sizeof(int)+sizeof(int)+sizeof(int), mostrar3+1);

	printf("Primer valor %d, segundo valor %d, tercer valor %d \n", mostrar, mostrar2, mostrar3);
	puts(mostrarmen);
	puts("Listo");
*/
	//int numero = 4;
	//memcpy(memoria_principal,&numero, sizeof(int));

	//int numero2=0;
	//memcpy(&numero2, memoria_principal, sizeof(int));
