#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>

t_log* logger;

void iniciar_logger();
void completar_logger(char* mensaje, char* programa, t_log_level log_level);

void log_intento_reintento_comunicacion_broker();
void log_intento_comunicacion_Broker_exitoso();
void log_reintento_comunicacion_Broker_fallido();
