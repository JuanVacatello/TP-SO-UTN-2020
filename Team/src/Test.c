#include<stdio.h>
#include<stdlib.h>

#include "Team.h"
#include "entrenador.h"
#include "utils.h"
#include "configTeam.h"
#include "logTeam.h"
#include "conexiones.h"



int main(void) {

		leer_config();
		iniciar_logger();

		iniciar_vg();

		puts("aca entra1");
		puts("hola");

		pthread_create(&hilo_servidor, NULL , iniciar_servidor ,NULL);
		pthread_detach(hilo_servidor);

		pthread_create(&hilo_appeared_pokemon, NULL , appeared_pokemon_broker ,NULL);
		pthread_detach(hilo_appeared_pokemon);

		pthread_create(&hilo_caught_pokemon, NULL , caught_pokemon_broker ,NULL);
		pthread_detach(hilo_caught_pokemon);

		pthread_create(&hilo_localized_pokemon, NULL , localized_pokemon_broker ,NULL);
		pthread_detach(hilo_localized_pokemon);

		pthread_create(&hilo_planificador, NULL , planificacion ,NULL);
		pthread_join(hilo_planificador,NULL);

		puts("chau");

		return 0;
}

