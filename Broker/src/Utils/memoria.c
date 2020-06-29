#include "memoria.h"

t_mensaje_guardado* guardar_mensaje_en_memoria(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){

	t_mensaje_guardado* mensaje_nuevo = malloc(sizeof(t_mensaje_guardado));
	char* esquema_de_administracion = obtener_algoritmo_memoria();
	char* algoritmo_de_reemplazo = obtener_algoritmo_reemplazo();

	if(toda_la_memoria_esta_ocupada()){  // logica de implementacion de algoritmo de reemplazo

		if(!(strcmp(algoritmo_de_reemplazo, "FIFO"))){
			mensaje_nuevo = reemplazar_segun_FIFO(bloque_a_agregar_en_memoria, tamanio_a_agregar);
		}

		if(!(strcmp(algoritmo_de_reemplazo, "LRU"))){

		}

	} else { // logica de implementacion de algoritmo de no reemplazo we

		if(!(strcmp(esquema_de_administracion, "PARTICIONES"))){
			mensaje_nuevo = administracion_de_memoria_particiones(bloque_a_agregar_en_memoria,tamanio_a_agregar);
		}

		if(!(strcmp(esquema_de_administracion, "BS"))){
			mensaje_nuevo = administracion_de_memoria_particiones(bloque_a_agregar_en_memoria,tamanio_a_agregar);
		}
	}

	return mensaje_nuevo;
}

int toda_la_memoria_esta_ocupada(void){
	int booleano = 0;
	int tamanio_lista = list_size(elementos_en_memoria);
	int contador_ocupado = 0;
	int a_leer;

	for(int desplazamiento=0; desplazamiento<tamanio_lista; desplazamiento++){
		memcpy(&a_leer, memoria_principal + desplazamiento, sizeof(int));
		if(a_leer != 0){ // Hay que poner negativo!!
			contador_ocupado++;
		}
	}

	if(contador_ocupado == tamanio_de_memoria){ // Quiere decir que toda la memoria está ocupada
		booleano = 1;
	}

	return booleano;
}

t_mensaje_guardado* administracion_de_memoria_particiones(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){

	t_mensaje_guardado* mensaje_nuevo = malloc(sizeof(t_mensaje_guardado));
	char* algoritmo_particion_libre = obtener_algoritmo_particion_libre();

	if(!(strcmp(algoritmo_particion_libre, "FF"))){

		printf("El algoritmo de particion libre es %s", algoritmo_particion_libre);

		mensaje_nuevo = agregar_segun_first_fit(bloque_a_agregar_en_memoria, tamanio_a_agregar);
	}

	if(!(strcmp(algoritmo_particion_libre, "BF"))){

		printf("El algoritmo de particion libre es %s", algoritmo_particion_libre);

		mensaje_nuevo = agregar_segun_best_fit(bloque_a_agregar_en_memoria, tamanio_a_agregar);
	}

	return mensaje_nuevo;
}

t_mensaje_guardado* guardar_en_primera_posicion(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar, t_mensaje_guardado* mensaje_nuevo){
	memcpy(memoria_principal, bloque_a_agregar_en_memoria, tamanio_a_agregar); //usar semaforos xq es variable global
	mensaje_nuevo->byte_comienzo_ocupado = 0;
	mensaje_nuevo->tamanio_ocupado = tamanio_a_agregar;
	list_add(elementos_en_memoria, mensaje_nuevo);

	char* log = string_from_format("Se almacenó un mensaje en la posición %d de la memoria principal", mensaje_nuevo->byte_comienzo_ocupado);
	completar_logger(log, "BROKER", LOG_LEVEL_INFO); // LOG OBLIGATORIO

	return mensaje_nuevo;
}

t_mensaje_guardado* agregar_segun_first_fit(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){

	t_mensaje_guardado* mensaje_nuevo = malloc(sizeof(t_mensaje_guardado));
	int listsize = list_size(elementos_en_memoria);
	printf("En memoria hay %d mensajes", listsize);

	if(list_is_empty(elementos_en_memoria) || primera_posicion_vacia_y_entra(tamanio_a_agregar)){ // Si está vacía o la primera posición, agregar al principio de la memoria

		completar_logger("Estoy en if esta vacia", "BROKER", LOG_LEVEL_INFO);
		mensaje_nuevo = guardar_en_primera_posicion(bloque_a_agregar_en_memoria,tamanio_a_agregar,mensaje_nuevo);

	} else {

		completar_logger("Estoy en if no esta vacia", "BROKER", LOG_LEVEL_INFO);
		t_list* lista_ordenada = list_duplicate(elementos_en_memoria);
		list_sort(lista_ordenada, comparar_inicios_mensajes); // Ordena la lista de menor a mayor a partir de la posición de inicio donde están guardados los mensajes en memoria
		int tamanio_lista = list_size(lista_ordenada);

		for(int i=0; i<tamanio_lista; i++){ // Recorre para ver todos los mensajes guardados en la memoria principal

			t_mensaje_guardado* mensaje_a_leer = malloc(sizeof(t_mensaje_guardado));
			mensaje_a_leer = list_get(lista_ordenada, i);

			int desplazamiento = mensaje_a_leer->byte_comienzo_ocupado + mensaje_a_leer->tamanio_ocupado; // Me paro al final del primer mensaje guardado

			char* alogea = string_from_format("El desplazamient deberia ser %d y es: %d.", 24, desplazamiento);
			completar_logger(alogea, "Broker", LOG_LEVEL_INFO);


			int contador = 0; // Si está vacío el espacio siguiente, leerá ceros
			int cero;
			memcpy(&cero, memoria_principal + desplazamiento, sizeof(int));

			while(cero == 0 && contador < tamanio_a_agregar){

				char* alogear = string_from_format("El valor que lee es: %d.", cero);
				completar_logger(alogear, "Broker", LOG_LEVEL_INFO);

				desplazamiento += sizeof(int);
				memcpy(&cero, memoria_principal + desplazamiento, sizeof(int));
				contador++;
			}

			if(contador == tamanio_a_agregar){
				int posicion_inicial_nuevo_mensaje = mensaje_a_leer->byte_comienzo_ocupado + mensaje_a_leer->tamanio_ocupado;
				memcpy(memoria_principal + posicion_inicial_nuevo_mensaje, bloque_a_agregar_en_memoria, tamanio_a_agregar); //usar semaforos xq es variable global
				mensaje_nuevo->byte_comienzo_ocupado = posicion_inicial_nuevo_mensaje;
				mensaje_nuevo->tamanio_ocupado = tamanio_a_agregar;
				list_add(elementos_en_memoria, mensaje_nuevo);

				char* log = string_from_format("Se almacenó un mensaje en la posición %d de la memoria principal y ocupa %d bytes", mensaje_nuevo->byte_comienzo_ocupado, mensaje_nuevo->tamanio_ocupado);
				completar_logger(log, "BROKER", LOG_LEVEL_INFO); // LOG OBLIGATORIO

				break; // chequear
			}

			free(mensaje_a_leer);
		}
	}

	return mensaje_nuevo;
}

t_mensaje_guardado* agregar_segun_best_fit(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){

	t_mensaje_guardado* mensaje_nuevo = malloc(sizeof(t_mensaje_guardado));

	if(list_is_empty(elementos_en_memoria) || primera_posicion_vacia_y_entra(tamanio_a_agregar)){ // Si está vacía o la primera posición, agregar al principio de la memoria

		completar_logger("Estoy en if esta vacia", "BROKER", LOG_LEVEL_INFO);
		mensaje_nuevo = guardar_en_primera_posicion(bloque_a_agregar_en_memoria,tamanio_a_agregar,mensaje_nuevo);

	} else {

		completar_logger("Estoy en if no esta vacia", "BROKER", LOG_LEVEL_INFO);

		int tamanio_lista = list_size(elementos_en_memoria);
		int encontrado = 0;
		int tamanio_aceptable = tamanio_a_agregar; // Empieza buscando un tamaño igual al necesario, si no lo encuentra, busca uno mas grande por 1 byte, y asi

		while(encontrado == 0){

			for(int i=0; i<tamanio_lista; i++){ // Recorre para ver todos los mensajes guardados en la memoria principal

				t_mensaje_guardado* mensaje_a_leer = malloc(sizeof(t_mensaje_guardado));
				mensaje_a_leer = list_get(elementos_en_memoria, i);

				int desplazamiento = mensaje_a_leer->byte_comienzo_ocupado + mensaje_a_leer->tamanio_ocupado; // Me paro al final del primer mensaje guardado

				char* alogea = string_from_format("El desplazamient deberia ser %d y es: %d.", 24, desplazamiento);
				completar_logger(alogea, "Broker", LOG_LEVEL_INFO);

				int contador = 0; // Si está vacío el espacio siguiente, leerá ceros
				int cero;
				memcpy(&cero, memoria_principal + desplazamiento, sizeof(int));

				while(cero == 0 && contador < tamanio_aceptable){
					desplazamiento += sizeof(int);
					memcpy(&cero, memoria_principal + desplazamiento, sizeof(int));
					contador++;
				}

				if(contador == tamanio_aceptable){

					int posicion_inicial_nuevo_mensaje = mensaje_a_leer->byte_comienzo_ocupado + mensaje_a_leer->tamanio_ocupado;
					memcpy(memoria_principal + posicion_inicial_nuevo_mensaje, bloque_a_agregar_en_memoria, tamanio_a_agregar); //usar semaforos xq es variable global
					mensaje_nuevo->byte_comienzo_ocupado = posicion_inicial_nuevo_mensaje;
					mensaje_nuevo->tamanio_ocupado = tamanio_a_agregar;
					list_add(elementos_en_memoria, mensaje_nuevo);

					char* log = string_from_format("Se almacenó un mensaje en la posición %d de la memoria principal y ocupa %d bytes", mensaje_nuevo->byte_comienzo_ocupado, mensaje_nuevo->tamanio_ocupado);
					completar_logger(log, "BROKER", LOG_LEVEL_INFO); // LOG OBLIGATORIO

					encontrado = 1;

					break; // chequear
				}

				free(mensaje_a_leer);
			}

			tamanio_aceptable++;
		}
	}

	return mensaje_nuevo;
}

t_mensaje_guardado* reemplazar_segun_FIFO(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){

	// QUE PASA SI NO ME ALCANZA CON EL QUE ELIMINO??

	t_mensaje_guardado* mensaje_a_eliminar = malloc(sizeof(t_mensaje_guardado));
	mensaje_a_eliminar = list_remove(elementos_en_memoria, 0); // Eliminamos el primer mensaje que entró
	int posicion_inicial_nuevo_mensaje = mensaje_a_eliminar->byte_comienzo_ocupado;
	int cantidad_a_eliminar = mensaje_a_eliminar->tamanio_ocupado;

	memset(memoria_principal + posicion_inicial_nuevo_mensaje, 0, cantidad_a_eliminar);

	t_mensaje_guardado* mensaje_nuevo = malloc(sizeof(t_mensaje_guardado));
	memcpy(memoria_principal + posicion_inicial_nuevo_mensaje, bloque_a_agregar_en_memoria, tamanio_a_agregar); //usar semaforos xq es variable global
	mensaje_nuevo->byte_comienzo_ocupado = posicion_inicial_nuevo_mensaje;
	mensaje_nuevo->tamanio_ocupado = tamanio_a_agregar;
	list_add(elementos_en_memoria, mensaje_nuevo);

	char* log = string_from_format("Se almacenó un mensaje en la posición %d de la memoria principal y ocupa %d bytes", mensaje_nuevo->byte_comienzo_ocupado, mensaje_nuevo->tamanio_ocupado);
	completar_logger(log, "BROKER", LOG_LEVEL_INFO); // LOG OBLIGATORIO

	free(mensaje_a_eliminar);

	return mensaje_nuevo;
}

void administracion_de_memoria_buddy_system(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){

	// Necesito pensar
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

bool comparar_inicios_mensajes(t_mensaje_guardado* mensaje1, t_mensaje_guardado* mensaje2){
	//int booleano = 0;
	//if(mensaje1->byte_comienzo_ocupado < mensaje2->byte_comienzo_ocupado){
	//	booleano = 1;
	//}
	//return booleano;
	return mensaje1->byte_comienzo_ocupado < mensaje2->byte_comienzo_ocupado;
}

void mostrar_memoria_principal(void){

	int desplazamiento = 0;

	for(int i=0; i<tamanio_de_memoria; i++){
		uint32_t valor;
		memcpy(&valor, memoria_principal, sizeof(uint32_t));
		printf("Valor en posicio %d: %d", i, valor);
		desplazamiento += sizeof(uint32_t);
	}

}



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




