#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "metadata.h"
#include "string.h"
#include <commons/collections/dictionary.h>


void leer_metadata(char* path_metadata)
{
	metadata = metadata_create(path_metadata);

	if(metadata == NULL){
		printf("No se pudo leer el archivo metadata.");
		exit(2);
	}
}

char* obtener_bloques_metada(){
	char* bloques;
	bloques = metadata_get_string_value(metadata, "BLOQUES");
	return bloques;
}

t_metadata *metadata_create(char *path) {
	FILE* file = fopen(path, "r");

	if (file == NULL) {
		return NULL;
	}

	struct stat stat_file;
	stat(path, &stat_file);

	t_metadata *metadata = malloc(sizeof(t_metadata));

	metadata->path = strdup(path);
	metadata->properties = dictionary_create();

	char* buffer = calloc(1, stat_file.st_size + 1);
	fread(buffer, stat_file.st_size, 1, file);

	if (strstr(buffer, "\r\n")) {
		printf("\n\nconfig_create - WARNING: the file %s contains a \\r\\n sequence "
		 "- the Windows new line sequence. The \\r characters will remain as part "
		 "of the value, as Unix newlines consist of a single \\n. You can install "
		 "and use `dos2unix` program to convert your files to Unix style.\n\n", path);
	}

	char** lines = string_split(buffer, "\n");

	void add_cofiguration(char *line) {
		if (!string_starts_with(line, "#")) {
			char** keyAndValue = string_n_split(line, 2, "=");
			dictionary_put(metadata->properties, keyAndValue[0], keyAndValue[1]);
			free(keyAndValue[0]);
			free(keyAndValue);
		}
	}
	string_iterate_lines(lines, add_cofiguration);
	string_iterate_lines(lines, (void*) free);

	free(lines);
	free(buffer);
	fclose(file);

	return metadata;
}

bool metadata_has_property(t_metadata *self, char* key) {
	return dictionary_has_key(self->properties, key);
}

char *metadata_get_string_value(t_metadata *self, char *key) {
	return dictionary_get(self->properties, key);
}

int metadata_get_int_value(t_metadata *self, char *key) {
	char *value = metadata_get_string_value(self, key);
	return atoi(value);
}

long metadata_get_long_value(t_metadata *self, char *key) {
	char *value = metadata_get_string_value(self, key);
	return atol(value);
}

double metadata_get_double_value(t_metadata *self, char *key) {
	char *value = metadata_get_string_value(self, key);
	return atof(value);
}

char** metadata_get_array_value(t_metadata* self, char* key) {
	char* value_in_dictionary = config_get_string_value(self, key);
	return string_get_string_as_array(value_in_dictionary);
}

int metadata_keys_amount(t_metadata *self) {
	return dictionary_size(self->properties);
}

void metadata_destroy(t_metadata *metadata) {
	dictionary_destroy_and_destroy_elements(metadata->properties, free);
	free(metadata->path);
	free(metadata);
}

void metadata_set_value(t_metadata *self, char *key, char *value) {
	metadata_remove_key(self, key);

	char* duplicate_value = string_duplicate(value);

	dictionary_put(self->properties, key, (void*)duplicate_value);
}

void metadata_remove_key(t_metadata *self, char *key) {
	t_dictionary* dictionary = self->properties;

	if(dictionary_has_key(dictionary, key)) {
		dictionary_remove_and_destroy(dictionary, key, free);
	}
}

int metadata_save(t_metadata *self) {
	return metadata_save_in_file(self, self->path);
}

int metadata_save_in_file(t_metadata *self, char* path) {
	FILE* file = fopen(path, "wb+");

	if (file == NULL) {
			return -1;
	}

	char* lines = string_new();
	void add_line(char* key, void* value) {
		string_append_with_format(&lines, "%s=%s\n", key, value);
	}

	dictionary_iterator(self->properties, add_line);
	int result = fwrite(lines, strlen(lines), 1, file);
	fclose(file);
	free(lines);
	return result;
}
