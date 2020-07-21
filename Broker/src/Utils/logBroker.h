#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>
#include<semaphore.h>

t_log* logger;
sem_t MUTEX_LOGGER;

void iniciar_logger();
void completar_logger(char* mensaje, char* programa, t_log_level log_level);

void log_conexion(int socket_proceso);
void log_mensaje_nuevo(void);
void log_suscripcion_nueva(int socket_suscriptor, int id_suscriptor, int cola_mensaje);
void log_envio_mensaje(int socket_suscriptor);
void log_confirmacion(int socket_suscriptor, int mensaje_id);
void log_almacenar_mensaje(int posicion_mensaje);
void log_particion_eliminada(int posicion_liberada);
void log_compactacion(void);
void log_asociacion_de_bloques(int posicion_1, int posicion_2);
void log_ejecucion_dump(void);
