#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>

#include "entrenador.h"

t_log* logger;

void iniciar_logger();
void completar_logger(char* mensaje, char* programa, t_log_level log_level);


void log_cambio_de_entrenador_termino_anterior(t_entrenador* entrenador);
void log_cambio_de_entrenador_porque_anterior_espera_brokers(t_entrenador* entrenador);
void log_cambio_de_entrenador_por_trabajo_mas_corto(t_entrenador* entrenador);
void log_cambio_de_entrenador_por_fin_de_quantum(t_entrenador* entrenador);
void log_movimiento_entrenador(t_entrenador* entrenador);
void log_operacion_de_atrapar_exitosa(t_entrenador* entrenador);
void log_operacion_de_atrapar_fallida(t_entrenador* entrenador);
void log_operacion_de_intercambio(t_entrenador* entrenador1, t_entrenador* entrenador2,char* pokemon1, char* pokemon2);
void log_inicio_deteccion_deadlock();
void log_deadlock_detectado();
void log_deadlock_no_detectado();
void log_llego_mensaje_nuevo_appeared_pokemon(char* pokemon, int posX, int posY);
void log_llego_mensaje_nuevo_caught_pokemon(t_entrenador* entrenador, int pudoAtrapar);
void log_llego_mensaje_nuevo_localized_pokemon(char* pokemon, int posX, int posY);
void log_intento_reintento_comunicacion_broker();
void log_intento_comunicacion_Broker_exitoso();
void log_reintento_comunicacion_Broker_fallido();


void loguearMensje(char* mensaje);
