/*
 * GameBoy.h
 *
 *  Created on: 3 may. 2020
 *      Author: utnso
 */

#ifndef GAMEBOY_H_
#define GAMEBOY_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<commons/string.h>
#include<commons/error.h>
#include "Utils/messageQueue.h"
#include "Utils/configGameBoy.h"
#include "Utils/logGameBoy.h"

void enviarMensajeBroker(int argc,char* argv[]);
void enviarMensajeTeam(int argc, char *argv[]);
void enviarMensajeGameCard(int argc, char *argv[]);

void cumple_cant_parametros(int argc, int cantidad_necesaria);
void controlar_cant_argumentos(int argc);

void terminar_programa(int conexion); //void terminar_programa(int conexion, t_log* logger, t_config* config);

#endif /* GAMEBOY_H_ */
