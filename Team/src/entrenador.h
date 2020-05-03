#include <stdio.h>
#include <stdlib.h>

typedef enum{

	READY = 1,
	BLOCKED = 2

}op_estado;


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



t_posicion moverse_A(t_posicion* posicion,t_posicion* posicionAMoverse);

void moverse_derecha(t_posicion* posicion);

void moverse_arriba(t_posicion* posicion);

void moverse_abajo(t_posicion* posicion);

_Bool sePuedeMover(int posicion);

void ciclos_de_cpu(int ciclos);

int transformarCiclos(int ciclos);
