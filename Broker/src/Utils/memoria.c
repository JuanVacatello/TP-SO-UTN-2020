#include "memoria.h"

t_mensaje_guardado* guardar_mensaje_en_memoria(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){

	t_mensaje_guardado* mensaje_nuevo = malloc(sizeof(t_mensaje_guardado));
	char* esquema_de_administracion = obtener_algoritmo_memoria();
	int tamanio_minimo_particion = obtener_tamanio_minimo_particion(); // Ver que hago con esto

	int encontrado = 0;

	if(toda_la_memoria_esta_ocupada()){

		int posicion_inicial_nuevo_mensaje = ejecutar_algoritmo_reemplazo();

		memcpy(memoria_principal + posicion_inicial_nuevo_mensaje, bloque_a_agregar_en_memoria, tamanio_a_agregar); //usar semaforos xq es variable global
		mensaje_nuevo->byte_comienzo_ocupado = posicion_inicial_nuevo_mensaje;
		mensaje_nuevo->tamanio_ocupado = tamanio_a_agregar;

		//falta ver que hago si no entra ahi

	} else {

		if(!(strcmp(esquema_de_administracion, "PARTICIONES"))){
			mensaje_nuevo = administracion_de_memoria_particiones(bloque_a_agregar_en_memoria, tamanio_a_agregar);
		}

		if(!(strcmp(esquema_de_administracion, "BS"))){
			mensaje_nuevo = administracion_de_memoria_buddy_system(bloque_a_agregar_en_memoria, tamanio_a_agregar);
		}

	}

	mensaje_nuevo->ultima_referencia = timestamp;
	timestamp++;
	list_add(elementos_en_memoria, mensaje_nuevo);

	char* log = string_from_format("Se almacenó un mensaje en la posición %d de la memoria principal", mensaje_nuevo->byte_comienzo_ocupado);
	completar_logger(log, "BROKER", LOG_LEVEL_INFO); // LOG OBLIGATORIO

	return mensaje_nuevo;
}



// REEMPLAZO

int ejecutar_algoritmo_reemplazo(void){

	char* algoritmo_de_reemplazo = obtener_algoritmo_reemplazo();
	int posicion_liberada;

	if(!(strcmp(algoritmo_de_reemplazo, "FIFO"))){
		posicion_liberada = reemplazar_segun_FIFO();
	}

	if(!(strcmp(algoritmo_de_reemplazo, "LRU"))){
		posicion_liberada = reemplazar_segun_LRU();
	}

	return posicion_liberada;
}

int reemplazar_segun_FIFO(void){

	t_mensaje_guardado* mensaje_a_eliminar = malloc(sizeof(t_mensaje_guardado));
	mensaje_a_eliminar = list_remove(elementos_en_memoria, 0); // Eliminamos el primer mensaje que entró
	int posicion_inicial_nuevo_mensaje = mensaje_a_eliminar->byte_comienzo_ocupado;
	int cantidad_a_eliminar = mensaje_a_eliminar->tamanio_ocupado;

	memset(memoria_principal + posicion_inicial_nuevo_mensaje, 0, cantidad_a_eliminar);

	free(mensaje_a_eliminar);

	return posicion_inicial_nuevo_mensaje;
}

int reemplazar_segun_LRU(void){

	t_list* lista_ordenada = list_duplicate(elementos_en_memoria);
	list_sort(lista_ordenada, comparar_timestamps_mensajes);

	t_mensaje_guardado* mensaje_a_eliminar = malloc(sizeof(t_mensaje_guardado));
	mensaje_a_eliminar = list_remove(elementos_en_memoria, 0); // Eliminamos el primer mensaje de la lista ordenada -> Timestamp mas bajo

	int posicion_inicial_nuevo_mensaje = mensaje_a_eliminar->byte_comienzo_ocupado;
	int cantidad_a_eliminar = mensaje_a_eliminar->tamanio_ocupado;

	memset(memoria_principal + posicion_inicial_nuevo_mensaje, 0, cantidad_a_eliminar);

	free(mensaje_a_eliminar);

	return posicion_inicial_nuevo_mensaje;
}

// PARTICIONES DINÁMICAS

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
		int se_guardo_el_mensaje = 0;

		while(se_guardo_el_mensaje == 0){
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

					se_guardo_el_mensaje = 1;

					break;
				}

				free(mensaje_a_leer);
			}

			if(se_guardo_el_mensaje == 1){
				break;
			}

			// Si no encontró lugar libre, copacta la memoria

			int desplazamiento = 0;
			compactar_memoria(&desplazamiento);

			if(tamanio_de_memoria - desplazamiento >= tamanio_a_agregar){ // Luego de compactar entra al final

				int posicion_inicial_nuevo_mensaje = desplazamiento;
				memcpy(memoria_principal + posicion_inicial_nuevo_mensaje, bloque_a_agregar_en_memoria, tamanio_a_agregar); //usar semaforos xq es variable global
				mensaje_nuevo->byte_comienzo_ocupado = posicion_inicial_nuevo_mensaje;
				mensaje_nuevo->tamanio_ocupado = tamanio_a_agregar;

				se_guardo_el_mensaje = 1;

			} else {

				int posicion_eliminada = ejecutar_algoritmo_reemplazo(); // Como no entra, elimina un mensaje y vuelve a buscar
			}
		}
	}

	return mensaje_nuevo;
}

void compactar_memoria(int *desplazamiento){

	t_list* lista_ordenada = list_duplicate(elementos_en_memoria);
	list_sort(lista_ordenada, comparar_inicios_mensajes); // Ordena la lista de menor a mayor a partir de la posición de inicio donde están guardados los mensajes en memoria
	int tamanio_lista = list_size(lista_ordenada);

	void* memoria_copactada = malloc(tamanio_de_memoria);
	t_mensaje_guardado* mensaje_a_leer;
	int inicio_mensaje;
	int tamanio_mensaje;

	for(int i=0; i<tamanio_lista; i++){

		mensaje_a_leer = list_get(lista_ordenada, i);
		inicio_mensaje = mensaje_a_leer->byte_comienzo_ocupado;
		tamanio_mensaje = mensaje_a_leer->tamanio_ocupado;

		void* bloque_a_agregar_en_memoria = malloc(tamanio_mensaje);
		memcpy(bloque_a_agregar_en_memoria, memoria_principal + inicio_mensaje, tamanio_mensaje);

		memcpy(memoria_copactada + *desplazamiento, bloque_a_agregar_en_memoria, tamanio_mensaje);
		*desplazamiento += tamanio_mensaje;

		free(bloque_a_agregar_en_memoria);
	}

	memoria_principal = memoria_copactada;
	free(memoria_copactada);
	free(mensaje_a_leer);
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

// BUDDY SYSTEM

t_mensaje_guardado* administracion_de_memoria_buddy_system(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){

	t_mensaje_guardado* mensaje_nuevo = malloc(sizeof(t_mensaje_guardado));

	int potencia_de_dos = 2;

	while(potencia_de_dos < tamanio_a_agregar && potencia_de_dos <= tamanio_de_memoria){
			potencia_de_dos = potencia_de_dos * 2;
	}

	// NO se como pensarlo

	return mensaje_nuevo;
}

// AUXILIARES

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
	return mensaje1->byte_comienzo_ocupado < mensaje2->byte_comienzo_ocupado;
}

bool comparar_timestamps_mensajes(t_mensaje_guardado* mensaje1, t_mensaje_guardado* mensaje2){
	return mensaje1->ultima_referencia < mensaje2->ultima_referencia;
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




