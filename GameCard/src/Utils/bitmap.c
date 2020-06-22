#include "pthread.h"
#include "bitmap.h"


void crear_bitmap(){
	pthread_mutex_lock(&MUTEX_BITMAP);
	int bloques = obtener_cantidad_bloques();

	if(bloques==0){
		log_error(logger,"El sistema no puede inicializar con 0 bloques");
		pthread_mutex_unlock(&MUTEX_BITMAP);
		return;
	}

	char* path_bitmap = obtener_path_metadata();
	string_append(&path_bitmap, "/bitmap.bin");

	int bitmap_fd = open(path_bitmap,O_CREAT |O_RDWR, S_IRUSR | S_IWUSR);
	ftruncate(bitmap_fd, bloques/8 + 1);

	if(bitmap_fd == -1){
			log_error(logger,"No se pudo abrir el archivo bitmap.h");
			close(bitmap_fd);

			free(path_bitmap); //Chequear

			pthread_mutex_unlock(&MUTEX_BITMAP);
			return;
		}

	char* contenido_bitmap = mmap(NULL, bloques/8 + 1 , PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FILE, bitmap_fd, 0);
	t_bitarray* bitmap = bitarray_create(contenido_bitmap, bloques/8 + 1 );


	free(path_bitmap); //Chequear

	for(int i = 0; i < bloques; i++){
		if(bloque_esta_vacio(i)==1){
			bitarray_clean_bit(bitmap,i);
		}
			bitarray_set_bit(bitmap,i);
	}

	msync(bitmap->bitarray,bitmap_fd,MS_SYNC);
	close(bitmap_fd);

}
