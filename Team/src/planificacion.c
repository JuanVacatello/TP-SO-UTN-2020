#include "planificacion.h"


void planificar_fifo(void){

	//pthread_mutex_init(&mutex_planificador);


	while (1){

		//El hilo se crea en el main principal

		puts("aca entra1");

		//prueba
		/*if(hay_pokemones_sueltos() == 1){
			puts("aca entra11");
			t_pokemon* pokeNuevo = list_remove(lista_de_pokemones_sueltos, 0);
			aparicion_pokemon(pokeNuevo);
		}
		else*/
		//
			pthread_mutex_lock(hilo_planificador);



		t_entrenador*  entrenador;

		entrenador = list_remove(lista_de_entrenadores_ready,0);

		ejecutar_entrenador(entrenador);

		/* IMPORTANTE HILOS
		 *
		 *	pthread_create(&hilo_entrenador, NULL, accion, entrenador);
		 *		la accion la sacamos de la cola de acciones (QUEUE_POP())
		 *	pthread_join(hilo_entrenador,NULL);
		 *		siempre abajo de un create tiene que haber un join
		*/

		//Le decimos al entrenador que ejecute sus acciones

		pthread_mutex_lock(&mutex_planificador);
	}

}

void planificar_sjf_sd(void){

	//pthread_mutex_init(&mutex_planificador);

	while(1){

		pthread_mutex_lock(&mutex_planificador);

		t_entrenador* entrenador;

		entrenador = entrenador_con_menor_cpu();

		ejecutar_entrenador(entrenador);

		pthread_mutex_lock(&mutex_planificador);
	}

}

// Ti = T(i­-1) * a + R(i­-1) * (1 -­ a) , donde a = obtener_alpha()

t_entrenador* entrenador_con_menor_cpu(){

	t_entrenador* entrenador_aux;
	t_entrenador* entrenador_menor_cpu;

	int estimacion_anterior, alpha, estimado_cpu, real_cpu;
	int menor_cpu = -1;
	alpha = obtener_alpha();

	for (int indice_entrenador=0; indice_entrenador<list_size(lista_de_entrenadores_ready); indice_entrenador++){

		entrenador_aux = list_get(lista_de_entrenadores_ready,indice_entrenador);

		if(entrenador_aux->ciclos_de_cpu_totales == 0)
			estimacion_anterior = obtener_estimacion_inicial();
		else
			estimacion_anterior = entrenador_aux->rafaga_anterior;

		//PREGUNTAR ESTO
		real_cpu = ciclos_rafaga_a_ejecutar(entrenador_aux);

		estimado_cpu = (estimacion_anterior * alpha) + real_cpu * (1-alpha);

		if(menor_cpu > estimado_cpu || menor_cpu == -1){
			menor_cpu = estimado_cpu;
			entrenador_menor_cpu = entrenador_aux;
		}
	}

	return entrenador_menor_cpu;
}

int ciclos_rafaga_a_ejecutar(t_entrenador* entrenador){

	int ciclos;
	t_queue* cola_acciones = entrenador->cola_de_acciones;
	t_accion* accion;

	for(int i=0; i<queue_size(cola_acciones); i++){
		 accion = queue_pop(cola_acciones);
		 ciclos += accion->ciclo_cpu;

	}
	return ciclos;
}

















