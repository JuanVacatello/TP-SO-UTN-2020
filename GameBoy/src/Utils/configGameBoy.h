/*
 * configGameBoy.h
 *
 *  Created on: 15 may. 2020
 *      Author: utnso
 */

#ifndef CONFIGGAMEBOY_H_
#define CONFIGGAMEBOY_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<commons/string.h>

t_config* configGameBoy;

void leer_config(void);
char* obtener_puerto_broker(void);
char* obtener_ip_broker(void);
char* obtener_puerto_team(void);
char* obtener_ip_team(void);
char* obtener_puerto_gamecard(void);
char* obtener_ip_gamecard(void);
char* obtener_log_file(void);

#endif /* CONFIGGAMEBOY_H_ */
