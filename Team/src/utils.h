#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<commons/collections/dictionary.h>
#include<string.h>
#include<pthread.h>
#include "entrenador.h"
#include "Team.h"



//DEFINO VARIABLES GLOBALES

t_list* lista_de_entrenadores;

t_dictionary* objetivo_global;

t_dictionary* atrapados_global;

t_list* lista_de_entrenadores_ready; // CUANDO LLEGA UN CAUGHT EL ENTRENADOR CON ESE ID ENTRA A ESTA LISTA

//t_list* pokemones_requeridos;


pthread_t hilo_planificador;

t_list* lista_de_pokemones_sueltos;

pthread_mutex_t mutex_planificador;

//pthread_mutex_t mutex_conexion;

pthread_t hilo_servidor;
pthread_t hilo_appeared_pokemon;
pthread_t hilo_caught_pokemon;
pthread_t hilo_localized_pokemon;

pthread_mutex_t mutex_entrenador;

t_list* lista_de_entrenadores_deadlock;

sem_t MUTEX_SUB;




typedef enum
{
	SUSCRIPTOR=0,
	NEW_POKEMON=1,
	APPEARED_POKEMON=2,
	CATCH_POKEMON=3,
	CAUGHT_POKEMON=4,
	GET_POKEMON=5,
	LOCALIZED_POKEMON=6,
	MENSAJE = 7
} op_code;


//////////////////////////////////////////////SOCKETS//////////////////////////////////////////////////////

void efectuar_ciclo_cpu(t_entrenador* entrenador, int ciclos);
int transformarCiclos(int ciclos);
void ciclos_de_cpu(int ciclos);
int cantidad_entrenadores(void);
void iniciar_vg(void);

int cantidad_de_elementos(char* pokemons);
bool esta_en_lista(t_list* lista_pokemones, char* especie);

void suscribirse_a_colas();


#endif /* CONEXIONES_H_ */
