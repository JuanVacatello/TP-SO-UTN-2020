#include "dump.h"

void actualizar_dump_cache(){

	FILE *dump;
	char* path = "/home/utnso/Documentos/Delibird/Broker/tp-2020-1c-wRAPPERS/Broker/Broker.dump";
	dump = fopen(path, "w");
	fclose(dump);

	dump = txt_open_for_append(path);
	llenar_el_dump(dump);
	txt_close_file(dump);
}

void llenar_el_dump(FILE* dump){

	llenar_inicio_dump(dump);

	int contador_de_particiones = 0;
	int memoria_leida = 0;

	t_list* lista_ordenada = list_duplicate(elementos_en_memoria);
	list_sort(lista_ordenada, comparar_inicios_mensajes);

	t_mensaje_guardado* mensaje_a_leer;
	t_mensaje_guardado* siguiente_mensaje;

	memoria_leida = si_el_anterior_esta_vacio(lista_ordenada, dump, 0, contador_de_particiones);
	if(memoria_leida != 0){
		contador_de_particiones++;
	}

	char* linea_a_agregar = string_new();
	int inicio, tamanio, final, lru, id;
	op_code cola;

	for(int i=0; i<list_size(lista_ordenada); i++){

		mensaje_a_leer = list_get(elementos_en_memoria, i);

		string_append(&linea_a_agregar, "Particion ");
		string_append_with_format(&linea_a_agregar, "%d: ", contador_de_particiones);

		inicio = mensaje_a_leer->byte_comienzo_ocupado;
		tamanio = mensaje_a_leer->tamanio_ocupado;
		final = inicio + tamanio - 1;
		lru = mensaje_a_leer->ultima_referencia;
		cola = mensaje_a_leer->cola;
		id = mensaje_a_leer->cola;

		linea_a_agregar = crear_linea_a_agregar_ocupada(inicio, final, tamanio, lru, cola, id);
		txt_write_in_file(dump, linea_a_agregar);
		contador_de_particiones++;

		siguiente_mensaje = list_get(elementos_en_memoria, (i+1));
		int cantidad_leida = si_el_anterior_esta_vacio(lista_ordenada, dump, (i+1), contador_de_particiones);
		if(cantidad_leida != 0){
			contador_de_particiones++;
			memoria_leida += cantidad_leida;
		}
	}

	if(memoria_leida < tamanio_de_memoria){
		int tamanio_restante = tamanio_de_memoria - final - 1;
		crear_linea_a_agregar_vacia((final+1), (tamanio_de_memoria-1), tamanio_restante);
	}

	char* guiones = string_repeat('-', 30);
	txt_write_in_file(dump, guiones);

}

void llenar_inicio_dump(FILE* dump){

	char* guiones = string_repeat('-', 30);
	char* linea_inicial = string_new();
	string_append_with_format(&linea_inicial, "%s \n", guiones);
	txt_write_in_file(dump, guiones);

	char* hora_actual = temporal_get_string_time();
	string_append(&linea_inicial, "Dump: ");
	string_append_with_format(&linea_inicial, "%s \n", hora_actual); // FALTA LA FECHA
	txt_write_in_file(dump, linea_inicial);
}

int si_el_anterior_esta_vacio(t_list* lista_ordenada, FILE* dump, int index, int contador_de_particiones){
	t_mensaje_guardado* mensaje_a_leer = list_get(lista_ordenada, index);
	char* linea_a_agregar = string_new();
	int tamanio_vacio;
	int final_vacio;
	int inicio_vacio;

	if(index == 0){
		if(mensaje_a_leer->byte_comienzo_ocupado != 0){

			string_append(&linea_a_agregar, "Particion ");
			string_append_with_format(&linea_a_agregar, "%d: ", contador_de_particiones);

			tamanio_vacio = mensaje_a_leer->byte_comienzo_ocupado;
			final_vacio = tamanio_vacio - 1;
			inicio_vacio = 0;

			linea_a_agregar = crear_linea_a_agregar_vacia(inicio_vacio, final_vacio, tamanio_vacio);
			txt_write_in_file(dump, linea_a_agregar);

			return tamanio_vacio;
		}
	} else {
		t_mensaje_guardado* mensaje_anterior = list_get(lista_ordenada, (index-1));
		int final_anterior = (mensaje_anterior->byte_comienzo_ocupado + mensaje_anterior->byte_comienzo_ocupado);

		if((mensaje_a_leer->byte_comienzo_ocupado - 1) != final_anterior){

			string_append(&linea_a_agregar, "Particion ");
			string_append_with_format(&linea_a_agregar, "%d: ", contador_de_particiones);

			tamanio_vacio = final_anterior - mensaje_a_leer->byte_comienzo_ocupado;
			final_vacio = mensaje_a_leer->byte_comienzo_ocupado - 1;
			inicio_vacio = mensaje_a_leer->byte_comienzo_ocupado - tamanio_vacio;

			linea_a_agregar = crear_linea_a_agregar_vacia(inicio_vacio, final_vacio, tamanio_vacio);
			txt_write_in_file(dump, linea_a_agregar);

			return tamanio_vacio;
		}
	}

	return 0;
}

char* crear_linea_a_agregar_ocupada(int inicio, int final, int tamanio, int lru, int cola, int id){

	char* linea_a_agregar = string_new();

	int direccion_de_inicio = &inicio;
	int direccion_de_final = &final;

	string_append_with_format(&linea_a_agregar, "%p - ", direccion_de_inicio);
	string_append_with_format(&linea_a_agregar, "%p.     ", direccion_de_final);
	string_append(&linea_a_agregar, "[X]     ");
	string_append_with_format(&linea_a_agregar, "Size: %db     ", tamanio);
	string_append_with_format(&linea_a_agregar, "LRU: %d   ", lru);
	string_append_with_format(&linea_a_agregar, "COLA: %d   ", cola);
	string_append_with_format(&linea_a_agregar, "COLA: %d \n", id);

	return linea_a_agregar;
}

char* crear_linea_a_agregar_vacia(int inicio, int final, int tamanio){

	char* linea_a_agregar = string_new();

	int direccion_de_inicio = &inicio;
	int direccion_de_final = &final;

	string_append_with_format(&linea_a_agregar, "%p - ", direccion_de_inicio);
	string_append_with_format(&linea_a_agregar, "%p.     ", direccion_de_final);
	string_append(&linea_a_agregar, "[L]     ");
	string_append_with_format(&linea_a_agregar, "Size: %db     ", tamanio);

	return linea_a_agregar;
}



