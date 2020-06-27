#include "memoria.h"

void guardar_mensaje_en_memoria(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){

	char* esquema_de_administracion = obtener_algoritmo_memoria();
	char* algoritmo_de_reemplazo = obtener_algoritmo_reemplazo();

	if(toda_la_memoria_esta_ocupada()){  // logica de implementacion de algoritmo de reemplazo
		if(!(strcmp(algoritmo_de_reemplazo, "FIFO"))){}
		if(!(strcmp(algoritmo_de_reemplazo, "LRU"))){}

	} else { // logica de implementacion de algoritmo de no reemplazo we

		if(!(strcmp(esquema_de_administracion, "PARTICIONES"))){

			printf("El esquema de administracion de memoria es %s", esquema_de_administracion);

			administracion_de_memoria_particiones(bloque_a_agregar_en_memoria,tamanio_a_agregar);
		}

		if(!(strcmp(esquema_de_administracion, "BS"))){
				administracion_de_memoria_particiones(bloque_a_agregar_en_memoria,tamanio_a_agregar);
		}
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

void administracion_de_memoria_particiones(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){

	char* algoritmo_particion_libre = obtener_algoritmo_particion_libre();

	if(!(strcmp(algoritmo_particion_libre, "FF"))){

		printf("El algoritmo de particion libre es %s", algoritmo_particion_libre);

		agregar_segun_first_fit(bloque_a_agregar_en_memoria, tamanio_a_agregar);
	}

	if(!(strcmp(algoritmo_particion_libre, "BF"))){

		printf("El algoritmo de particion libre es %s", algoritmo_particion_libre);

		agregar_segun_best_fit(bloque_a_agregar_en_memoria, tamanio_a_agregar);
	}

}

void guardar_en_primera_posicion(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar, t_mensaje_guardado* mensaje_nuevo){
	memcpy(memoria_principal, bloque_a_agregar_en_memoria, tamanio_a_agregar); //usar semaforos xq es variable global
	mensaje_nuevo->byte_comienzo_ocupado = 0;
	mensaje_nuevo->tamanio_ocupado = tamanio_a_agregar;
	list_add(elementos_en_memoria, mensaje_nuevo);

	char* log = string_from_format("Se almacenó un mensaje en la posición %d de la memoria principal", mensaje_nuevo->byte_comienzo_ocupado);
	completar_logger(log, "BROKER", LOG_LEVEL_INFO); // LOG OBLIGATORIO
}

void agregar_segun_first_fit(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){

	t_mensaje_guardado* mensaje_nuevo = malloc(sizeof(t_mensaje_guardado));
	int listsize = list_size(elementos_en_memoria);
	printf("En memoria hay %d mensajes", listsize);

	if(list_is_empty(elementos_en_memoria) || primera_posicion_vacia_y_entra(tamanio_a_agregar)){ // Si está vacía o la primera posición, agregar al principio de la memoria

		completar_logger("Estoy en if esta vacia", "BROKER", LOG_LEVEL_INFO);
		guardar_en_primera_posicion(bloque_a_agregar_en_memoria,tamanio_a_agregar,mensaje_nuevo);

	} else {

		completar_logger("Estoy en if no esta vacia", "BROKER", LOG_LEVEL_INFO);
		list_sort(elementos_en_memoria, comparar_inicios_mensajes); // Ordena la lista de menor a mayor a partir de la posición de inicio donde están guardados los mensajes en memoria
		int tamanio_lista = list_size(elementos_en_memoria);

		for(int i=0; i<tamanio_lista; i++){ // Recorre para ver todos los mensajes guardados en la memoria principal

			t_mensaje_guardado* mensaje_a_leer = malloc(sizeof(t_mensaje_guardado));
			mensaje_a_leer = list_get(elementos_en_memoria, i);

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

}

void agregar_segun_best_fit(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){

	t_mensaje_guardado* mensaje_nuevo = malloc(sizeof(t_mensaje_guardado));

	if(list_is_empty(elementos_en_memoria) || primera_posicion_vacia_y_entra(tamanio_a_agregar)){ // Si está vacía o la primera posición, agregar al principio de la memoria

		completar_logger("Estoy en if esta vacia", "BROKER", LOG_LEVEL_INFO);
		guardar_en_primera_posicion(bloque_a_agregar_en_memoria,tamanio_a_agregar,mensaje_nuevo);

	} else {

		completar_logger("Estoy en if no esta vacia", "BROKER", LOG_LEVEL_INFO);

		t_list* lista_de_pos_libres = list_create();
		int tamanio_lista = list_size(elementos_en_memoria);

		for(int i=0; i<tamanio_lista; i++){ // Recorre para ver todos los mensajes guardados en la memoria principal

			t_mensaje_guardado* mensaje_a_leer = malloc(sizeof(t_mensaje_guardado));
			mensaje_a_leer = list_get(elementos_en_memoria, i);

			int tamanio_libre;
			int comienzo_libre;
			int desplazamiento = mensaje_a_leer->byte_comienzo_ocupado + mensaje_a_leer->tamanio_ocupado; // Me paro al final del primer mensaje guardado

			char* alogea = string_from_format("El desplazamient deberia ser %d y es: %d.", 24, desplazamiento);
			completar_logger(alogea, "Broker", LOG_LEVEL_INFO);

			int contador = 0; // Si está vacío el espacio siguiente, leerá ceros
			int cero;
			memcpy(&cero, memoria_principal + desplazamiento, sizeof(int));

			if(cero == 0){ // Quiere decir que el byte siguiente esta libre
				comienzo_libre = desplazamiento;
			}

			while(cero == 0){
				desplazamiento += sizeof(int);
				memcpy(&cero, memoria_principal + desplazamiento, sizeof(int));
				contador++;
			}

			char* alogear = string_from_format("Leyo %d ceros.", contador);
			completar_logger(alogear, "Broker", LOG_LEVEL_INFO);

			tamanio_libre = contador; // Igualo el tamaño libre a la cantidad de ceros leidos

			t_posiciones_libres* nueva_pos_libre = malloc(sizeof(t_posiciones_libres));
			nueva_pos_libre->byte_comienzo_libre = comienzo_libre;
			nueva_pos_libre->tamanio_libre = tamanio_libre;
			list_add(lista_de_pos_libres, nueva_pos_libre);

			free(mensaje_a_leer);

		}

		// Una vez que termine de recorrer todos los mensajes

		list_sort(lista_de_pos_libres, comparar_tamanios_libres); // Ordena la lista de menor a mayor a partir de la posición de inicio donde están guardados los mensajes en memoria
		int tamanio = list_size(lista_de_pos_libres);
		int encontrado = 0;
		int tamanio_aceptable = tamanio_a_agregar; // Empieza buscando un tamaño igual al necesario, si no lo encuentra, busca uno mas grande por 1 byte, y asi

		while(encontrado == 0){
			t_posiciones_libres* posicion_a_leer = malloc(sizeof(t_posiciones_libres));

			for(int i=0; i<tamanio; i++){
				posicion_a_leer = list_get(lista_de_pos_libres, i);

				if(posicion_a_leer->tamanio_libre == tamanio_aceptable){

					int posicion_inicial_nuevo_mensaje = posicion_a_leer->byte_comienzo_libre;
					memcpy(memoria_principal + posicion_inicial_nuevo_mensaje, bloque_a_agregar_en_memoria, tamanio_a_agregar); //usar semaforos xq es variable global
					mensaje_nuevo->byte_comienzo_ocupado = posicion_inicial_nuevo_mensaje;
					mensaje_nuevo->tamanio_ocupado = tamanio_a_agregar;
					list_add(elementos_en_memoria, mensaje_nuevo);

					char* log = string_from_format("Se almacenó un mensaje en la posición %d de la memoria principal y ocupa %d bytes", mensaje_nuevo->byte_comienzo_ocupado, mensaje_nuevo->tamanio_ocupado);
					completar_logger(log, "BROKER", LOG_LEVEL_INFO); // LOG OBLIGATORIO

					encontrado = 1;

					break; // chequear
				}

			tamanio_aceptable++;

			}

		free(posicion_a_leer);

		}
	}
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

bool comparar_tamanios_libres(t_posiciones_libres* pos1, t_posiciones_libres* pos2){
	return pos1->tamanio_libre < pos2->tamanio_libre;
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

void administracion_de_memoria_buddy_system(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){
	//hola
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




