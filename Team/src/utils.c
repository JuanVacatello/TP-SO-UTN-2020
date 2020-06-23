#include"utils.h"
#include"configTeam.h"



//------------INICIAMOS VARIABLES GLOBALES-------------//

void iniciar_vg(void){

	armar_entrenadores();
	generar_objetivo_global();
	generar_atrapados_global();
	lista_de_entrenadores_deadlock = list_create();
	lista_de_entrenadores_ready = list_create();
	lista_de_pokemones_sueltos = list_create();
	//informar_pokemones_a_atrapar();

	pthread_mutex_init(&mutex_planificador, NULL);
	//PONEMOS EL SEMÁFORO EN 0
	pthread_mutex_lock(&mutex_planificador);

	//pthread_mutex_init(&mutex_conexion, NULL);
	//PONEMOS EL SEMÁFORO EN 1
	//pthread_mutex_lock(&mutex_conexion);

	pthread_mutex_init(&mutex_entrenador, NULL);
	//PONEMOS EL SEMÁFORO EN 0
	pthread_mutex_lock(&mutex_entrenador);

	//pokemones nuevos de prueba

	/*t_pokemon* pokemonPikachu = malloc(sizeof(t_pokemon));
	pokemonPikachu->especie = "Pikachu";
	pokemonPikachu->posicion.x = 6;
	pokemonPikachu->posicion.y = 6;
	//lista_de_pokemones_sueltos = list_create();
	list_add(lista_de_pokemones_sueltos, pokemonPikachu);

	t_pokemon* pokemonCharmander = malloc(sizeof(t_pokemon));
	pokemonCharmander->especie = "Charmander";
	pokemonCharmander->posicion.x = 1;
	pokemonCharmander->posicion.y = 1;
	//lista_de_pokemones_sueltos = list_create();
	list_add(lista_de_pokemones_sueltos, pokemonCharmander);

	t_pokemon* pokemonPidgey = malloc(sizeof(t_pokemon));
	pokemonPidgey->especie = "Pidgey";
	pokemonPidgey->posicion.x = 2;
	pokemonPidgey->posicion.y = 3;
	//lista_de_pokemones_sueltos = list_create();
	list_add(lista_de_pokemones_sueltos, pokemonPidgey);
	*/

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
		//int sem = 1;
		op_code cola = 2;
		while(cola <= 2){
			enviar_suscripcion_a_cola(cola); //APPEARED POKEMON
			cola += 2;
			sleep(1);
		}
		puts("Me suscribo a APPEARED POKEMON");
		puts("Me suscribo a CAUGHT POKEMON");
		puts("Me suscribo a LOCALIZED POKEMON");
}

