#include "memoria.h"

t_mensaje_guardado* guardar_mensaje_en_memoria(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){

	t_mensaje_guardado* mensaje_nuevo;
	char* esquema_de_administracion = obtener_algoritmo_memoria();
	int tamanio_minimo_particion = obtener_tamanio_minimo_particion(); // Ver que hago con esto

	if(toda_la_memoria_esta_ocupada()){
		completar_logger("La memoria está llena", "BROKER", LOG_LEVEL_INFO);
		mensaje_nuevo = eliminar_y_compactar_hasta_encontrar(bloque_a_agregar_en_memoria, tamanio_a_agregar);
	}
	else {
		completar_logger("La memoria tiene espacio", "BROKER", LOG_LEVEL_INFO);

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

	char* log = string_from_format("Se almacenó un mensaje en la posición %d de la memoria principal.", mensaje_nuevo->byte_comienzo_ocupado);
	completar_logger(log, "BROKER", LOG_LEVEL_INFO); // LOG OBLIGATORIO

	return mensaje_nuevo;
}

// REEMPLAZO

t_mensaje_guardado* eliminar_y_compactar_hasta_encontrar(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){

	t_mensaje_guardado* mensaje_nuevo;
	int frecuencia_compactacion = obtener_frecuencia_compactacion();
	int encontrado = 0;

	// Si despues de compactar sigue sin haber lugar, procedo a eliminar algun mensaje

	while(encontrado == 0){ //Repito el procedimiento hasta poder guardar el nuevo mensaje

		int posicion_inicial_nuevo_mensaje = ejecutar_algoritmo_reemplazo(); //Elimino un mensaje de memoria

		contador_fallos++; //Cuento el primer fallo (por si la frecuencia de compactación es 2

		if(frecuencia_compactacion == -1 || frecuencia_compactacion == 0 || frecuencia_compactacion == 1){ //Compacto cada vez que se libera
			int desplazamiento = compactar_memoria();

			if(entra_en_hueco(tamanio_a_agregar, desplazamiento)){ //Si entra al final de la memoria (espacio libre) lo guardo
				mensaje_nuevo = guardar_en_posicion(bloque_a_agregar_en_memoria, tamanio_a_agregar, desplazamiento);
				encontrado = 1;
			}
		}

		if(frecuencia_compactacion != 2 && (contador_fallos%frecuencia_compactacion) == 0 && contador_fallos >=3){ //Si la frecuencia es 2 y los fallos son multiplo de 2
			int desplazamiento = compactar_memoria();

			char* log = string_from_format("El primer lugar vacio es %d.", desplazamiento);
			completar_logger(log, "BROKER", LOG_LEVEL_INFO);


			if(entra_en_hueco(tamanio_a_agregar, desplazamiento)){
				mensaje_nuevo = guardar_en_posicion(bloque_a_agregar_en_memoria, tamanio_a_agregar, desplazamiento);
				encontrado = 1;
			}
		}

		if(entra_en_hueco(tamanio_a_agregar, posicion_inicial_nuevo_mensaje) && encontrado == 0){ //Me fijo si en esa posicion tiene espacio suficiente
			completar_logger("Entra en hueco", "Broker", LOG_LEVEL_INFO);

			mensaje_nuevo = guardar_en_posicion(bloque_a_agregar_en_memoria, tamanio_a_agregar, posicion_inicial_nuevo_mensaje);
			encontrado = 1;
		}
	}

	return mensaje_nuevo;
}

int ejecutar_algoritmo_reemplazo(void){

	char* algoritmo_de_reemplazo = obtener_algoritmo_reemplazo();
	int posicion_liberada;

	if(!(strcmp(algoritmo_de_reemplazo, "FIFO"))){
		posicion_liberada = reemplazar_segun_FIFO();
	}

	if(!(strcmp(algoritmo_de_reemplazo, "LRU"))){
		posicion_liberada = reemplazar_segun_LRU();
	}

	char* log = string_from_format("Se eliminó de memoria principal la partición que empezaba en la posicion %d", posicion_liberada);
	completar_logger(log, "BROKER", LOG_LEVEL_INFO); // LOG OBLIGATORIO

	return posicion_liberada;
}

int reemplazar_segun_FIFO(void){

	t_mensaje_guardado* mensaje_a_eliminar;
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

	t_mensaje_guardado* mensaje_nuevo;
	char* algoritmo_particion_libre = obtener_algoritmo_particion_libre();

	if(!(strcmp(algoritmo_particion_libre, "FF"))){
		mensaje_nuevo = agregar_segun_first_fit(bloque_a_agregar_en_memoria, tamanio_a_agregar);
	}

	if(!(strcmp(algoritmo_particion_libre, "BF"))){
		mensaje_nuevo = agregar_segun_best_fit(bloque_a_agregar_en_memoria, tamanio_a_agregar);
	}

	return mensaje_nuevo;
}

t_mensaje_guardado* agregar_segun_first_fit(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){

	t_mensaje_guardado* mensaje_nuevo;
	//int listsize = list_size(elementos_en_memoria);

	if(list_is_empty(elementos_en_memoria) || primera_posicion_vacia_y_entra(tamanio_a_agregar)){ // Si está vacía o la primera posición esta vacia, agregar al principio de la memoria

		mensaje_nuevo = guardar_en_posicion(bloque_a_agregar_en_memoria, tamanio_a_agregar, 0);
	}
	else {

		t_list* lista_ordenada = list_duplicate(elementos_en_memoria);
		list_sort(lista_ordenada, comparar_inicios_mensajes); // Ordena la lista de menor a mayor a partir de la posición de inicio donde están guardados los mensajes en memoria
		int tamanio_lista = list_size(lista_ordenada);
		int se_guardo_el_mensaje = 0;

		for(int i=0; i<tamanio_lista; i++){ // Recorre para ver todos los mensajes guardados en la memoria principal

			t_mensaje_guardado* mensaje_a_leer;
			mensaje_a_leer = list_get(lista_ordenada, i);

			int desplazamiento = mensaje_a_leer->byte_comienzo_ocupado + mensaje_a_leer->tamanio_ocupado; // Me paro al final del primer mensaje guardado

			int contador = 4; // Si está vacío el espacio siguiente, leerá ceros
			int cero;
			memcpy(&cero, memoria_principal + desplazamiento, sizeof(int));

			while(cero == 0 && contador <= tamanio_a_agregar){
				desplazamiento += sizeof(int);
				memcpy(&cero, memoria_principal + desplazamiento, sizeof(int));
				contador+=4;
			}

			if(contador >= tamanio_a_agregar){

				int posicion_inicial_nuevo_mensaje = mensaje_a_leer->byte_comienzo_ocupado + mensaje_a_leer->tamanio_ocupado;
				mensaje_nuevo = guardar_en_posicion(bloque_a_agregar_en_memoria, tamanio_a_agregar, posicion_inicial_nuevo_mensaje);
				se_guardo_el_mensaje = 1;

				break;
			}

			//free(mensaje_a_leer);
		}

		if(se_guardo_el_mensaje == 0){

			int frecuencia_compactacion = obtener_frecuencia_compactacion();
			if(frecuencia_compactacion == -1 || frecuencia_compactacion == 0 || frecuencia_compactacion == 1){ //Compacto cada vez que se libera
				int desplazamiento = compactar_memoria();

				if(entra_en_hueco(tamanio_a_agregar, desplazamiento)){ //Si entra al final de la memoria (espacio libre) lo guardo
					mensaje_nuevo = guardar_en_posicion(bloque_a_agregar_en_memoria, tamanio_a_agregar, desplazamiento);
					se_guardo_el_mensaje = 1;
				}
			}

			if(frecuencia_compactacion != 2 && (contador_fallos%frecuencia_compactacion) == 0 && contador_fallos >= 3){ //Si la frecuencia es mayor a 1 y los fallos son multiplo de la frecuencia
				completar_logger("Estoy en compactar", "Broker", LOG_LEVEL_INFO);
				int desplazamiento = compactar_memoria();

				if(entra_en_hueco(tamanio_a_agregar, desplazamiento)){
					mensaje_nuevo = guardar_en_posicion(bloque_a_agregar_en_memoria, tamanio_a_agregar, desplazamiento);
					se_guardo_el_mensaje = 1;
				}
			}
		}

		if(se_guardo_el_mensaje == 0){
			mensaje_nuevo = eliminar_y_compactar_hasta_encontrar(bloque_a_agregar_en_memoria, tamanio_a_agregar);
		}
	}

	return mensaje_nuevo;
}

t_mensaje_guardado* agregar_segun_best_fit(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){

	t_mensaje_guardado* mensaje_nuevo;

	if(list_is_empty(elementos_en_memoria) || primera_posicion_vacia_y_entra(tamanio_a_agregar)){ // Si está vacía agregar al principio de la memoria

			completar_logger("Estoy en if esta vacia", "BROKER", LOG_LEVEL_INFO);

		mensaje_nuevo = guardar_en_posicion(bloque_a_agregar_en_memoria, tamanio_a_agregar, 0);

	} else {

			completar_logger("Estoy en if no esta vacia", "BROKER", LOG_LEVEL_INFO);

		int tamanio_lista = list_size(elementos_en_memoria);
		int encontrado = 0;
		int tamanio_aceptable = tamanio_a_agregar; // Empieza buscando un tamaño igual al necesario, si no lo encuentra, busca uno mas grande por 1 byte, y asi

		while(encontrado == 0 || tamanio_aceptable <= tamanio_de_memoria){

			for(int i=0; i<tamanio_lista; i++){ // Recorre para ver todos los mensajes guardados en la memoria principal

				t_mensaje_guardado* mensaje_a_leer;
				mensaje_a_leer = list_get(elementos_en_memoria, i);

				int desplazamiento = mensaje_a_leer->byte_comienzo_ocupado + mensaje_a_leer->tamanio_ocupado; // Me paro al final del primer mensaje guardado

				char* alogea = string_from_format("El desplazamient deberia ser %d y es: %d.", 24, desplazamiento);
				completar_logger(alogea, "Broker", LOG_LEVEL_INFO);

				int contador = 4; // Si está vacío el espacio siguiente, leerá ceros
				int cero;
				memcpy(&cero, memoria_principal + desplazamiento, sizeof(int));

				while(cero == 0 && contador <= tamanio_a_agregar){
					desplazamiento += sizeof(int);
					memcpy(&cero, memoria_principal + desplazamiento, sizeof(int));
					contador+=4;
				}

				if(contador >= tamanio_aceptable){

					int posicion_inicial_nuevo_mensaje = mensaje_a_leer->byte_comienzo_ocupado + mensaje_a_leer->tamanio_ocupado;
					mensaje_nuevo = guardar_en_posicion(bloque_a_agregar_en_memoria, tamanio_a_agregar, posicion_inicial_nuevo_mensaje);
					encontrado = 1;

					break;
				}

				//free(mensaje_a_leer);
			}

			tamanio_aceptable++;
		}

		if(encontrado == 0){
			mensaje_nuevo = eliminar_y_compactar_hasta_encontrar(bloque_a_agregar_en_memoria, tamanio_a_agregar);
		}
	}
	return mensaje_nuevo;
}

// BUDDY SYSTEM

t_mensaje_guardado* administracion_de_memoria_buddy_system(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){

	t_mensaje_guardado* mensaje_nuevo;

	int potencia_de_dos = 2;

	while(potencia_de_dos < tamanio_a_agregar && potencia_de_dos <= tamanio_de_memoria){
			potencia_de_dos = potencia_de_dos * 2;
	}

	// NO se como pensarlo

	return mensaje_nuevo;
}

// AUXILIARES

bool comparar_inicios_mensajes(t_mensaje_guardado* mensaje1, t_mensaje_guardado* mensaje2){
	return mensaje1->byte_comienzo_ocupado < mensaje2->byte_comienzo_ocupado;
}

bool comparar_timestamps_mensajes(t_mensaje_guardado* mensaje1, t_mensaje_guardado* mensaje2){
	return mensaje1->ultima_referencia < mensaje2->ultima_referencia;
}

void mostrar_memoria_principal(void){

	int desplazamiento = 0;

	for(int i=0; i<tamanio_de_memoria; i+=sizeof(uint32_t)){
		uint32_t valor;
		memcpy(&valor, memoria_principal + desplazamiento, sizeof(uint32_t));

		char* mensaje = string_from_format("Valor en posicion %d: %d.", i, valor);
		completar_logger(mensaje, "Broker", LOG_LEVEL_INFO); // LOG OBLIGATORIO

		//printf("Valor en posicio %d: %d", i, valor);
		desplazamiento += sizeof(uint32_t);
	}

}

int primera_posicion_vacia_y_entra(uint32_t tamanio_a_agregar){
	int cero;
	int boolean = 0;
	memcpy(&cero, memoria_principal, sizeof(int));

	if(cero == 0 && entra_en_hueco(tamanio_a_agregar, 0)){
		boolean = 1;
	}

	return boolean;
}

int toda_la_memoria_esta_ocupada(void){
	int booleano = 0;
	int tamanio_lista = list_size(elementos_en_memoria);
	int contador_ocupado = 0;
	int a_leer;

	for(int desplazamiento=0; desplazamiento<tamanio_lista; desplazamiento++){
		memcpy(&a_leer, memoria_principal + desplazamiento, sizeof(int));
		if(a_leer != 0){
			contador_ocupado++;
		}
	}

	if(contador_ocupado == tamanio_de_memoria){ // Quiere decir que toda la memoria está ocupada
		booleano = 1;
	}

	return booleano;
}

int entra_en_hueco(int tamanio_a_agregar, int posicion_libre){
	int boolean = 0;
	int contador = 4; // Si está vacío el espacio siguiente, leerá ceros
	int cero;
	int desplazamiento = 0;

	memcpy(&cero, memoria_principal + posicion_libre, sizeof(int));

	while(cero == 0 && contador <= tamanio_a_agregar){
		desplazamiento += sizeof(int);
		memcpy(&cero, memoria_principal + posicion_libre + desplazamiento, sizeof(int));
		contador+=4;
	}

	if(contador >= tamanio_a_agregar){
		boolean = 1;
	}

	return boolean;
}


t_mensaje_guardado* guardar_en_posicion(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar, int posicion){
	t_mensaje_guardado* mensaje_nuevo = malloc(sizeof(t_mensaje_guardado));

	memcpy(memoria_principal + posicion, bloque_a_agregar_en_memoria, tamanio_a_agregar); //usar semaforos xq es variable global
	mensaje_nuevo->byte_comienzo_ocupado = posicion;
	mensaje_nuevo->tamanio_ocupado = tamanio_a_agregar;

	return mensaje_nuevo;
}

int compactar_memoria(void){
	int desplazamiento = 0;
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

		memcpy(memoria_copactada + desplazamiento, bloque_a_agregar_en_memoria, tamanio_mensaje);

		mensaje_a_leer->byte_comienzo_ocupado = desplazamiento;

		desplazamiento += tamanio_mensaje;

		//free(bloque_a_agregar_en_memoria);
	}

	memoria_principal = memoria_copactada;

	completar_logger("Se compactó la memoria", "BROKER", LOG_LEVEL_INFO); // LOG OBLIGATORIO

	free(memoria_copactada);
	//free(mensaje_a_leer);

	return desplazamiento;
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




