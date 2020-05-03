#include<stdio.h>
#include<stdlib.h>
#include<commons/config.h>

t_config leer_config(void);
char obtener_ip(t_config* config);
char obtener_puerto(t_config* config);
int obtener_retardo_ciclo_cpu(t_config* config);
