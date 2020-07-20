#include "planificacion.h"


void planificar_fifo(void){

	t_entrenador* entrenador;
	t_entrenador* entrenador_test;


	while (1){

		pthread_mutex_lock(&mutex_planificador);

		while(!list_is_empty(lista_de_entrenadores_ready)){

			sem_wait(&MUTEX_ENTRENADORES);

			entrenador = list_get(lista_de_entrenadores_ready,0);
			entrenador->estado = EXEC;
			cambiosDeContexto++;

			while(list_size(entrenador->cola_de_acciones) > 0){
				if(list_size(entrenador->cola_de_acciones) == 1){
					ejecutar_entrenador(entrenador);
					break;
				}
				else{
					ejecutar_entrenador(entrenador);
				}
			}
		}

		while(deteccion_de_deadlock()){
			entrenador = preparar_intercambio();
			entrenador = list_remove(lista_de_entrenadores_ready,0);
			entrenador->estado = EXEC;
			cambiosDeContexto++;

			while(list_size(entrenador->cola_de_acciones) > 0){
				ejecutar_entrenador(entrenador);
			}
		}

		if(terminoTeam()){
			finalizoTeam();
			exit(10);
		}

	}
}



void planificar_sjf_sd(void){

	t_entrenador* entrenador_aux;
	t_entrenador* entrenador;

	while(1){

		pthread_mutex_lock(&mutex_planificador);

		while(!list_is_empty(lista_de_entrenadores_ready)){

			sem_wait(&MUTEX_ENTRENADORES);

			entrenador_aux = entrenador_con_menor_cpu();

			for(int i =0 ; i<list_size(lista_de_entrenadores_ready); i++){
				entrenador = list_get(lista_de_entrenadores_ready,i);
				if(entrenador->ID_entrenador == entrenador_aux->ID_entrenador){
					entrenador = list_get(lista_de_entrenadores_ready,i);
					break;
				}
			}

			entrenador->rafaga_anterior = 0;
			entrenador->estado = EXEC;
			cambiosDeContexto++;

			while(list_size(entrenador->cola_de_acciones) > 0){
				if(list_size(entrenador->cola_de_acciones) == 1){
					ejecutar_entrenador(entrenador);
					break;
				}
				else{
					ejecutar_entrenador(entrenador);
				}
			}
		}

		while(deteccion_de_deadlock()){
			entrenador = preparar_intercambio();
			entrenador = list_remove(lista_de_entrenadores_ready,0);
			entrenador->estado = EXEC;
			cambiosDeContexto++;

			while(list_size(entrenador->cola_de_acciones) > 0){
				ejecutar_entrenador(entrenador);
			}
		}

		if(terminoTeam()){
			finalizoTeam();
			exit(10);
		}

	}

}


void planificar_sjf_cd(void){

	t_entrenador* entrenador_aux;
	t_entrenador* entrenador;

	while(1){

		pthread_mutex_lock(&mutex_planificador);

		while(!list_is_empty(lista_de_entrenadores_ready)){

			sem_wait(&MUTEX_ENTRENADORES);

			entrenador_aux = entrenador_con_menor_cpu();

			for(int i =0 ; i<list_size(lista_de_entrenadores_ready); i++){
				entrenador = list_get(lista_de_entrenadores_ready,i);
				if(entrenador->ID_entrenador == entrenador_aux->ID_entrenador){
					entrenador = list_get(lista_de_entrenadores_ready,i);
					break;
				}
			}

			entrenador->rafaga_anterior = 0;
			entrenador->estado = EXEC;
			cambiosDeContexto++;

			while(list_size(entrenador->cola_de_acciones) > 0){
				entrenador_aux = entrenador_con_menor_cpu();

				if(entrenador_aux->ID_entrenador != entrenador->ID_entrenador){

					entrenador->estado = READY;

					remover_entrenador_ready(entrenador);
					list_add(lista_de_entrenadores_ready, entrenador);
					sem_post(&MUTEX_ENTRENADORES);
					break;

				}

				if(list_size(entrenador->cola_de_acciones) == 1){
					ejecutar_entrenador(entrenador);
					break;
				}
				else{
					ejecutar_entrenador(entrenador);
				}
			}
		}

			while(deteccion_de_deadlock()){
				entrenador = preparar_intercambio();
				entrenador = list_remove(lista_de_entrenadores_ready,0);
				entrenador->estado = EXEC;
				cambiosDeContexto++;


				while(list_size(entrenador->cola_de_acciones) > 0){
					ejecutar_entrenador(entrenador);
				}
			}

			if(terminoTeam()){
				finalizoTeam();
				exit(10);
			}
		}
}

// Ti = T(i­-1) * a + R(i­-1) * (1 -­ a) , donde a = obtener_alpha()

t_entrenador* entrenador_con_menor_cpu(){

	t_entrenador* entrenador_aux;
	t_entrenador* entrenador_menor_cpu;

	double estimacion_anterior, estimado_cpu, real_cpu;
	double alpha;
	double menor_cpu = -1;
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

	int ciclos = 0;
	t_list* cola_acciones = entrenador->cola_de_acciones;
	t_accion* accion_aux;

	for(int i = 0; i<list_size(entrenador->cola_de_acciones); i++){
		accion_aux = list_get(entrenador->cola_de_acciones,i);
		ciclos += accion_aux->ciclo_cpu;
	}

	return ciclos;
}


void planificar_rr(void){

	t_entrenador* entrenador;
	int quantum_remanente = obtener_quantum();
	t_accion* accion_aux;
	puts("aca entra4");

	while (1){


		pthread_mutex_lock(&mutex_planificador);

		puts("aca entra10");
															//quantum = 0
		while(!list_is_empty(lista_de_entrenadores_ready)){//, entrenador2= 2 acciones  entrenador 1 = 1 acciones

			sem_wait(&MUTEX_ENTRENADORES);

			if(quantum_remanente == 0){
				quantum_remanente = obtener_quantum();
			}
			entrenador = list_get(lista_de_entrenadores_ready,0);
			entrenador->estado = EXEC;
			cambiosDeContexto++;


			while(list_size(entrenador->cola_de_acciones) > 0 && quantum_remanente > 0){
				accion_aux = list_get(entrenador->cola_de_acciones,0);

				if(list_size(entrenador->cola_de_acciones) == 1){
					ejecutar_entrenador(entrenador);
					quantum_remanente-= accion_aux->ciclo_cpu;

					if(!list_is_empty(lista_de_entrenadores_ready)){
						entrenador = list_get(lista_de_entrenadores_ready, 0);
					}
				}
				else{
					if(accion_aux->ciclo_cpu <= quantum_remanente){
							ejecutar_entrenador(entrenador);
							quantum_remanente-= accion_aux->ciclo_cpu;
					}
					else{
						accion_aux->ciclo_cpu -= quantum_remanente;
						list_replace_and_destroy_element(entrenador->cola_de_acciones, 0, accion_aux, free);

						quantum_remanente = 0;
						//termina en -1 cuando tenemos acciones grandes
					}

					if(list_size(entrenador->cola_de_acciones) > 0 && quantum_remanente == 0){
						entrenador->estado = READY;
						cambiosDeContexto++;
						remover_entrenador_ready(entrenador);
						list_add(lista_de_entrenadores_ready, entrenador);
						sem_post(&MUTEX_ENTRENADORES);
					}
				}
			}
		}

		while(deteccion_de_deadlock()){
			entrenador = preparar_intercambio();
			entrenador = list_remove(lista_de_entrenadores_ready,0);
			entrenador->estado = EXEC;
			cambiosDeContexto++;

			while(list_size(entrenador->cola_de_acciones) > 0){
				ejecutar_entrenador(entrenador);
			}
		}

		if(terminoTeam()){
			finalizoTeam();
			exit(10);
		}
	}
}

//void destruir_accion(t_accion* accion){}	//NO BORRAR








