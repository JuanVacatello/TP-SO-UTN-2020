/*
 ============================================================================
 Name        : GameCard.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include "GameCard.h"


int main(void) {
	leer_config();
	char* punto_montaje = obtener_punto_montaje();

	inicializar_file_system(punto_montaje);



	return 0;
}


void inicializar_file_system(char* punto_montaje){


	char* path_tall_grass = string_new();
	puts(path_tall_grass);
	string_append(&path_tall_grass, punto_montaje);
	string_append(&path_tall_grass, "/TALL_GRASS");
	mkdir(path_tall_grass, 0777);

	//CREA METADATA//FALT
	char* path_metadata = string_new();
	string_append(&path_metadata, path_tall_grass);
	string_append(&path_metadata, "/Metadata");
	mkdir(path_metadata, 0777);

	//CREA BLOQUES
	char* path_bloques = string_new();
	string_append(&path_bloques, path_tall_grass);
	string_append(&path_bloques, "/Bloques");
	mkdir(path_bloques, 0777);

	//CREA FILES
	char* path_files = string_new();
	string_append(&path_files, path_tall_grass);
	string_append(&path_files, "/Files");
	mkdir(path_files, 0777);

	//puts(path_metadata);
	//puts(path_bloques);
	//puts(path_files);


	inicializar_metadata(path_metadata, 4, 2);
	inicializar_files(path_files);
	inicializar_bloques(path_bloques);



	leer_metadata("/home/utnso/Documentos/Prueba_GameCard/TALL_GRASS/Files/Charmander/Metadata.bin");
	char* bloques = obtener_bloques_metada();
	puts(bloques);

	//ROMPE LEER METADATA. ARREGLAR.




	//obtener_bloque_pokemon("Charmander", path_files);

	//agregar_pokemon("Charmander",1,1,2, path_files); //Funciona

	//int a = existe_bloque(path_bloques,"1");
	//printf("%d",a);
	//obtener_bloque_pokemon( "Pikachu", path_files);
}

void inicializar_metadata(char* path_metadata, int block_size, int cant_bloques){

//inicializa metadata.bin//TERMINAR BITMAP, NO LEE BIEN
	char* path_archivo_metadata = string_new();
	string_append(&path_archivo_metadata, path_metadata);
	string_append(&path_archivo_metadata, "/Metadata.bin");//TERMINAR BITMAP, NO LEE BIEN

	FILE* metadata = txt_open_for_append(path_archivo_metadata);

	txt_write_in_file(metadata, "BLOCK_SIZE=64\n");
	txt_write_in_file(metadata, "BLOCKS=5192\n");
	txt_write_in_file(metadata, "MAGIC_NUMBER=TALL_GRASS\n");
	txt_close_file(metadata);



//inicializa bitmap.bin
	char* path_archivo_bitmap= string_new();
	string_append(&path_archivo_bitmap, path_metadata);
	string_append(&path_archivo_bitmap, "/Bitmap.bin");

	FILE* bitmap = txt_open_for_append(path_archivo_bitmap);


	void* array= malloc(1);
	t_bitarray* bit_array = bitarray_create_with_mode(array,1, MSB_FIRST);
	size_t tamanio = bitarray_get_max_bit(bitmap);

	//printf("%d\n",tamanio);

	bitarray_set_bit(bit_array, 4);
	//puts((bit_array->bitarray));

	txt_write_in_file(bitmap,(bit_array->bitarray));
	txt_close_file(bitmap);
		//TERMINAR BITMAP, NO LEE BIEN
}



void inicializar_files(char* path_files){

	completar_metadata_directorio(path_files);

}


void inicializar_bloques(char* path_bloques){

	completar_metadata_directorio(path_bloques);
}

void agregar_pokemon(char* pokemon,int posX,int posY,int cantidad, char* path_files){

	char* path_pokemon = string_new();
		string_append(&path_pokemon, path_files);
		string_append(&path_pokemon, "/");
		string_append(&path_pokemon, pokemon);


		if(existe_file(path_pokemon)==0){ //FALTA IMPLEMENTAR EXISTE POKEMON
			//actualizar_valores_pokemon(path_pokemon,posX,posY,cantidad); //FALTA IMPLEMENTAR, TENDRIA QUE FUNCIONAR TANTO PARA SUMAR COMO RESTAR
			puts("existe");
		}
		else{
			puts("no existe");
			mkdir(path_pokemon, 0777);
			string_append(&path_pokemon,"/");
			string_append(&path_pokemon,"Metadata.bin");


			FILE* metadata = txt_open_for_append(path_pokemon);

			txt_write_in_file(metadata, "DIRECTORY=N\n");
			txt_write_in_file(metadata, "SIZE=250\n");

			//char* bloque_asignado = obtener_siguiente_bloque(); //FALTA IMPLEMENTAR.OBTENER PROX BLOQUE FORMATO:[]
			//txt_write_in_file(metadata, bloque_asignado);
			txt_write_in_file(metadata, "\n");

			txt_write_in_file(metadata, "OPEN=N");
			txt_close_file(metadata);

			//actualizar_valores_pokemon(path_pokemon,posX,posY,cantidad);

		}
}

void obtener_bloque_pokemon(char* pokemon,char* path_files){
	char* path_metadata_pokemon = string_new();
	string_append(&path_metadata_pokemon,path_files);
	string_append(&path_metadata_pokemon,"/");
	string_append(&path_metadata_pokemon,pokemon);
	string_append(&path_metadata_pokemon,"/");
	string_append(&path_metadata_pokemon,"Metadata.bin");





	/*
	FILE *metadata=NULL;
	metadata = fopen(path_metadata_pokemon,"rb");

	if(metadata == NULL){
		printf("Error no se pudo abrir el file\n");
	}

	char* caracter;
	int i=0;

	while (i < 2){
		fread(caracter,1,1,metadata);
		printf("%c",*caracter);
		if(string_equals_ignore_case(caracter, "\n"))
			i++;
	}
	char palabra[7];
	fread(palabra,7,1,metadata);
	printf("%s \n" ,palabra);

	/*
	char* letra= string_new();
	char palabra[12];


	fseek(metadata, 28,SEEK_SET);
	fread(palabra,12,1,metadata);

	printf("%s \n" ,palabra);
	char** vector =string_get_string_as_array(palabra);
	puts(vector[0]);

	for(int i=1;i<5;i++){
		fread(letra,1,1,metadata);
		printf("%s \n" ,letra);
	}
	*/
}

void completar_metadata_directorio(char* path_directorio){

	char* path_archivo_metadata = string_new();
	string_append(&path_archivo_metadata, path_directorio);
	string_append(&path_archivo_metadata, "/Metadata.bin");

		FILE* metadata = txt_open_for_append(path_archivo_metadata);

		txt_write_in_file(metadata, "DIRECTORY=Y\n");
		txt_close_file(metadata);
}


int existe_file(char* path){
	struct stat buffer;
	return stat(path,&buffer);
}




















