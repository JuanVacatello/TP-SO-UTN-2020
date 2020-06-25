#include<stdio.h>
#include<stdlib.h>

#include "Team.h"
#include "entrenador.h"
#include "utils.h"
#include "configTeam.h"
#include "logTeam.h"
#include "conexiones.h"


int main(void) {

		int algoritmo;
		char* logFile;
		int retardo, tiempoReconexion, estimacion, quantum;// alpha;

		leer_config();
		iniciar_logger();

// Conexion con broker
		iniciar_vg();
		suscribirse_a_colas();



		/*puts("aca entra00");

		retardo = obtener_retardo_ciclo_cpu();
		printf("Retardo: %d\n",retardo);
		tiempoReconexion = obtener_tiempo_reconexion();
		printf("Tiempo de reconexion: %d\n",tiempoReconexion);
		estimacion = obtener_estimacion_inicial();
		printf("Estimacion: %d\n",estimacion);
		quantum = obtener_quantum();
		printf("Quantum: %d\n",quantum);
		logFile = obtener_log_file();
		puts("Log path:");
		puts(logFile);
		algoritmo = obtener_algoritmo_planificacion();
		printf("Algoritmo de planificacion: %d\n",algoritmo);
		//alpha = obtener_alpha();
		//printf("%d\n",alpha);

		puts(" ");
*/
		//t_entrenador* entrenador = armar_entrenador(2);

/*
		printf("%d\n",entrenador->posicion.x);
		printf("%d\n",entrenador->posicion.y);
		puts(" ");
		puts(list_get(entrenador->objetivo,0));
		puts(list_get(entrenador->objetivo,1));
		puts(" ");
		puts(list_get(entrenador->atrapados,0));
*/
		puts("aca entra1");

		puts("hola");

		//pthread_create(&hilo_servidor, NULL , iniciar_servidor ,NULL);
		//pthread_detach(hilo_servidor);

		pthread_create(&hilo_planificador, NULL , planificacion ,NULL);
		pthread_join(hilo_planificador,NULL);
		//aparicion_pokemon(pokemon);
		puts("chau");

		return 0;
}


