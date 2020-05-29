#include "Broker.h"

int main(void)
{
	iniciar_servidor();

	return 0;
}

void terminar_programa(void)
{
	config_destroy(configBroker);
}
