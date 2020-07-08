#ifndef TEAM_H_
#define TEAM_H_


#include "entrenador.h"

#include<stdio.h>
#include<stdlib.h>
#include<commons/collections/list.h>
#include<commons/collections/queue.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include "configTeam.h"
#include<semaphore.h>

typedef enum{
	FIFO = 1,
	SJF_SD = 2,
	SJF_CD = 3,
	RR = 4

} op_planificacion;


int cantidad_entrenadores(void);
void armar_entrenadores(void);
void generar_objetivo_global(void);
void generar_atrapados_global(void);
void planificacion();
void aparicion_pokemon(t_pokemon* pokemon);
bool es_pokemon_requerido( char* pokemon);
void terminar_programa(int conexion, t_log* logger);
bool deteccion_de_deadlock();
t_accion* armar_accion(void(*accion)(void*), int ciclos);


bool hay_pokemones_sueltos(t_list*);
void eliminar_pokemon(void* pokemon);

bool TerminoTeam();
#endif /* TEAM_H_ */
