#include<stdio.h>
#include<stdlib.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include <commons/string.h>

t_config* config;

void leer_config(void);
char* obtener_ip(void);
char* obtener_puerto(void);
int obtener_retardo_ciclo_cpu(void);
int obtener_tiempo_reconexion(void);
int obtener_algoritmo_planificacion(void);
int obtener_estimacion_inicial(void);
int obtener_quantum(void);
char* obtener_log_file(void);
char** obtener_posiciones_entrenadores(void);
char** obtener_pokemon_entrenadores(void);
char** obtener_objetivos_entrenadores(void);
double obtener_alpha(void);
int obtener_id_propio(void);
char* obtener_ip_team(void);
char* obtener_puerto_team(void);
