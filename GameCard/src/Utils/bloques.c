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

void actualizar_valores_pokemon(char* path_metadata_pokemon,int posX,int posY,int cantidad){

	t_config* metadata = leer_metadata_pokemon(path_metadata_pokemon);

	char** bloques_pokemon = obtener_bloques_pokemon(metadata);
	int cantidad_bloques = obtener_cantidad_bloques_pokemon(metadata);

	for(int i =0; i<cantidad_bloques ; i++){
		//actualizar_valores_bloque(bloques_pokemon[i],)
		}
}


 int diferente_largo(char* numero1,char * numero2){
 	int largo1 = strlen(numero1);
 	int largo2 = strlen(numero2);

 	if(largo1 != largo2){

 		return 1;
 	}
 	return 0;
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

	 char* path = obtener_path_bloque_individual(bloque);

	  	struct stat st;
	  	stat(path,&st);

	  	int tamanio_actual = st.st_size;

	 /*
	 int tamanio_libre = tamanio_libre_bloque(bloque_en_string);
	 if(tamanio_libre == obtener_tamanio_bloques()){
	 */
	  	if(tamanio_actual==0){
		 free(bloque_en_string);
		 return 1;
	 }
	 else{
	free(bloque_en_string);
	 return 0;

	 }
 }


 int bloque_esta_lleno(char* bloque){
	 if(tamanio_libre_bloque(bloque)==0){
		 return 1;
	 }
	 return 0;
}


t_list* obtener_datos_bloques(char* path_pokemon ){

	FILE *file;
		int tamanio_archivo;
		t_list *lista_datos = list_create();
		char** bloques = obtener_bloques_pokemon(path_pokemon);


		char * datos = string_new();
		char *path_bloque_individual; // url de cada block particular
		char *path_bloques = obtener_path_bloques(); //url absoluta de donde estan los bloques
		char *aux;
		struct stat st;
		for(int i = 0; i<tamanio_array(bloques); i++)
		{
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
		free(datos); free(path_bloques);

		free(bloques);
		return lista_datos;

}

char* obtener_datos_en_string(t_list* lista_datos){ //para insertar el chorro de string a los bloques con almacenar_datos
	int cantidad_nodos = list_size(lista_datos);
	char* string_stream = string_new();
	char* aux = string_new();

	for(int i=0; i<cantidad_nodos ; i++){

		aux =list_get(lista_datos,i);
		string_append(&string_stream, aux);
		// NO SE ESTA LIBERANDO EL AUX PORQUE ME ROMPE A LA MIERDA
	}
	//free(aux);
	return string_stream;
}

void insertar_datos_a_lista(char *datos, t_list *lista_datos)
{
	if(!strcmp(datos,"")) return; //si viene vacio no agrego nada

	char **array_de_datos = string_split(datos,"\n");
	char *aux;
	for(int i =0; i<tamanio_array(array_de_datos); i++)
	{
		aux = string_duplicate(array_de_datos[i]);
		string_append(&aux, "\n");
		list_add(lista_datos,aux);
		free(array_de_datos[i]);
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



void almacenar_datos(char *data, char* path_pokemon){
	mostrar_paths_generados("adentro funcion antes de leer");
	 leer_config(); //prueba
	 mostrar_paths_generados("adentro funcion despues de leer");
	 char* path_bloques = obtener_path_bloques();
	 char** bloques = obtener_bloques_pokemon(path_pokemon);
	 char* bloques_string = obtener_bloques_pokemon_string(path_pokemon);
	 int tamanio_bloques = obtener_tamanio_bloques();


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
		}
		while(tamanio_array(bloques) > bloques_necesitados){// SI LA CANT DE BLOQUES QUE TENGO ES MAYOR A LA QUE NECESITO, LIBERO BLOQUES
			bloques_string = liberar_ultimo_bloque(bloques_string);
			bloques = string_get_string_as_array(bloques_string);
		}

		 //hacer un if() utilizando un flag de bloques agregados/sustraidos
	modificar_campo_bloques_metadata(path_pokemon,bloques_string);// MODIFICO EL ARCHIVO METADATA AL FINAL PARA NO HACER ESCRITURAS EN DISCO
																  // INNECESARIAS

	int ultima_pos_insertada = 0;

	for(int i =1; i <= tamanio_array(bloques);i++){ // ESCRIBE BLOQUE A BLOQUE, SABIENDO QUE YA TENGO LOS BLOQUES NECESARIOS

		char* path_bloque = obtener_path_bloque_individual(i);
		char* a_insertar = string_substring(data, ultima_pos_insertada, tamanio_bloques);
		ultima_pos_insertada += tamanio_bloques;
		guardar_data_en_bloque(a_insertar, path_bloque);

		free(path_bloque);
	}
}




int tamanio_array(char **array){
	int i=0;
	if(!array[i]) return 0;
	while(array[i] != NULL){
		i++;
	}
	return i;
}

char* obtener_path_bloque_individual(int numero_bloque){

	char* path_bloque = string_new();
	string_append(&path_bloque, obtener_path_bloques());
	string_append(&path_bloque, "/");
	string_append(&path_bloque, string_itoa(numero_bloque));
	string_append(&path_bloque, ".bin");
	return path_bloque;
}


char* asignar_bloque(char* bloques){

 	int nuevo_bloque = obtener_nuevo_bloque(); // IMPLEMENTAR EN BITMAP NASHE
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

 	//free(vector_bloques);
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
	 		list_add(lista_bloques,string_duplicate(array_bloques[j]));
	 		free(array_bloques[j]);
	 }
	 free(array_bloques);

	 list_remove(lista_bloques, cantidad_bloques - 1);

	 char *vector_bloques = string_new();
	  	string_append(&vector_bloques, "[");
	  	for(int k = 0; k < list_size(lista_bloques); k++){
	  		string_append(&vector_bloques,list_get(lista_bloques,k));
	  		if((k + 1) != list_size(lista_bloques)) string_append(&vector_bloques,",");
	  	}
	  	string_append(&vector_bloques, "]");

	  	list_destroy_and_destroy_elements(lista_bloques,free);

	  	//free(vector_bloques);
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
	char* digito_actual_X;
	char* digito_actual_Y;
	int tamanio_lista = list_size(lista);
	int k = 0;
	int i =0;
	char* linea;

	      while(k < tamanio_lista){ //BUSCO EL PRIMER DIGITO

	    	  linea = list_get(lista, k);
	    	  char* posicion_final_X = string_new();
	    	  char* posicion_final_Y = string_new();

	    	  while(linea[i] != '-'){
	    		  digito_actual_X =  string_from_format("%c",linea[i]);
	    		  string_append(&posicion_final_X,digito_actual_X);
	    		  i++;

	    		  }
	    	  if(string_equals_ignore_case(posicion_final_X,posX_string)){
	    		i++;

	       while(linea[i] != '='){// BUSCO SEGUNDO DIGITO
	    		  digito_actual_Y =  string_from_format("%c",linea[i]);
	    		  string_append(&posicion_final_Y,digito_actual_Y);
	    		  i++;
	    	  }
	    	  if(string_equals_ignore_case(posicion_final_Y,posY_string)){
	    		  //ENCUENTRO LA POSICION DESEADA
	    		  printf("encontre en el indice %d", k);

	    		 return k; //devuelvo el indice en la lista
	    	  	  }
	    	  }
	    	  	  k++;
	    	  	  i=0;

	  	  	  }

	      return -1; //no lo encontro
}
