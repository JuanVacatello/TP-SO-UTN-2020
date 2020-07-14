#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>
#include <semaphore.h>


t_log* logger;
sem_t MUTEX_LOGGER;

void iniciar_logger();
void completar_logger(char* mensaje, char* programa, t_log_level log_level);
