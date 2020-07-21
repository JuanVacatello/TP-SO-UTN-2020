#ifndef UTILS_DUMP_H_
#define UTILS_DUMP_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/txt.h>
#include<commons/temporal.h>
#include<string.h>
#include<pthread.h>
#include<semaphore.h>
#include"configBroker.h"
#include"logBroker.h"
#include<time.h>
#include"memoria.h"

void actualizar_dump_cache(void);
void llenar_el_dump(FILE* dump);
void llenar_inicio_dump(FILE* dump);
int si_el_anterior_esta_vacio(t_list* lista_ordenada, FILE* dump, int index, int contador_de_particiones);
char* crear_linea_a_agregar_ocupada(int inicio, int final, int tamanio, int lru, int cola, int id);
char* crear_linea_a_agregar_vacia(int inicio, int final, int tamanio);

#endif /* UTILS_DUMP_H_ */
