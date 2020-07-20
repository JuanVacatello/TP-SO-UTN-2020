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

 char* obtener_path_bloque(char* bloque){
 	char* path = string_new();
 	string_append(&path, "/home/utnso/Documentos/Prueba/");
 	string_append(&path, bloque);
 	string_append(&path, ".bin");
 	return path;
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
 	char* path = obtener_path_bloque(bloque);

 	struct stat st;
 	stat(path,&st);

 	int tamanio_actual = st.st_size;

 	free(path);

 	return(obtener_tamanio_bloques() - tamanio_actual);
 }

 int bloque_esta_vacio(int bloque){
	 char* bloque_en_string = string_itoa(bloque);
	 if(tamanio_libre_bloque(bloque_en_string) == obtener_tamanio_bloques())
	 return 1;
	 else
	 return 0;
	 }



 int bloque_esta_lleno(char* bloque){
	 if(tamanio_libre_bloque(bloque)==0){
		 return 1;
	 }
	 return 0;
}


void obtener_datos_bloques(t_list *lista,char* path_pokemon ){

	FILE *file;
		int tamanio_archivo;
		t_list *lista_datos = list_create();
		char *vector_bloques_string = obtener_bloques_pokemon(path_pokemon);
		char** bloques = string_get_string_as_array(vector_bloques_string);
		free(vector_bloques_string);
		int tamanio_array = tamanio_array_en_string(vector_bloques_string);

		char * datos = string_new();
		char *path_bloque_individual; // url de cada block particular
		char *path_bloques = obtener_path_bloques(); //url absoluta de donde estan los bloques "mnt/blocks"
		char *aux;
		struct stat st;
		for(int i = 0; i<tamanio_array; i++)
		{
			path_bloque_individual = string_new();
			string_append(&path_bloque_individual,path_bloques);
			string_append(&path_bloque_individual,bloques[i]);
			string_append(&path_bloque_individual,".bin");


			stat(path_bloque_individual,&st);
			tamanio_archivo = st.st_size;

			aux = malloc(tamanio_archivo+1);

			file = fopen(path_bloque_individual,"r");
			fread(aux,tamanio_archivo,1,file);
			fclose(file);
			aux[tamanio_archivo] = '\0';

			if(strcmp(aux,"&")) //si no es igual a "&" lo agrego a la lista de inserts
				string_append(&datos,aux);

			free(bloques[i]);
			free(aux);
			free(path_bloque_individual);
		}

		insertar_datos_a_lista(datos,lista_datos); //parsea el char *inserts por \n y los mete en la lista
		free(datos); free(path_bloques);

		free(bloques);

}

void insertar_datos_a_lista(char *datos, t_list *lista_datos)
{
	if(!strcmp(datos,"")) return; //si viene vacio no agrego nada

	char **array_de_datos = string_split(datos,"\n");
	char *aux;
	for(int i =0; i<2; i++) //<Size of array
	{
		aux = string_duplicate(array_de_datos[i]);
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














