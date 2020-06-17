#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>

#include "entrenador.h"

t_log* logger;

void iniciar_logger();
void completar_logger(char* mensaje, char* programa, t_log_level log_level);

void log_movimiento_entrenador(t_entrenador* entrenador);
void log_operacion_de_atrapar_exitosa(t_entrenador* entrenador);
