#include<stdio.h>
#include<stdlib.h>

#include "Team.h"
#include "entrenador.h"
#include "utils.h"
#include "configTeam.h"
#include "logTeam.h"


int main(void) {

		//int conexion;
		char* ip;
		char* puerto;
		int algoritmo;
		char* logFile;
		int retardo, tiempoReconexion, estimacion, quantum;// alpha;

		leer_config();
		iniciar_logger();



		ip = obtener_ip();
		puts(ip);
		puerto = obtener_puerto();
		puts(puerto);
		retardo = obtener_retardo_ciclo_cpu();
		printf("%d\n",retardo);
		tiempoReconexion = obtener_tiempo_reconexion();
		printf("%d\n",tiempoReconexion);
		estimacion = obtener_estimacion_inicial();
		printf("%d\n",estimacion);
		quantum = obtener_quantum();
		printf("%d\n",quantum);
		logFile = obtener_log_file();
		puts(logFile);
		algoritmo = obtener_algoritmo_planificacion();
		printf("%d\n",algoritmo);
		//alpha = obtener_alpha();
		//printf("%d\n",alpha);

		puts(" ");

		char** posiciones = obtener_posiciones_entrenadores();
		char** objetivos = obtener_objetivos_entrenadores();
		char** atrapados = obtener_pokemon_entrenadores();

		char* posicion;
		char* objetivo_entrenador;
		char* atrapado;

		for(int i = 0; i < 3; i++){
			posicion = posiciones[i];
			objetivo_entrenador = objetivos[i];
			atrapado = atrapados[i];
			printf("Posicion entrenador = ");
			puts(posicion);
			printf("Objetivos entrenador = ");
			puts(objetivo_entrenador);
			printf("Atrapados entrenador = ");
		    puts(atrapado);
		    puts(" ");

		}

		//obtener_posiciones_entrenadores();
		//obtener_pokemon_entrenadores();
		//obtener_objetivos_entrenadores();

		armar_entrenadores();


		t_entrenador* entrenador = armar_entrenador(0);


		printf("%d\n",entrenador->posicion->x);
		printf("%d\n",entrenador->posicion->y);
		puts(" ");
		puts(list_get(entrenador->objetivo,0));
		puts(list_get(entrenador->objetivo,1));
		puts(list_get(entrenador->objetivo,2));
		puts(list_get(entrenador->objetivo,3));
		puts(" ");
		puts(list_get(entrenador->atrapados,0));
		puts(list_get(entrenador->atrapados,1));
		puts(list_get(entrenador->atrapados,2));


		/*printf("%d\n",entrenador->posicion->x);
		printf("%d\n",entrenador->posicion->y);

		moverse_derecha(entrenador);
		moverse_derecha(entrenador);
		moverse_derecha(entrenador);
		moverse_izquierda(entrenador);
		moverse_arriba(entrenador);
		moverse_arriba(entrenador);
		moverse_arriba(entrenador);
		moverse_arriba(entrenador);
		*/


		puts("aca entra");

		t_pokemon* pokemonPikachu = malloc(sizeof(t_pokemon));
		pokemonPikachu->especie = "Pikachu";
		pokemonPikachu->posicion = malloc(sizeof(t_posicion));
		pokemonPikachu->posicion->x = 6;
		pokemonPikachu->posicion->y = 6;

		t_list* listaTest = list_create();
		list_add(listaTest, pokemonPikachu);
		puts("aca entra8");
		int pokemones = hay_pokemones_sueltos(listaTest);

		printf("%d\n",pokemones);


		puts("hola");
		pthread_t hilo_test;
		pthread_create(&hilo_test, NULL , (void *) planificacion ,NULL);
		pthread_join(hilo_test,NULL);
		//aparicion_pokemon(pokemon);
		puts("chau");


		//completar_logger("hola como estas", "TEAM", LOG_LEVEL_INFO);
		//terminar_programa(conexion, logger);


		return 0;
}
