#include "memoria.h"

void guardar_mensaje_en_memoria(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){

	char* esquema_de_administracion = obtener_algoritmo_memoria();

	if(!(strcmp(esquema_de_administracion, "PARTICIONES"))){
		administracion_de_memoria_particiones(bloque_a_agregar_en_memoria,tamanio_a_agregar);
	}


	/* Si quiero sacarlo de memoria y leerlo:
	void* mostrar_memoria = malloc(tamanio_a_agregar);
	memset(mostrar_memoria,0,tamanio_a_agregar);

	int desplazamiento = 0;
	memcpy(mostrar_memoria, memoria_principal, tamanio_a_agregar);

	uint32_t tamanio_pokemon;
	memcpy(&tamanio_pokemon, mostrar_memoria, sizeof(uint32_t));
	printf("lo pudo atrapar %d \n", tamanio_pokemon);
	desplazamiento += sizeof(uint32_t);
	*/



}

void administracion_de_memoria_particiones(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){

	char* algoritmo_particion_libre = obtener_algoritmo_particion_libre();

	if(!(strcmp(algoritmo_particion_libre, "FF"))){
		agregar_segun_first_fit(bloque_a_agregar_en_memoria, tamanio_a_agregar);
	}

	if(!(strcmp(algoritmo_particion_libre, "BF"))){
		agregar_segun_best_fit(bloque_a_agregar_en_memoria, tamanio_a_agregar);
	}

}

void agregar_segun_first_fit(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){

	t_mensaje_guardado mensaje_nuevo;

	if(!(list_is_empty(elementos_en_memoria))){ // Si está vacía, agregar al principio de la memoria

		t_list* lista_ordenada = list_create();
		t_list* lista_de_comienzos = list_create();
		lista_ordenada = list_sorted(elementos_en_memoria, comparar_bytes_mensajes);

/*		int tamanio_lista_mensajes = list_size(elementos_en_memoria);
		t_mensaje_guardado* mensaje_comparar = malloc(sizeof(t_mensaje_guardado));
		t_mensaje_guardado* mensaje_comparar_siguiente = malloc(sizeof(t_mensaje_guardado));
		t_mensaje_guardado* mensaje_elegido = malloc(sizeof(t_mensaje_guardado));

		uint32_t comienzo_comparar;
		uint32_t comienzo_comparar_siguiente;

		for(int i=0; i<tamanio_lista_mensajes; i++){

			mensaje_comparar = list_get(elementos_en_memoria,i);
			comienzo_comparar = mensaje_comparar->byte_comienzo_ocupado;

			mensaje_comparar = list_get(elementos_en_memoria,i+1);
			comienzo_comparar_siguiente = mensaje_comparar_siguiente->byte_comienzo_ocupado;

			if(comienzo_comparar < comienzo_comparar_siguiente && ){
				mensaje_elegido = list_get(elementos_en_memoria,i);
			}
		}

		if(mensaje_elegido->tamanio_ocupado >= tamanio_a_agregar){
		}

		free(mensaje_comparar);
		free(mensaje_comparar_siguiente);
		free(mensaje_elegido);
	}
*/
	memcpy(memoria_principal + desplazamiento_memoria_principal, bloque_a_agregar_en_memoria, tamanio_a_agregar); //usar semaforos xq es variable global
	mensaje_nuevo.byte_comienzo_ocupado = desplazamiento_memoria_principal;
	mensaje_nuevo.tamanio_ocupado = tamanio_a_agregar;

}





/*
	int cantidad_a_leer = sizeof(uint32_t);
	int cantidad_a_leer_encontrado = sizeof(uint32_t);
	int contador_bytes_disponibles = 0;

	for(int i=0; i<tamanio_de_memoria; i++){

		uint32_t valor_encontrado;
		memcpy(&valor_encontrado, memoria_principal, cantidad_a_leer);

		if(valor_encontrado == 0){

			while(valor_encontrado == 0){

				cantidad_a_leer_encontrado += sizeof(uint32_t);
				uint32_t valor_encontrado;
				memcpy(&valor_encontrado, memoria_principal, cantidad_a_leer_encontrado);
				contador_bytes_disponibles = 0;
			}

			if(contador_bytes_disponibles >= tamanio_a_agregar){
				memcpy(memoria_principal + cantidad_a_leer, bloque_a_agregar_en_memoria, tamanio_a_agregar); //usar semaforos xq es variable global
				break;
			}
		}

		cantidad_a_leer += sizeof(uint32_t);
		cantidad_a_leer_encontrado += sizeof(uint32_t);

	}

}
*/
}

void agregar_segun_best_fit(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){
//hola
}

bool comparar_bytes_mensajes(t_mensaje_guardado mensaje1, t_mensaje_guardado mensaje2){
	return mensaje1.byte_comienzo_ocupado < mensaje2.byte_comienzo_ocupado;
}
