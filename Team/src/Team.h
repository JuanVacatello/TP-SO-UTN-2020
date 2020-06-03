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
void planificacion(void);
void planificar_fifo(void);
void aparicion_pokemon(t_pokemon* pokemon);
int es_pokemon_requerido(t_pokemon* pokemon);
void terminar_programa(int conexion, t_log* logger);

int hay_pokemones_sueltos();

#endif /* TEAM_H_ */
