#include"utils.h"
#include"configTeam.h"



//------------INICIAMOS VARIABLES GLOBALES-------------//

void iniciar_vg(void){

	armar_entrenadores();
	generar_objetivo_global();
	generar_atrapados_global();
	lista_de_entrenadores_deadlock = list_create();
	lista_de_entrenadores_ready = list_create();

	pthread_mutex_init(&mutex_planificador, NULL);
	//PONEMOS EL SEMÁFORO EN 0
	pthread_mutex_lock(&mutex_planificador);

}




//------------CICLOS DE CPU---------------

void efectuar_ciclo_cpu(t_entrenador* entrenador, int ciclos){
	//contabilizar_ciclos( entrenador,ciclos);
	ciclos_de_cpu(ciclos);
}

void contabilizar_ciclos(t_entrenador* entrenador, int ciclos){
	entrenador->ciclos_de_cpu_totales += ciclos;
}

int transformarCiclos(int ciclos){
	int retardo;
	retardo = obtener_retardo_ciclo_cpu();
	return ciclos * retardo ;
}

void ciclos_de_cpu(int ciclos){
	int segundos = transformarCiclos(ciclos);
	sleep(segundos);
}

//----------FUNCIONES AUX ENTRENADORES----------

int cantidad_entrenadores(void){
	char** entrenadores = obtener_posiciones_entrenadores();
	int cantidad = 0;
	for(int i=0; entrenadores[i] != NULL; i++){
		cantidad = i;
	}
	return cantidad + 1;
}

int cantidad_de_elementos(char* pokemons){
	int contador = 0;
	//if(pokemons != NULL || pokemons != '\0'){
		for(int i=0; i < string_length(pokemons); i++){
			if(pokemons[i] =='|')
				contador++;
		}
	//}
	//else{
	//	return contador;
	//}
	return contador+1;
}




