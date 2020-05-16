#include<stdio.h>
#include<stdlib.h>

#include "Team.h"
#include "entrenador.h"
#include "utils.h"
#include "configTeam.h"


int main(void) {

		//int conexion;
		char* ip;
		char* puerto;
		char* algoritmo;
		char* logFile;
		int retardo, tiempoReconexion, estimacion, quantum;// alpha;


		//t_log* logger;


		//logger = iniciar_logger();
		//log_destroy(logger);
		//Loggear "soy un log"

		leer_config();


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
		puts(algoritmo);
		//alpha = obtener_alpha();
		//printf("%d\n",alpha);


		char** posiciones = obtener_posiciones_entrenadores();

		char* posicion = posiciones[0];

		char** objetivos = obtener_objetivos_entrenadores();

		char* objetivo = objetivos[0];

		char** atrapados = obtener_pokemon_entrenadores();

		char* atrapado = atrapados[0];

		puts(posicion);

		puts(objetivo);

		puts(atrapado);

		//obtener_posiciones_entrenadores();
		//obtener_pokemon_entrenadores();
		//obtener_objetivos_entrenadores();

		//armar_entrenadores();





		return 0;
}
