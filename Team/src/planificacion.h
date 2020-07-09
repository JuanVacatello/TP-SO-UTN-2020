#include <stdio.h>
#include <stdlib.h>

#include<commons/collections/list.h>
#include<commons/collections/queue.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include "configTeam.h"
#include "entrenador.h"
#include "Team.h"
#include <pthread.h>
#include"utils.h"

void planificar_fifo(void);
void planificar_sjf_sd(void);
void planificar_sjf_cd(void);
void planificar_rr(void);
t_entrenador* entrenador_con_menor_cpu(void);
int ciclos_rafaga_a_ejecutar(t_entrenador* entrenador);


void destruir_accion(t_accion* accion);
