#ifndef TEAM_H_
#define TEAM_H_


#include "entrenador.h"

#include<stdio.h>
#include<stdlib.h>
#include<commons/collections/list.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include "configTeam.h"

typedef enum{
	FIFO = 1,
	RR = 2,
	SJF_CD = 3,
	SJF_SD = 4

} op_planificacion;




int cantidad_entrenadores(void);
void armar_entrenadores(void);
void generar_objetivo_global(void);
void terminar_programa(int conexion, t_log* logger);
void generar_atrapados_global(void);
void planificacion(void);
void planificar_fifo(void);
void aparicion_pokemon(t_pokemon* pokemon);
int es_pokemon_requerido(t_pokemon* pokemon);

#endif /* TEAM_H_ */
