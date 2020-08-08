#include "bloques.h"
#include "metadata.h"

int existe_bloque(char* path_bloques,char* numero_de_bloque){
	char* path_bloque = string_new();
	string_append(&path_bloque, path_bloques);
	string_append(&path_bloque,"/" );
	string_append(&path_bloque,numero_de_bloque);
	string_append(&path_bloque, ".bin");
	struct stat buffer;
	return stat(path_bloque,&buffer);
}

int existe_file(char* path){
	struct stat buffer;
	return stat(path,&buffer);
}

int tamanio_libre_bloque(char* bloque){

	int numero_bloque = atoi(bloque);
 	char* path = obtener_path_bloque_individual(numero_bloque);

 	struct stat st;
 	stat(path,&st);

 	int tamanio_actual = st.st_size;

 	free(path);

 	return(obtener_tamanio_bloques() - tamanio_actual);
 }

 int bloque_esta_vacio(int bloque){
	 char* bloque_en_string = string_itoa(bloque);

	 char* path = obtener_path_bloque_individual(bloque_en_string);

	 struct stat st;
	 stat(path,&st);

	 int tamanio_actual = st.st_size;

	 /*
	 int tamanio_libre = tamanio_libre_bloque(bloque_en_string);
	 if(tamanio_libre == obtener_tamanio_bloques()){
	 */

	 if(tamanio_actual==0){
		 free(bloque_en_string);
		 free(path);
		 return 1;
	 }
	 else{
		 free(bloque_en_string);
		 free(path);
		 return 0;
	 }
 }


 int bloque_esta_lleno(char* bloque){
	 if(tamanio_libre_bloque(bloque)==0){
		 return 1;
	 }
	 return 0;
}

t_list* obtener_datos_bloques(t_config* metadata_pokemon){

	FILE *file;
	int tamanio_archivo;
	t_list* lista_datos = list_create();
	char** bloques = obtener_bloques_pokemon(metadata_pokemon);

	char * datos = string_new();
	char *path_bloque_individual; // url de cada block particular
	char *path_bloques = obtener_path_bloques(); //url absoluta de donde estan los bloques
	char *aux;
	struct stat st;

	int cantidad_bloques = tamanio_array(bloques);

	for(int i = 0; i<cantidad_bloques; i++){

		path_bloque_individual = string_new();
		string_append(&path_bloque_individual,path_bloques);
		string_append(&path_bloque_individual,"/");
		string_append(&path_bloque_individual,bloques[i]);
		string_append(&path_bloque_individual,".bin");

		stat(path_bloque_individual,&st);
		tamanio_archivo = st.st_size;

		aux = malloc(tamanio_archivo+1);
		file = fopen(path_bloque_individual,"r");
		fread(aux,tamanio_archivo,1,file);
		fclose(file);
		aux[tamanio_archivo] = '\0';

		//if(strcmp(aux,"&")) //si no es igual a "&" lo agrego a la lista de inserts
		string_append(&datos,aux);
		free(bloques[i]);
		free(aux);
		free(path_bloque_individual);
	}

	insertar_datos_a_lista(datos,lista_datos); //parsea el char *inserts por \n y los mete en la lista

	free(datos); // PUEDE ROMPER
	free(bloques);

	return lista_datos;
}

char* obtener_datos_en_string(t_list* lista_datos){ //para insertar el chorro de string a los bloques con almacenar_datos()
	int cantidad_nodos = list_size(lista_datos);
	char* string_stream = string_new();

	for(int i=0; i<cantidad_nodos ; i++){

		char* aux = string_new();
		aux = list_get(lista_datos, i);
		string_append(&string_stream, aux);
		//free(aux);
		// NO SE ESTA LIBERANDO EL AUX PORQUE ME ROMPE A LA MIERDA
	}

	return string_stream;
}

void insertar_datos_a_lista(char *datos, t_list *lista_datos){

	if(!strcmp(datos,"")) return; //si viene vacio no agrego nada

	char **array_de_datos = string_split(datos,"\n");

	for(int i =0; i<tamanio_array(array_de_datos); i++){

		char *aux;
		aux = string_duplicate(array_de_datos[i]);
		string_append(&aux, "\n");
		list_add(lista_datos,aux);
		free(array_de_datos[i]);
		//free(aux); //creo que rompe porque lo liberarias de la lista
	}

	free(array_de_datos);
}


void guardar_data_en_bloque(char* data, char* path_bloque){

	char *aux = malloc(3);
	struct stat st;
	FILE *file;
	stat(path_bloque,&st);
	int tamanio_bloque = st.st_size;

	file = fopen(path_bloque,"w");
	fclose(file);

	if(tamanio_bloque == 1){ // NO SE SI ES UTIL
		file = fopen(path_bloque,"r");
		fread(aux, tamanio_bloque ,1,file);
		fclose(file);
		aux[tamanio_bloque] = '\0';

		if(!strcmp(aux,"&")){
			file = fopen(path_bloque,"w");
			fclose(file);
		}
	}

	FILE *file2;
	file2 = txt_open_for_append(path_bloque);
	txt_write_in_file(file2, data);
	txt_close_file(file2);

	free(aux);
}

void almacenar_datos(char *data,t_config* metadata_pokemon){

	 char* path_bloques = obtener_path_bloques();
	 char* bloques_string = obtener_bloques_pokemon_string(metadata_pokemon);
	 char** bloques = string_get_string_as_array(bloques_string);;

	 int tamanio_bloques = obtener_tamanio_bloques();
	 int flag_cantidad_bloques_modificada =0;

	 //ME FIJO CUANTOS BLOQUES VOY A NECESITAR PARA ALMACENAR TODA LA DATA QUE LEVANTE
	 int bloques_necesitados;

	 if((strlen(data) % tamanio_bloques)==0){ // SI DIVISION TIENE RESTO 0, ENTRA TODO JUSTO EN X CANTIDAD DE BLOQUES
		 bloques_necesitados = strlen(data)/tamanio_bloques;
	 }
	 else{											   // SI DIVISION TIENE RESTO != 0 , HAY QUE PEDIR UN BLOQUE EXTRA PARA LO QUE SOBRA
		 bloques_necesitados = strlen(data)/tamanio_bloques + 1 ;
	 }

	 while(tamanio_array(bloques) < bloques_necesitados){// SI LA CANT DE BLOQUES QUE TENGO ES MENOR A LO QUE NECESITO, TENGO QUE PEDIR MAS.
		 bloques_string = asignar_bloque(bloques_string);
		 bloques = string_get_string_as_array(bloques_string);
		 flag_cantidad_bloques_modificada = 1;
	 }

	 while(tamanio_array(bloques) > bloques_necesitados){// SI LA CANT DE BLOQUES QUE TENGO ES MAYOR A LA QUE NECESITO, LIBERO BLOQUES
		 bloques_string = liberar_ultimo_bloque(bloques_string);
		 bloques = string_get_string_as_array(bloques_string);
		 flag_cantidad_bloques_modificada = 1;
	 }

	 if(flag_cantidad_bloques_modificada == 1){
		 modificar_campo_bloques_metadata(metadata_pokemon, bloques_string);// MODIFICO EL ARCHIVO METADATA AL FINAL PARA NO HACER ESCRITURAS EN DISCO INNECESARIAS
	 }

	 int ultima_pos_insertada = 0;

	 for(int i =0; i < tamanio_array(bloques);i++){ // ESCRIBE BLOQUE A BLOQUE, SABIENDO QUE YA TENGO LOS BLOQUES NECESARIOS
		 char* path_bloque = obtener_path_bloque_individual(bloques[i]);
		 char* a_insertar = string_substring(data, ultima_pos_insertada, tamanio_bloques);
		 ultima_pos_insertada += tamanio_bloques;
		 guardar_data_en_bloque(a_insertar, path_bloque);

		 free(path_bloque);
		 free(a_insertar);
		 free(bloques[i]);
	 }

	free(bloques);
}

int tamanio_array(char **array){
	int i=0;
	if(!array[i]) return 0;
	while(array[i] != NULL){
		i++;
	}
	return i;
}

char* obtener_path_bloque_individual(char* bloque){

	char* path_bloque = string_new();
	string_append(&path_bloque, obtener_path_bloques());
	string_append(&path_bloque, "/");
	string_append(&path_bloque, bloque);
	string_append(&path_bloque, ".bin");
	return path_bloque;
}


char* asignar_bloque(char* bloques){

 	int nuevo_bloque = obtener_nuevo_bloque(); // le pide al bitmap
 	char** array_bloques = string_get_string_as_array(bloques);
 	t_list *lista_bloques = list_create();

 	for(int j = 0; j < tamanio_array_en_string(bloques); j++){
 		list_add(lista_bloques,string_duplicate(array_bloques[j]));
 		free(array_bloques[j]);
 	}

 	free(array_bloques);

 	char *x = string_itoa(nuevo_bloque);
 	list_add(lista_bloques,x);

 	char *vector_bloques = string_new();
 	string_append(&vector_bloques, "[");
 	for(int k = 0; k < list_size(lista_bloques); k++){
 		string_append(&vector_bloques,list_get(lista_bloques,k));
 		if((k + 1) != list_size(lista_bloques)) string_append(&vector_bloques,",");
 	}

 	string_append(&vector_bloques, "]");
 	list_destroy_and_destroy_elements(lista_bloques,free);

 	return vector_bloques;
 }

char* asignar_primer_bloque(){

	char* bloques = string_new();
	int nuevo_bloque = obtener_nuevo_bloque();
	char* bloque = string_itoa(nuevo_bloque);
	string_append(&bloques,"[");
	string_append(&bloques,bloque);
	string_append(&bloques,"]");

	return bloques;
}


char* liberar_ultimo_bloque(char* bloques){

	char** array_bloques = string_get_string_as_array(bloques);
	int cantidad_bloques = tamanio_array(array_bloques);
	t_list *lista_bloques = list_create();

	for(int j = 0; j < cantidad_bloques; j++){
		list_add(lista_bloques, string_duplicate(array_bloques[j]));
	 	//free(array_bloques[j]);
	}

	free(array_bloques);

	char* bloque = list_remove(lista_bloques, cantidad_bloques - 1);
	int numero_bloque = atoi(bloque);
	bitmap_liberar_bloque(numero_bloque); // libero el bloque en el bitmap

	limpiar_bloque(bloque); // vacio el contenido del bloque

	char *vector_bloques = string_new();
	string_append(&vector_bloques, "[");

	for(int k = 0; k < list_size(lista_bloques); k++){
		string_append(&vector_bloques,list_get(lista_bloques,k));

		if((k + 1) != list_size(lista_bloques)) string_append(&vector_bloques,",");
	 }

	string_append(&vector_bloques, "]");
	list_destroy_and_destroy_elements(lista_bloques,free);

	return vector_bloques;
}

void agregar_linea(t_list* lista, char* linea){ //agrega una entrada al final de la lista. Caso de uso que la pos no este en el FS
	list_add(lista, linea);
}

void mostrar_contenido_lista(t_list* datos){

	int tamanio_lista = list_size(datos);

	for(int i=0; i<tamanio_lista ; i++){
		printf("%s", list_get(datos, i));
	}
}

int existe_posicion_en_lista(t_list* lista, int posX, int posY){ //funciona

	char* posX_string = string_itoa(posX);
    char* posY_string = string_itoa(posY);
//	char* digito_actual_X;
//	char* digito_actual_Y;
	int tamanio_lista = list_size(lista);
	int k = 0;
	int i =0;
	char* linea;

	while(k < tamanio_lista){ //BUSCO EL PRIMER DIGITO

		linea = list_get(lista, k);
		char* posicion_final_X = string_new();
	    char* posicion_final_Y = string_new();

	    while(linea[i] != '-'){
	    	char* digito_actual_X = string_from_format("%c",linea[i]);
	    	string_append(&posicion_final_X, digito_actual_X);
	    	i++;
	    	free(digito_actual_X);
	    }

	    if(string_equals_ignore_case(posicion_final_X, posX_string)){
	    	i++;

	    	while(linea[i] != '='){// BUSCO SEGUNDO DIGITO
	    		char* digito_actual_Y =  string_from_format("%c", linea[i]);
	    		string_append(&posicion_final_Y, digito_actual_Y);
	    		i++;
	    		free(digito_actual_Y);
	    	}

	    	if(string_equals_ignore_case(posicion_final_Y, posY_string)){
	    		return k; //devuelvo el indice en la lista
	    	}
	    }

	    k++;
	    i=0;

	    free(posicion_final_Y);
	    free(posicion_final_X);
	}

	free(posX_string);
	free(posY_string);

	return -1; //no lo encontro
}

void liberar_bloques_pokemon(t_config* metadata_pokemon){

	char** bloques = obtener_bloques_pokemon(metadata_pokemon);

	for(int i = 0 ; i < tamanio_array(bloques); i++){

		int numero_bloque = atoi(bloques[i]);
		bitmap_liberar_bloque(numero_bloque);
	}
}

void limpiar_bloques_pokemon(t_config* metadata_pokemon){

	char* bloques_string= config_get_string_value(metadata_pokemon, "BLOCKS");
	char** bloques = string_get_string_as_array(bloques_string);

	for(int i = 0 ; i < tamanio_array(bloques); i++){

		limpiar_bloque(bloques[i]);
	}
}

void limpiar_bloque(char* bloque){
	char* path_bloques = obtener_path_bloques();

	char* aux = string_new();
	string_append(&aux, path_bloques);
	string_append(&aux, "/");
	string_append(&aux,bloque);
	string_append(&aux, ".bin");

	FILE *f = fopen(aux,"w");
	fclose(f);

	free(aux);
}
