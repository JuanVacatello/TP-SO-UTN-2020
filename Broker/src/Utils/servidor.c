#include "servidor.h"

#include <pthread.h>

int cantidadProcesosregistrados = 0;
cola_mensaje newPokemon, appearedPokemon , catchPokemon ,
caughtPokemon, getPokemon, localizedPokemon;

void iniciar_servidor(void)
{
	iniciar_logger();
	completar_logger("me creo","BROKER",LOG_LEVEL_INFO);
	int socket_servidor;
    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(IP, PUERTO, &hints, &servinfo);

    for (p=servinfo; p != NULL; p = p->ai_next){
        if ((socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            continue;

        if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
            close(socket_servidor);
            continue;
        }
        break;
    }

	if(listen(socket_servidor, SOMAXCONN) == -1){
		close(socket_servidor);
		exit(6);
	}

    freeaddrinfo(servinfo);

    completar_logger("termino de establecer socket","BROKER",LOG_LEVEL_INFO);

    while(1)
    	esperar_cliente(socket_servidor); // hacerlo con select, NUNCA ESPERA ACTIVA
}

void esperar_cliente(int socket_servidor)
{
	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	pthread_create(&thread,NULL,(void*)serve_client,&socket_cliente);
	pthread_detach(thread);
}

void serve_client(int* socket)
{
	completar_logger("llego un mensaje","BROKER",LOG_LEVEL_INFO);
	uint32_t cod_op;
	if(recv(*socket, &cod_op, sizeof(uint32_t), MSG_WAITALL) == -1)
		cod_op = -1;

	char* mensaje = string_from_format("El codigo de operacion es: %d.", cod_op);
	completar_logger(mensaje, "Broker", LOG_LEVEL_INFO);

	process_request(cod_op, *socket);
}

void process_request(uint32_t cod_op, int socket_cliente) {
	//int size;
		switch (cod_op) {
		case 1:
			recibir_new_pokemon(socket_cliente);

			//msg = recibir_mensaje(socket_cliente, &size);
			//devolver_mensaje(msg, size, socket_cliente);
			//free(msg);
			break;
		case 0:
			completar_logger("Entre al SUSCRIBIRSE", "Broker", LOG_LEVEL_INFO);
			atenderMensajePrueba(socket_cliente);
			//atenderSuscripcion(socket_cliente);
			break;
		case -1:
			pthread_exit(NULL);
		case 7:
			atenderMensajePrueba(socket_cliente);
			break;
		}
}

/*void* recibir_mensaje(int socket_cliente, int* size)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));

	void* buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}
*/
void recibir_new_pokemon(int socket_cliente)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = NEW_POKEMON;
	completar_logger("recibo new pokemon","Broker",LOG_LEVEL_INFO);

	paquete->buffer = malloc(sizeof(t_buffer));

	recv(socket_cliente, &(paquete->buffer->size), sizeof(int), MSG_WAITALL);

	int caracteresPokemon;
	recv(socket_cliente, &caracteresPokemon, sizeof(int), MSG_WAITALL);
	completar_logger("recibo caracteres Pokemon","Broker",LOG_LEVEL_INFO);

	char* pokemon;
	recv(socket_cliente, &pokemon, caracteresPokemon, MSG_WAITALL);
	completar_logger(pokemon,"Broker",LOG_LEVEL_INFO);

	int posX;
	recv(socket_cliente, &posX, sizeof(int), MSG_WAITALL);
	completar_logger("recibo posX Pokemon ","Broker",LOG_LEVEL_INFO);

	int posY;
	recv(socket_cliente, &posY, sizeof(int), MSG_WAITALL);
	completar_logger("recibo posY Pokemon ","Broker",LOG_LEVEL_INFO);

	int cantidad;
	recv(socket_cliente, &cantidad, sizeof(int), MSG_WAITALL);
	completar_logger("recibo cantidad Pokemon ","Broker",LOG_LEVEL_INFO);

}

void* serializar_paquete(t_paquete* paquete, int bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

void devolver_mensaje(void* payload, int size, int socket_cliente)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = PRUEBA;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = size;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, payload, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void atenderSuscripcion(int socket_cliente)
{
	proceso* suscriptor = modelarProceso(socket_cliente);
	suscribirseAColas( suscriptor, socket_cliente );
}

void suscribirseACola(proceso* suscriptor,cola_mensaje cola_mensaje ){
	list_add(cola_mensaje.suscriptores, suscriptor);
}

proceso* modelarProceso(int socket){
	proceso* suscriptor = malloc(sizeof(proceso));
		suscriptor -> id = cantidadProcesosregistrados + 1 ;
		cantidadProcesosregistrados++;
		suscriptor -> socket_cliente = socket;
	return suscriptor;
}

void suscribirseAColas(proceso* suscriptor, int socket ){
	int size;
	recv(socket, &size, sizeof(int), MSG_WAITALL);

		for(int cantidad_colas = size/sizeof(op_code); cantidad_colas >0; cantidad_colas--){
			op_code cola_mensaje;

			recv(socket, &cola_mensaje,sizeof(op_code), MSG_WAITALL);
				switch(cola_mensaje){
				case 0:
					break; //OPCION SUSCRIBIRSE
				case 1:
					suscribirseACola(suscriptor, newPokemon);
					break;
				case 2:
					suscribirseACola(suscriptor, appearedPokemon);
						break;
				case 3:
					suscribirseACola(suscriptor, catchPokemon);
						break;
				case 4:
					suscribirseACola(suscriptor, caughtPokemon);
						break;
				case 5:
					suscribirseACola(suscriptor, getPokemon);
						break;
				case 6:
					suscribirseACola(suscriptor, localizedPokemon);
						break;
				case 7:
					break; //HAY QUE BORRAR LA OPCION MENSAJE
			}
		}
}

void atenderMensajePrueba(int socket_cliente){
	uint32_t tamanio;
	uint32_t numero;
	recv(socket_cliente, &tamanio, sizeof(uint32_t), MSG_WAITALL);
	recv(socket_cliente, &numero, tamanio, MSG_WAITALL);

	//char* mensaje1 = string_from_format("El tamanio es: %d.", tamanio);
	completar_logger("llega tamanio", "Broker", LOG_LEVEL_INFO);

	char* mensaje = string_from_format("El numero es: %d.", numero);
	completar_logger("llega numero", "Broker", LOG_LEVEL_INFO);

}
