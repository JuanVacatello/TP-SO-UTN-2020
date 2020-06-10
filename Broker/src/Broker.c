#include "Broker.h"

int main(void)
{
	suscriptores_new_pokemon = list_create();
	suscriptores_appeared_pokemon = list_create();
	suscriptores_catch_pokemon = list_create();
	suscriptores_caught_pokemon = list_create();
	suscriptores_get_pokemon = list_create();
	suscriptores_localized_pokemon = list_create();

	iniciar_servidor();

	return 0;
}

void terminar_programa(void)
{
	config_destroy(configBroker);
}
