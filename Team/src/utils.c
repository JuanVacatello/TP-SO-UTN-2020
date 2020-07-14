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
	lista_de_pokemones_sueltos = list_create();
	//informar_pokemones_a_atrapar();

	ciclosCpuTotales = 0;
	cambiosDeContexto = 0;
	deadlocksProducidos = 0;
	deadlocksResueltos = 0;

	pthread_mutex_init(&mutex_planificador, NULL);
	//PONEMOS EL SEMÁFORO EN 0
	pthread_mutex_lock(&mutex_planificador);

	pthread_mutex_init(&mutex_entrenador, NULL);
	//PONEMOS EL SEMÁFORO EN 0
	pthread_mutex_lock(&mutex_entrenador);

	sem_init(&MUTEX_SUB,0,1);
	sem_init(&MUTEX_POKEMON_REQUERIDO,0,1);
	sem_init(&CONTADOR_ENTRENADORES,0,cantidad_entrenadores());

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
		   else{
		return false;
		}
	}
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




//pokemones nuevos de prueba

/*t_pokemon* pokemonPikachu = malloc(sizeof(t_pokemon));
pokemonPikachu->especie = "Pikachu";
pokemonPikachu->posicion.x = 1;
pokemonPikachu->posicion.y = 1;
list_add(lista_de_pokemones_sueltos, pokemonPikachu);

t_pokemon* pokemonSquirtle = malloc(sizeof(t_pokemon));
pokemonSquirtle->especie = "Squirtle";
pokemonSquirtle->posicion.x = 9;
pokemonSquirtle->posicion.y = 7;
list_add(lista_de_pokemones_sueltos, pokemonSquirtle);

t_pokemon* pokemonOnix = malloc(sizeof(t_pokemon));
pokemonOnix->especie = "Onix";
pokemonOnix->posicion.x = 2;
pokemonOnix->posicion.y = 2;
list_add(lista_de_pokemones_sueltos, pokemonOnix);


t_pokemon* pokemonSquirtle1 = malloc(sizeof(t_pokemon));
pokemonSquirtle1->especie = "Squirtle";
pokemonSquirtle1->posicion.x = 3;
pokemonSquirtle1->posicion.y = 5;
list_add(lista_de_pokemones_sueltos, pokemonSquirtle1);

t_pokemon* pokemonGengar = malloc(sizeof(t_pokemon));
pokemonGengar->especie = "Gengar";
pokemonGengar->posicion.x = 7;
pokemonGengar->posicion.y = 5;
list_add(lista_de_pokemones_sueltos, pokemonGengar);
*/








