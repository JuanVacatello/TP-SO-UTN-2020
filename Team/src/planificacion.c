#include "planificacion.h"


void planificar_fifo(void){

	t_entrenador* entrenador;
	/*PRUEBA
	*	t_pokemon* pokemonPikachu = malloc(sizeof(t_pokemon));
	*	pokemonPikachu->especie = "Pikachu";
	*	pokemonPikachu->posicion.x = 6;
	*	pokemonPikachu->posicion.y = 6;
	*
	*	lista_de_pokemones_sueltos = list_create();
	*	list_add(lista_de_pokemones_sueltos, pokemonPikachu);
	*/

		puts("aca entra4");
	while (1){

		if(deteccion_de_deadlock()){

			//hacer el intercambio

		}

		else{

		puts("aca entra5");

		//pthread_mutex_lock(&mutex_planificador);	//SE BLOQUEA BIEN


		while(!list_is_empty(lista_de_pokemones_sueltos)){
				t_pokemon* pokemon_nuevo = list_remove(lista_de_pokemones_sueltos,0);
				aparicion_pokemon(pokemon_nuevo);
		}

		puts("aca entra10");

		while(!list_is_empty(lista_de_entrenadores_ready)){

			t_entrenador* entrenador = list_remove(lista_de_entrenadores_ready,0);
			entrenador->estado = EXEC;

			while(list_size(entrenador->cola_de_acciones) > 0){
				ejecutar_entrenador(entrenador);
			}

		}


		//intentar atrapar pokemon --> mandar catch pokemon
		puts("SE BLOQUEA DESPUES DE EJECUTAR");
		pthread_mutex_lock(&mutex_planificador);
	}

}
}

void planificar_sjf_sd(void){

	t_entrenador* entrenador_aux;
	t_entrenador* entrenador;

	while(1){

		//pthread_mutex_lock(&mutex_planificador);

		while(!list_is_empty(lista_de_pokemones_sueltos)){
			t_pokemon* pokemon_nuevo = list_remove(lista_de_pokemones_sueltos,0);

			aparicion_pokemon(pokemon_nuevo);
		}

		while(!list_is_empty(lista_de_entrenadores_ready)){

			entrenador_aux = entrenador_con_menor_cpu();

			for(int i =0 ; i<list_size(lista_de_entrenadores_ready); i++){
				entrenador = list_get(lista_de_entrenadores_ready,i);
				if(entrenador->ID_entrenador == entrenador_aux->ID_entrenador){
					entrenador = list_remove(lista_de_entrenadores_ready,i);
				}
			}

			entrenador->rafaga_anterior = 0;
			entrenador->estado = EXEC;
			while(list_size(entrenador->cola_de_acciones) > 0){
				ejecutar_entrenador(entrenador);
				}
			}
		}

		pthread_mutex_lock(&mutex_planificador);
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

















