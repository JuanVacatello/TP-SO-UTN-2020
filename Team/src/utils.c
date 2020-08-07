#include"utils.h"
#include"configTeam.h"
#include<semaphore.h>



//------------INICIAMOS VARIABLES GLOBALES-------------//

void iniciar_vg(){

	armar_entrenadores();
	generar_objetivo_global();
	generar_atrapados_global();
	lista_de_entrenadores_deadlock = list_create();
	lista_de_entrenadores_ready = list_create();
	lista_ids_getPokemon = list_create();
	lista_pokemonesNoRequeridos_enElMapa = list_create();

	ciclosCpuTotales = 0;
	cambiosDeContexto = 0;
	deadlocksProducidos = 0;
	deadlocksResueltos = 0;

	flagListaAtrapados = 0;

	pthread_mutex_init(&mutex_planificador, NULL);
	//PONEMOS EL SEMÁFORO EN 0
	pthread_mutex_lock(&mutex_planificador);

//	pthread_mutex_init(&mutex_entrenador, NULL);
	//PONEMOS EL SEMÁFORO EN 0
//	pthread_mutex_lock(&mutex_entrenador);

	sem_init(&MUTEX_SUB,0,1);
	sem_init(&MUTEX_POKEMON_REQUERIDO,0,1);
	sem_init(&CONTADOR_ENTRENADORES,0,cantidad_entrenadores());
	sem_init(&MUTEX_ENTRENADORES,0,1);
	sem_init(&MUTEX_MENSAJES_GB,0,1);
	sem_init(&GET,0,1);
	sem_init(&GETT,0,1);
	sem_init(&SUB,0,1);

	verificar_entrenadores();

}


//------------CICLOS DE CPU---------------

void efectuar_ciclo_cpu(t_entrenador* entrenador, int ciclos){
	ciclos_de_cpu(ciclos);
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

int cantidad_entrenadores(){
	char** entrenadores = obtener_posiciones_entrenadores();
	int cantidad = 0;
	for(int i=0; entrenadores[i] != NULL; i++){
		cantidad = i;
	}
	return cantidad + 1;
}

int cantidad_de_elementos(char* pokemons){
	int contador = 0;
		for(int i=0; i < string_length(pokemons); i++){
			if(pokemons[i] =='|')
				contador++;
		}
	return contador+1;
}

bool esta_en_lista(t_list* lista_pokemones, char* especie){
   char* especie_pokemon;
   for (int i=0; i<list_size(lista_pokemones); i++){
      especie_pokemon = list_get(lista_pokemones, i);
		  if (strcmp(especie_pokemon, especie) == 0){
		  return true;
		}
	}
   return false;
}

bool existe_id_en_lista(uint32_t id_correlativo){
	int id = -1;
	for (int i=0; i<list_size(lista_ids_getPokemon); i++){
	   id = list_get(lista_ids_getPokemon, i);
	   if (id_correlativo == id){
		   return true;
		}
	}
	return false;
}

void suscribirse_a_colas(){

		sem_wait(&MUTEX_SUB);
		enviar_suscripcion_a_cola(2);
		puts("Me suscribo a APPEARED POKEMON");

		sem_wait(&MUTEX_SUB);
		puts("Me suscribo a CAUGHT POKEMON");
		enviar_suscripcion_a_cola(4);

		sem_wait(&MUTEX_SUB);
		enviar_suscripcion_a_cola(6);
		puts("Me suscribo a LOCALIZED POKEMON");

}

void verificar_entrenadores(){
	t_entrenador* entrenador;
	for(int i=0; i<cantidad_entrenadores(); i++){
		entrenador = list_get(lista_de_entrenadores,i);
		verificar_estado_entrenador(entrenador);
	}
}


int cantidad_de_elementos_coma(char* pokemons){
	int contador = 0;
		for(int i=0; i < string_length(pokemons); i++){
			if(pokemons[i] ==',')
				contador++;
		}
	return contador+1;
}

int cantidad_de_entrenadores_que_atrapan(){
	int cantidad;
	char** atrapados = obtener_pokemon_entrenadores();

	if(!string_is_empty(atrapados)){
		int n = cantidad_de_elementos_coma(atrapados);

		string_n_split(atrapados,n, ",");
	}
	return cantidad;
}

t_pokemon* remover_pokemon_lista(t_list* lista, char* pokemon){
	t_pokemon* pokemon_aux;
		for(int i = 0; i<list_size(lista); i++){
			pokemon_aux = list_get(lista, i);
			if(!strcmp(pokemon_aux->especie, pokemon)){
				pokemon_aux = list_remove(lista_de_entrenadores_ready, i);
				break;
			}
		}
	return pokemon_aux;
}







