#include<stdio.h>
#include<stdlib.h>
#include<commons/config.h>
#include<commons/collections/list.h>

t_config* leer_config(void);
char obtener_ip(t_config* config);
char obtener_puerto(t_config* config);
int obtener_retardo_ciclo_cpu(t_config* config);
int obtener_tiempo_reconexion(t_config* config);
char obtener_algoritmo_planificacion(t_config* config);
int obtener_estimacion_inicial(t_config* config);
int obtener_quantum(t_config* config);
char obtener_log_file(t_config* config);
t_list* obtener_posiciones_entrenadores(t_config* config);
t_list* obtener_pokemon_entrenadores(t_config* config);
t_list* obtener_objetivos_entrenadores(t_config* config);
