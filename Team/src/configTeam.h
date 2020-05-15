#include<stdio.h>
#include<stdlib.h>
#include<commons/config.h>
#include<commons/collections/list.h>

t_config* config;

void leer_config(void);
char* obtener_ip(void);
char* obtener_puerto(void);
int obtener_retardo_ciclo_cpu(void);
int obtener_tiempo_reconexion(void);
char* obtener_algoritmo_planificacion(void);
int obtener_estimacion_inicial(void);
int obtener_quantum(void);
char* obtener_log_file(void);
t_list* obtener_posiciones_entrenadores(void);
t_list* obtener_pokemon_entrenadores(void);
t_list* obtener_objetivos_entrenadores(void);
int obtener_alpha(void);
