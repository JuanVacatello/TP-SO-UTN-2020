#include "memoria.h"

void guardar_mensaje_en_memoria(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){

	char* esquema_de_administracion = obtener_algoritmo_memoria();

	if(!(strcmp(esquema_de_administracion, "PARTICIONES"))){
		administracion_de_memoria_particiones(bloque_a_agregar_en_memoria,tamanio_a_agregar);
	}

	if(!(strcmp(esquema_de_administracion, "BS"))){
			administracion_de_memoria_particiones(bloque_a_agregar_en_memoria,tamanio_a_agregar);
		}
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

void guardar_en_primera_posicion(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar, t_mensaje_guardado* mensaje_nuevo){
	memcpy(memoria_principal, bloque_a_agregar_en_memoria, tamanio_a_agregar); //usar semaforos xq es variable global
	mensaje_nuevo->byte_comienzo_ocupado = 0;
	mensaje_nuevo->tamanio_ocupado = tamanio_a_agregar;
	list_add(elementos_en_memoria, mensaje_nuevo);
}

void agregar_segun_first_fit(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){

	t_mensaje_guardado* mensaje_nuevo = malloc(sizeof(t_mensaje_guardado));

	if(list_is_empty(elementos_en_memoria) || primera_posicion_vacia_y_entra(tamanio_a_agregar)){ // Si está vacía o la primera posición, agregar al principio de la memoria

		guardar_en_primera_posicion(bloque_a_agregar_en_memoria,tamanio_a_agregar,mensaje_nuevo);

	} else {

		list_sort(elementos_en_memoria, comparar_inicios_mensajes); // Ordena la lista de menor a mayor a partir de la posición de inicio donde están guardados los mensajes en memoria
		int tamanio_lista = list_size(elementos_en_memoria);

		for(int i=0; i<tamanio_lista; i++){ // Recorre para ver todos los mensajes guardados en la memoria principal

			t_mensaje_guardado* mensaje_a_leer = malloc(sizeof(t_mensaje_guardado));
			mensaje_a_leer = list_get(elementos_en_memoria, i);

			int desplazamiento = mensaje_a_leer->byte_comienzo_ocupado + mensaje_a_leer->tamanio_ocupado; // Me paro al final del primer mensaje guardado
			int contador = 0; // Si está vacío el espacio siguiente, leerá ceros
			int cero;
			memcpy(&cero, memoria_principal + desplazamiento, sizeof(int));

			while(cero == 0 && contador < tamanio_a_agregar){
				desplazamiento += sizeof(int);
				memcpy(&cero, memoria_principal + desplazamiento, sizeof(int));
				contador++;
			}

			if(contador == tamanio_a_agregar){
				memcpy(memoria_principal + mensaje_a_leer->byte_comienzo_ocupado + mensaje_a_leer->tamanio_ocupado, bloque_a_agregar_en_memoria, tamanio_a_agregar); //usar semaforos xq es variable global
				mensaje_nuevo->byte_comienzo_ocupado = desplazamiento_memoria_principal;
				mensaje_nuevo->tamanio_ocupado = tamanio_a_agregar;

				break; // chequear
			}

			free(mensaje_a_leer);
		}
	}

}

void agregar_segun_best_fit(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){

	t_mensaje_guardado* mensaje_nuevo = malloc(sizeof(t_mensaje_guardado));

	if(list_is_empty(elementos_en_memoria) || primera_posicion_vacia_y_entra(tamanio_a_agregar)){ // Si está vacía o la primera posición, agregar al principio de la memoria

		guardar_en_primera_posicion(bloque_a_agregar_en_memoria,tamanio_a_agregar,mensaje_nuevo);

	} else {} // Falta seguir pensando Best Fit
}

int primera_posicion_vacia_y_entra(uint32_t tamanio_a_agregar){

	int booleano = 0; // No entra
	int contador = 0;
	int desplazamiento = 0;

	int primera_posicion;
	memcpy(&primera_posicion, memoria_principal + desplazamiento, sizeof(int));

	while(primera_posicion == 0 && contador < tamanio_a_agregar){
		desplazamiento += sizeof(int);
		memcpy(&primera_posicion, memoria_principal + desplazamiento, sizeof(int));
		contador ++;
	}

	if(contador == tamanio_a_agregar){
		booleano = 1;
	}

	return booleano;
}

int comparar_inicios_mensajes(t_mensaje_guardado* mensaje1, t_mensaje_guardado* mensaje2){
	int booleano = 0;
	if(mensaje1->byte_comienzo_ocupado < mensaje2->byte_comienzo_ocupado){
		booleano = 1;
	}
	return booleano;
}

void administracion_de_memoria_buddy_system(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){}

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
*/	//


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

*/




