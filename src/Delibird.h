#ifndef DLB_H_
#define DLB_H_

#include "log.h"
#include "config.h"
#include "messageQueue.h"

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>

t_log* completar_logger(char* mensaje,char* programa);
t_config* leer_config(void);
void terminar_programa(int conexion, t_log* logger, t_config* config);



#endif /* DLB_H_ */
