#include "Broker.h"

int main(void)
{
	suscriptores_new_pokemon = list_create();
	iniciar_servidor();

	return 0;
}

void terminar_programa(void)
{
	config_destroy(configBroker);
}
