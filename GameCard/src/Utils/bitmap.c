#include "pthread.h"
#include "bitmap.h"


void crear_bitmap(char* path_bitmap){
	pthread_mutex_lock(&MUTEX_BITMAP);
	int bloques = obtener_cantidad_bloques();

	if(bloques==0){
		//log_error(logger,"El sistema no puede inicializar con 0 bloques");
		pthread_mutex_unlock(&MUTEX_BITMAP);
		return;
	}


	bitmap_fd = open(path_bitmap,O_CREAT |O_RDWR, S_IRUSR | S_IWUSR);
	ftruncate(bitmap_fd, bloques/8 + 1);

	if(bitmap_fd == -1){
		//	log_error(logger,"No se pudo abrir el archivo bitmap.bin");
			close(bitmap_fd);

			free(path_bitmap);

			pthread_mutex_unlock(&MUTEX_BITMAP);
			return;
		}

	char* contenido_bitmap = mmap(NULL, bloques/8 + 1 , PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FILE, bitmap_fd, 0);
	bitarray = bitarray_create(contenido_bitmap, bloques/8 + 1 );

	int contador_bloques_ocupados=0;

	free(path_bitmap);

	for(int i = 0; i < bloques; i++){
		if(bloque_esta_vacio(i+1)==1){
			bitarray_clean_bit(bitarray,i);
		}
		else{
			bitarray_set_bit(bitarray,i);
			contador_bloques_ocupados++;
		}
	}
	if(contador_bloques_ocupados == bloques){
		flag_bloques_libres = 0;// si hay ocupados la misma cantidad que bloques totales, no hay bloques libres
	}
	else{
		flag_bloques_libres = 1;
	}

	msync(bitarray->bitarray,bitmap_fd,MS_SYNC);
	//close(bitmap_fd); Chequear si hace falta cerrar el archivo

}

void eliminar_bitmap(){
	//msync(bitarray->bitarray, bitarrayfd, MS_SYNC);
		bitarray_destroy(bitarray);
	}




void bitmap_liberar_bloque(int bloque){

	//	pthread_mutex_lock(&MUTEX_BITMAP);

		bitarray_clean_bit(bitarray,bloque);

		flag_bloques_libres = 1;

	//	pthread_mutex_unlock(&MUTEX_BITMAP);
}


int obtener_nuevo_bloque(){

		if(flag_bloques_libres ==0){ //si no hay bloques libres ni busca
			return -1;
		}

		//pthread_mutex_lock(&MUTEX_BITMAP); QUEDA TRABADO EN EL SEMAFORO AYUDA

		int cantidad_bloques = obtener_cantidad_bloques();
		int i;

		for(i = 0; i < cantidad_bloques; i++){
			if(!bitarray_test_bit(bitarray,i)){
				bitarray_set_bit(bitarray,i);

				msync(bitarray->bitarray, bitmap_fd, MS_SYNC);
				pthread_mutex_unlock(&MUTEX_BITMAP);
				return i+1;
			}
		}


		flag_bloques_libres = 0; // 0 si no hay libres, 1 si los hay
		//pthread_mutex_unlock(&MUTEX_BITMAP); 	QUEDA TRABADO EN EL SEMAFORO AYUDA
		return -1; // salio del for, por lo que no hay bloque libre/
}

void mostrar_contenido_bitmap(){
	int tamanio =bitarray_get_max_bit(bitarray);

	for(int i=0 ; i< tamanio ; i++){
		if(bitarray_test_bit(bitarray, i))
			puts("1");
		else
			puts("0");
	}
}

