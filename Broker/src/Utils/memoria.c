#include "memoria.h"

t_mensaje_guardado* guardar_mensaje_en_memoria(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){

	sem_wait(&MUTEX_LISTA);

	t_mensaje_guardado* mensaje_nuevo;
	char* esquema_de_administracion = obtener_algoritmo_memoria();
	int tamanio_minimo_particion = obtener_tamanio_minimo_particion();

	if(tamanio_a_agregar < tamanio_minimo_particion){
		bloque_a_agregar_en_memoria = tratar_fragmentacion_interna(bloque_a_agregar_en_memoria, tamanio_a_agregar);
		tamanio_a_agregar = tamanio_minimo_particion;
	}

	if(toda_la_memoria_esta_ocupada()){
		mensaje_nuevo = eliminar_y_compactar_hasta_encontrar(bloque_a_agregar_en_memoria, tamanio_a_agregar);
	}
	else {
		if(!(strcmp(esquema_de_administracion, "PARTICIONES"))){
			mensaje_nuevo = administracion_de_memoria_particiones(bloque_a_agregar_en_memoria, tamanio_a_agregar);
		}

		if(!(strcmp(esquema_de_administracion, "BS"))){
			mensaje_nuevo = administracion_de_memoria_buddy_system(bloque_a_agregar_en_memoria, tamanio_a_agregar);
		}
	}

	sem_wait(&MUTEX_TIMESTAMP);
	timestamp++;
	mensaje_nuevo->ultima_referencia = timestamp;
	sem_post(&MUTEX_TIMESTAMP);

	list_add(elementos_en_memoria, mensaje_nuevo);

	log_almacenar_mensaje(mensaje_nuevo->byte_comienzo_ocupado);

	char* log = string_from_format("Con timestamp %d y tamaño ocupado %d.", mensaje_nuevo->ultima_referencia, mensaje_nuevo->tamanio_ocupado);
	completar_logger(log, "BROKER", LOG_LEVEL_INFO);

	sem_post(&MUTEX_LISTA);

	return mensaje_nuevo;
}

// REEMPLAZO Y COMPACTACION

t_mensaje_guardado* eliminar_y_compactar_hasta_encontrar(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){

	t_mensaje_guardado* mensaje_nuevo;
	int frecuencia_compactacion = obtener_frecuencia_compactacion();
	int encontrado = 0;
	char* algoritmo_particion_libre = obtener_algoritmo_particion_libre();

	sem_wait(&MUTEX_FALLOS);
	while(encontrado == 0){ //Repito el procedimiento hasta poder guardar el nuevo mensaje

		int posicion_inicial_nuevo_mensaje = ejecutar_algoritmo_reemplazo();

		// Si no entra, cuenta como fallo y procede a compactar si es momento
		int se_compacto = 0;
		if((frecuencia_compactacion == -1 || frecuencia_compactacion == 0 || frecuencia_compactacion == 1) && encontrado == 0){ //Compacto cada vez que se libera
			compactar_memoria();
			se_compacto = 1;
		}

		if(frecuencia_compactacion >= 2 && (contador_fallos%frecuencia_compactacion) == 0 && contador_fallos >=frecuencia_compactacion && encontrado == 0){ //Si la frecuencia es 2 y los fallos son multiplo de 2
			compactar_memoria();
			se_compacto = 1;
		}

		// Una vez compactada la memoria me fijo si ahora entra
		if((!(strcmp(algoritmo_particion_libre, "FF"))) && encontrado == 0){
			posicion_inicial_nuevo_mensaje = buscar_first_fit(&encontrado, bloque_a_agregar_en_memoria, tamanio_a_agregar);
		}

		if((!(strcmp(algoritmo_particion_libre, "BF"))) && encontrado == 0){
			posicion_inicial_nuevo_mensaje = buscar_best_fit(&encontrado, bloque_a_agregar_en_memoria, tamanio_a_agregar);
		}

		if(encontrado == 1){
			mensaje_nuevo = guardar_en_posicion(bloque_a_agregar_en_memoria, tamanio_a_agregar, posicion_inicial_nuevo_mensaje);
		}

		// Si se compacto y aun asi no entra, se vuelve a eliminar
	}
	sem_post(&MUTEX_FALLOS);

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

	log_particion_eliminada(posicion_liberada);

	if(list_is_empty(elementos_en_memoria)){
		log_compactacion();
	}

	contador_fallos++; // Cada vez que se elimina, sumo fallos

	return posicion_liberada;
}

int reemplazar_segun_FIFO(void){

	t_mensaje_guardado* mensaje_a_eliminar;
	mensaje_a_eliminar = list_remove(elementos_en_memoria, 0); // Eliminamos el primer mensaje que entró
	int posicion_inicial_nuevo_mensaje = mensaje_a_eliminar->byte_comienzo_ocupado;
	int cantidad_a_eliminar = mensaje_a_eliminar->tamanio_ocupado;

	sem_wait(&MUTEX_MEM_PRIN);
	memset(memoria_principal + posicion_inicial_nuevo_mensaje, 0, cantidad_a_eliminar);
	sem_post(&MUTEX_MEM_PRIN);

	free(mensaje_a_eliminar);

	return posicion_inicial_nuevo_mensaje;
}

int reemplazar_segun_LRU(void){

	t_list* lista_ordenada = list_duplicate(elementos_en_memoria);
	list_sort(lista_ordenada, comparar_timestamps_mensajes);

	t_mensaje_guardado* mensaje_aux;
	mensaje_aux = list_remove(lista_ordenada, 0); // Eliminamos el primer mensaje de la lista ordenada -> Timestamp mas bajo
	int posicion_inicial_nuevo_mensaje = mensaje_aux->byte_comienzo_ocupado;
	int cantidad_a_eliminar = mensaje_aux->tamanio_ocupado;

	int posicion_mensaje_a_eliminar = encontrar_mensaje_a_eliminar_por_posicion(posicion_inicial_nuevo_mensaje);
	t_mensaje_guardado* mensaje_a_eliminar = list_remove(elementos_en_memoria, posicion_mensaje_a_eliminar);

	sem_wait(&MUTEX_MEM_PRIN);
	memset(memoria_principal + posicion_inicial_nuevo_mensaje, 0, cantidad_a_eliminar);
	sem_post(&MUTEX_MEM_PRIN);

	//free(mensaje_aux);
	free(mensaje_a_eliminar);
	//free(lista_ordenada);

	return posicion_inicial_nuevo_mensaje;
}

void compactar_memoria(void){
	int desplazamiento = 0;
	t_list* lista_ordenada = list_duplicate(elementos_en_memoria);
	list_sort(lista_ordenada, comparar_inicios_mensajes); // Ordena la lista de menor a mayor a partir de la posición de inicio donde están guardados los mensajes en memoria
	int tamanio_lista = list_size(lista_ordenada);

	void* memoria_copactada = malloc(tamanio_de_memoria);
	t_mensaje_guardado* mensaje_a_leer;
	memset(memoria_copactada,0,tamanio_de_memoria);
	int inicio_mensaje;
	int tamanio_mensaje;

	sem_wait(&MUTEX_MEM_PRIN);
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

	memcpy(memoria_principal, memoria_copactada, tamanio_de_memoria);
	sem_post(&MUTEX_MEM_PRIN);

	log_compactacion();

	free(memoria_copactada);
	//free(mensaje_a_leer);

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

	int encontrado = 0;

		// Primero se busca posicion libre por First Fit
	int posicion_inicial_nuevo_mensaje = buscar_first_fit(&encontrado, bloque_a_agregar_en_memoria, tamanio_a_agregar);
	if(encontrado == 1){
		mensaje_nuevo = guardar_en_posicion(bloque_a_agregar_en_memoria, tamanio_a_agregar, posicion_inicial_nuevo_mensaje);
	}

		// Si no se encuentra, y es momento de compactar, se compacta
	//sem_wait(&MUTEX_FALLOS);
	if(encontrado == 0){
		int frecuencia_compactacion = obtener_frecuencia_compactacion();
		int se_compacto = 0;

		if(frecuencia_compactacion == -1 || frecuencia_compactacion == 0 || frecuencia_compactacion == 1){ //Compacto cada vez que se libera
			compactar_memoria();
			se_compacto = 1;
		}

		if(frecuencia_compactacion >= 2 && (contador_fallos%frecuencia_compactacion) == 0 && contador_fallos >= frecuencia_compactacion){ //Si la frecuencia es mayor a 1 y los fallos son multiplo de la frecuencia
			compactar_memoria();
			se_compacto = 1;
		}

		if(se_compacto == 1){
			posicion_inicial_nuevo_mensaje = buscar_first_fit(&encontrado, bloque_a_agregar_en_memoria, tamanio_a_agregar);
			if(encontrado == 1){
				mensaje_nuevo = guardar_en_posicion(bloque_a_agregar_en_memoria, tamanio_a_agregar, posicion_inicial_nuevo_mensaje);
			}
		}
	}
	//sem_post(&MUTEX_FALLOS);

		// Si luego de compactar tampoco entra, se elimina y compacta hasta que se encuentre
	if(encontrado == 0){
		mensaje_nuevo = eliminar_y_compactar_hasta_encontrar(bloque_a_agregar_en_memoria, tamanio_a_agregar);
	}

	return mensaje_nuevo;
}

int buscar_first_fit(int *encontrado, void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){

	int posicion_inicial_nuevo_mensaje = -1;

	t_list* lista_ordenada = list_duplicate(elementos_en_memoria);
	list_sort(lista_ordenada, comparar_inicios_mensajes); // Ordena la lista de menor a mayor a partir de la posición de inicio donde están guardados los mensajes en memoria
	int tamanio_lista = list_size(lista_ordenada);

	if(list_is_empty(elementos_en_memoria) || primera_posicion_vacia_y_entra(tamanio_a_agregar)){ // Si está vacía o la primera posición esta vacia, agregar al principio de la memoria

		posicion_inicial_nuevo_mensaje = 0;
		*encontrado = 1;
	}
	else {

		//sem_wait(&MUTEX_MEM_PRIN);
		for(int i=0; i<tamanio_lista; i++){ // Recorre para ver todos los mensajes guardados en la memoria principal

			t_mensaje_guardado* mensaje_a_leer;
			mensaje_a_leer = list_get(lista_ordenada, i);

			int desplazamiento = mensaje_a_leer->byte_comienzo_ocupado + mensaje_a_leer->tamanio_ocupado; // Me paro al final del primer mensaje guardado

			int contador = 4; // Si está vacío el espacio siguiente, leerá ceros
			int cero;
			memcpy(&cero, memoria_principal + desplazamiento, sizeof(int));

			while(cero == 0){
				desplazamiento += sizeof(int);
				memcpy(&cero, memoria_principal + desplazamiento, sizeof(int));
				contador+=4;

				if(contador >= tamanio_a_agregar){

					posicion_inicial_nuevo_mensaje = mensaje_a_leer->byte_comienzo_ocupado + mensaje_a_leer->tamanio_ocupado;
					*encontrado = 1;

					break;
				}
			}
			if(*encontrado == 1)
				break;
		}
		//sem_post(&MUTEX_MEM_PRIN);
	}

	return posicion_inicial_nuevo_mensaje;
}

t_mensaje_guardado* agregar_segun_best_fit(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){

	t_mensaje_guardado* mensaje_nuevo;

	int encontrado = 0;

		// Primero se busca posicion libre por Best Fit
	int posicion_inicial_nuevo_mensaje = buscar_best_fit(&encontrado, bloque_a_agregar_en_memoria, tamanio_a_agregar);

	if(encontrado == 1){
		mensaje_nuevo = guardar_en_posicion(bloque_a_agregar_en_memoria, tamanio_a_agregar, posicion_inicial_nuevo_mensaje);
	}

		// Si no se encuentra, y es momento de compactar, se compacta
	//sem_wait(&MUTEX_FALLOS);
	if(encontrado == 0){

		int frecuencia_compactacion = obtener_frecuencia_compactacion();
		int se_compacto = 0;

		if(frecuencia_compactacion == -1 || frecuencia_compactacion == 0 || frecuencia_compactacion == 1){ //Compacto cada vez que se libera
			compactar_memoria();
			se_compacto = 1;
		}

		if(frecuencia_compactacion >= 2 && (contador_fallos%frecuencia_compactacion) == 0 && contador_fallos >= frecuencia_compactacion){ //Si la frecuencia es mayor a 1 y los fallos son multiplo de la frecuencia
			compactar_memoria();
			se_compacto = 1;
		}

		if(se_compacto == 1){
			posicion_inicial_nuevo_mensaje = buscar_best_fit(&encontrado, bloque_a_agregar_en_memoria, tamanio_a_agregar);
			if(encontrado == 1){
				mensaje_nuevo = guardar_en_posicion(bloque_a_agregar_en_memoria, tamanio_a_agregar, posicion_inicial_nuevo_mensaje);
			}
		}
	}
	//sem_post(&MUTEX_FALLOS);

		// Si luego de compactar tampoco entra, se elimina y compacta hasta que se encuentre
	if(encontrado == 0){
		mensaje_nuevo = eliminar_y_compactar_hasta_encontrar(bloque_a_agregar_en_memoria, tamanio_a_agregar);
	}

	return mensaje_nuevo;
}

int buscar_best_fit(int *encontrado, void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){
	int tamanio_lista = list_size(elementos_en_memoria);
	int tamanio_aceptable = tamanio_a_agregar; // Empieza buscando un tamaño igual al necesario, si no lo encuentra, busca uno mas grande por 1 byte, y asi
	int posicion_inicial_nuevo_mensaje = -1;

	if(list_is_empty(elementos_en_memoria) || primera_posicion_vacia_y_entra(tamanio_a_agregar)){ // Si está vacía agregar al principio de la memoria

		posicion_inicial_nuevo_mensaje = 0;
		*encontrado = 1;

	} else {
		//sem_wait(&MUTEX_MEM_PRIN);
		while(*encontrado == 0 && tamanio_aceptable <= tamanio_de_memoria){

			for(int i=0; i<tamanio_lista; i++){ // Recorre para ver todos los mensajes guardados en la memoria principal

				t_mensaje_guardado* mensaje_a_leer;
				mensaje_a_leer = list_get(elementos_en_memoria, i);

				int desplazamiento = mensaje_a_leer->byte_comienzo_ocupado + mensaje_a_leer->tamanio_ocupado; // Me paro al final del primer mensaje guardado

				int contador = 4; // Si está vacío el espacio siguiente, leerá ceros
				int cero;
				memcpy(&cero, memoria_principal + desplazamiento, sizeof(int));

				while(cero == 0 && contador <= tamanio_a_agregar){
					desplazamiento += sizeof(int);
					memcpy(&cero, memoria_principal + desplazamiento, sizeof(int));
					contador+=4;
				}

				if(contador >= tamanio_aceptable){

					posicion_inicial_nuevo_mensaje = mensaje_a_leer->byte_comienzo_ocupado + mensaje_a_leer->tamanio_ocupado;
					*encontrado = 1;
					break;
				}

				//free(mensaje_a_leer);
			}
		tamanio_aceptable++;

		}
		//sem_post(&MUTEX_MEM_PRIN);
	}

	return posicion_inicial_nuevo_mensaje;
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

	sem_wait(&MUTEX_MEM_PRIN);
	for(int i=0; i<tamanio_de_memoria; i+=sizeof(uint32_t)){
		uint32_t valor;
		memcpy(&valor, memoria_principal + desplazamiento, sizeof(uint32_t));

		char* mensaje = string_from_format("Valor en posicion %d: %d.", i, valor);
		completar_logger(mensaje, "Broker", LOG_LEVEL_INFO); // LOG OBLIGATORIO

		//printf("Valor en posicio %d: %d", i, valor);
		desplazamiento += sizeof(uint32_t);
	}
	sem_post(&MUTEX_MEM_PRIN);

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

	sem_wait(&MUTEX_MEM_PRIN);
	for(int desplazamiento=0; desplazamiento<tamanio_lista; desplazamiento++){
		memcpy(&a_leer, memoria_principal + desplazamiento, sizeof(int));
		if(a_leer != 0){
			contador_ocupado++;
		}
	}
	sem_post(&MUTEX_MEM_PRIN);

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

	sem_wait(&MUTEX_MEM_PRIN);
	memcpy(&cero, memoria_principal + posicion_libre, sizeof(int));

	while(cero == 0 && contador <= tamanio_a_agregar){
		desplazamiento += sizeof(int);
		memcpy(&cero, memoria_principal + posicion_libre + desplazamiento, sizeof(int));
		contador+=4;
	}

	if(contador >= tamanio_a_agregar){
		boolean = 1;
	}
	sem_post(&MUTEX_MEM_PRIN);

	return boolean;
}


t_mensaje_guardado* guardar_en_posicion(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar, int posicion){
	t_mensaje_guardado* mensaje_nuevo = malloc(sizeof(t_mensaje_guardado));

	memcpy(memoria_principal + posicion, bloque_a_agregar_en_memoria, tamanio_a_agregar); //usar semaforos xq es variable global
	mensaje_nuevo->byte_comienzo_ocupado = posicion;
	mensaje_nuevo->tamanio_ocupado = tamanio_a_agregar;

	//sem_post(&MUTEX_MEM_PRIN);

	return mensaje_nuevo;
}

void* tratar_fragmentacion_interna(void* bloque_a_agregar_en_memoria, uint32_t tamanio_a_agregar){

	int tamanio_minimo_particion = obtener_tamanio_minimo_particion();
	int desplazamiento = 0;
	char relleno = 'f';

	void* bloque_con_fragmentacion = malloc(tamanio_minimo_particion);
	memcpy(bloque_con_fragmentacion + desplazamiento, bloque_a_agregar_en_memoria, tamanio_a_agregar);
	desplazamiento += tamanio_a_agregar;

	while(desplazamiento < tamanio_minimo_particion){
		memcpy(bloque_con_fragmentacion + desplazamiento, &relleno, sizeof(char));
		desplazamiento++;
	}

	return bloque_con_fragmentacion;
}

int encontrar_mensaje_a_eliminar_por_posicion(int posicion){
	t_mensaje_guardado* mensaje_aux;
	for(int i = 0; i<list_size(elementos_en_memoria); i++){
		mensaje_aux = list_get(elementos_en_memoria, i);
		if(mensaje_aux->byte_comienzo_ocupado == posicion){
			return i;
		}
	}
}

