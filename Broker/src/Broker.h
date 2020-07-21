#ifndef BROKER_H_
#define BROKER_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<commons/log.h>
#include<commons/config.h>
#include "Utils/configBroker.h"
#include "Utils/servidor.h"
#include "Utils/logBroker.h"
#include "Utils/memoria.h"
#include<pthread.h>

void terminar_programa(void);
void inicializacion_de_variables_globales(void);

#endif /* BROKER_H_ */

