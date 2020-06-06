#include"utils.h"
#include"configTeam.h"


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




