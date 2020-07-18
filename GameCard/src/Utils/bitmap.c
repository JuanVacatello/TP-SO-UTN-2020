#include "pthread.h"
#include "bitmap.h"


void crear_bitmap(){
	pthread_mutex_lock(&MUTEX_BITMAP);
	int bloques = obtener_cantidad_bloques();

	if(bloques==0){
		//log_error(logger,"El sistema no puede inicializar con 0 bloques");
		pthread_mutex_unlock(&MUTEX_BITMAP);
		return;
	}

	char* path_bitmap = obtener_path_metadata();
	string_append(&path_bitmap, "/bitmap.bin");

	int bitmap_fd = open(path_bitmap,O_CREAT |O_RDWR, S_IRUSR | S_IWUSR);
	ftruncate(bitmap_fd, bloques/8 + 1);

	if(bitmap_fd == -1){
		//	log_error(logger,"No se pudo abrir el archivo bitmap.bin");
			close(bitmap_fd);

			free(path_bitmap); //Chequear

			pthread_mutex_unlock(&MUTEX_BITMAP);
			return;
		}

	char* contenido_bitmap = mmap(NULL, bloques/8 + 1 , PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FILE, bitmap_fd, 0);
	bitarray = bitarray_create(contenido_bitmap, bloques/8 + 1 );


	free(path_bitmap); //Chequear

	for(int i = 0; i < bloques; i++){
		if(bloque_esta_vacio(i)==1){
			bitarray_clean_bit(bitarray,i);
		}
			bitarray_set_bit(bitarray,i);
	}

	msync(bitarray->bitarray,bitmap_fd,MS_SYNC);
	close(bitmap_fd);

}

void eliminar_bitmap(){
	//msync(bitarray->bitarray, bitarrayfd, MS_SYNC);
		bitarray_destroy(bitarray);
	}




void bitmap_liberar_bloque(int bloque, int bitmap);

int obtener_nuevo_bloque(){

}

