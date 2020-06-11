/*
 * metadata.h
 *
 *  Created on: 11 jun. 2020
 *      Author: utnso
 */

#ifndef UTILS_METADATA_H_
#define UTILS_METADATA_H_

#include <commons/collections/dictionary.h>

	typedef struct {
		char *path;
		t_dictionary *properties;
	} t_metadata;

	t_metadata* metadata;

	void leer_metadata(char* path_metadata);

	char* obtener_bloques_metada();



	/**
	* @NAME: config_create
	* @DESC: Crea y devuelve un puntero a una estructura t_config
	* @PARAMS:
	* 		path - path del archivo de configuracion
	*/
	t_metadata *metadata_create(char *path);

	/**
	* @NAME: config_has_property
	* @DESC: Retorna true si key se encuentra en la configuracion.
	*/
	bool	 metadata_has_property(t_metadata*, char* key);

	/**
	* @NAME: config_get_string_value
	* @DESC: Retorna un string con el valor asociado a key.
	*/
	char 	 *metadata_get_string_value(t_metadata*, char *key);

	/**
	* @NAME: config_get_int_value
	* @DESC:Retorna un int con el valor asociado a key.
	*/
	int 	  metadata_get_int_value(t_metadata*, char *key);

	/**
	* @NAME: config_get_long_value
	* @DESC:Retorna un long con el valor asociado a key.
	*/
	long	  metadata_get_long_value(t_metadata*, char *key);

	/**
	* @NAME: config_get_double_value
	* @DESC:Retorna un double con el valor asociado a key.
	*/
	double 	  metadata_get_double_value(t_metadata*, char *key);

	/**
	* @NAME: config_get_array_value
	* @DESC: Retorna un array con los valores asociados a la key especificada.
	* En el archivo de configuracion un valor de este tipo debería ser representado
	* de la siguiente forma [lista_valores_separados_por_coma]
	* Ejemplo:
	* VALORES=[1,2,3,4,5]
	* El array que devuelve termina en NULL
	*/
	char**    metadata_get_array_value(t_metadata*, char* key);

	/**
	* @NAME: config_key_amount
	* @DESC: Retorna la cantidad de keys.
	*/
	int 	  metadata_keys_amount(t_metadata*);

	/**
	* @NAME: config_destroy
	* @DESC: Destruye la estructura config.
	*/
	void 	  metadata_destroy(t_metadata *metadata);

	/**
	* @NAME: config_set_value
	* @DESC: Setea el valor en el archivo de config, a la key asociada.
	*/
	void      metadata_set_value(t_metadata*, char *key, char *value);

	/**
	* @NAME: config_remove_key
	* @DESC: Remueve la clave y su valor asociado del archivo de config.
	*/
	void      metadata_remove_key(t_metadata*, char *key);

	/**
	* @NAME: config_save
	* @DESC: Reescribe el archivo de configuracion con los valores del config.
	*/
	int       metadata_save(t_metadata*);

	/**
	* @NAME: config_save_in_file
	* @DESC: Escribe un archivo de configuracion en el path indicado con los valores del config.
	*/
	int       metadata_save_in_file(t_metadata*, char *path);

#endif /* UTILS_METADATA_H_ */
