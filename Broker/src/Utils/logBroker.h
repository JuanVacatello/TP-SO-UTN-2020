#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>

t_log* logger;

void iniciar_logger();
void completar_logger(char* mensaje, char* programa, t_log_level log_level);
