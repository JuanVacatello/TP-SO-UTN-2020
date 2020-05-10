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



int cantidad_entrenadores(void);
void armar_entrenadores(void);
void generar_objetivo_global(void);
void terminar_programa(int conexion, t_log* logger);


#endif /* TEAM_H_ */
