#ifndef CONFIGBROKER_H_
#define CONFIGBROKER_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<commons/string.h>

t_config* configBroker;

void leer_config(void);
int obtener_tamanio_memoria(void);
int obtener_tamanio_minimo_particion(void);
char* obtener_puerto_broker(void);
int obtener_frecuencia_compactacion(void);
char* obtener_algoritmo_memoria(void);
char* obtener_algoritmo_reemplazo(void);
char* obtener_algoritmo_particion_libre(void);
char* obtener_ip_broker(void);
char obtener_log_file(void); // en el ejemplo no aparece pero bueno

#endif /* CONFIGBROKER_H_ */
