#ifndef TEAM_H_
#define TEAM_H_


#include "utils/log.h"
#include "utils/config.h"
#include "utils/messageQueue.h"
#include "config.h"
#include "entrenador.h"

#include<stdio.h>
#include<stdlib.h>
#include<commons/collections/list.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>


/*typedef struct
/{
	t_list* objetivoGlobal;
	t_posicion* posicion;
	t_list* atrapadosGlobal;
	t_list* entrenadores;

} t_team;
*/


int cantidad_entrenadores(t_config* config);
void armar_entrenadores(t_config* config);
void generar_objetivo_global(t_config* config);
void terminar_programa(int conexion, t_log* logger, t_config* config);


#endif /* TEAM_H_ */
