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

		puerto = obtener_puerto();
		ip = obtener_ip();

		socket_conexion_broker = crear_conexion(ip,puerto);

		enviar_suscripcion_a_cola(socket_conexion_broker, 2);
		enviar_suscripcion_a_cola(socket_conexion_broker, 4);
		enviar_suscripcion_a_cola(socket_conexion_broker, 6);

		armar_entrenadores();
		generar_objetivo_global();
		generar_atrapados_global();

		//pedir_ubicacion_pokemones(socket_conexion_broker);

/*
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





		t_entrenador* entrenador = armar_entrenador(0);


		printf("%d\n",entrenador->posicion.x);
		printf("%d\n",entrenador->posicion.y);
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

		armar_entrenadores();
		generar_objetivo_global();
		generar_atrapados_global();

		puts("aca entra");

		t_pokemon* pokemonPikachu = malloc(sizeof(t_pokemon));
		pokemonPikachu->especie = "Pikachu";
		pokemonPikachu->posicion.x = 6;
		pokemonPikachu->posicion.y = 6;

		t_entrenador* entrenadorCercano = entrenador_mas_cercano(pokemonPikachu);
		printf("%d\n",entrenadorCercano->posicion.x);
		printf("%d\n",entrenadorCercano->posicion.y);


		if(es_pokemon_requerido(pokemonPikachu)){
			puts("true");
		}

		//printf("%d\n",numero);


		//lista_de_pokemones_sueltos = list_create();
		//list_add(lista_de_pokemones_sueltos, pokemonPikachu);
		puts("aca entra8");
		//int pokemones = hay_pokemones_sueltos(lista_de_pokemones_sueltos);

		//printf("%d\n",pokemones);

		/*t_pokemon* pokemonLista = malloc(sizeof(t_pokemon));

		pokemonLista = list_get(lista_de_pokemones_sueltos,0);
		char* especie = pokemonLista->especie;
		puts(especie)*/;
		puts("hola");
		pthread_create(&hilo_planificador, NULL , (void *) planificacion ,NULL);
		pthread_join(hilo_planificador,NULL);
		//aparicion_pokemon(pokemon);
		puts("chau");




		//completar_logger("hola como estas", "TEAM", LOG_LEVEL_INFO);
		//terminar_programa(conexion, logger);


		return 0;
}


void pedir_ubicacion_pokemones(socket_conexion_broker){

}

