#ifndef ENTRENADOR_H_
#define ENTRENADOR_H_


#include <stdio.h>
#include <stdlib.h>
#include<commons/collections/list.h>

typedef enum{

	READY = 1,
	BLOCKED = 2

} op_estado;

typedef struct
{
	int x;
	int y;

} t_posicion;


typedef struct
{
	char* especie;
	char* tipo;
	t_posicion posicion;

} t_pokemon;


typedef struct
{
	int ciclos_de_cpu;
	op_estado estado;
	t_list* objetivo;
	t_posicion posicion;
	t_list* atrapados;

} t_entrenador;


t_entrenador* moverse_A(t_entrenador* entrenador, t_posicion* posicionAMoverse);
t_entrenador atrapar_Pokemon(t_entrenador* entrenador, t_pokemon* pokemon); //PREGUNTAR POR ESTA FUNCION
void efectuar_ciclo_cpu(t_entrenador entrenador, int ciclos);
void contabilizar_ciclos(t_entrenador* entrenador, int ciclos);
int transformarCiclos(int ciclos, t_config* config);
void ciclos_de_cpu(int ciclos);
t_entrenador* armar_entrenador(t_config* config, int indice);
t_posicion* obtener_posicion(char* posicion);
t_list* obtener_objetivos(char* objetivos);
t_list* obtener_atrapados(char* atrapados);
int cantidad_de_elementos(char* pokemons);

#endif /* ENTRENADOR_H_ */
