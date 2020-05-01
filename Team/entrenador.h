#include <stdio.h>
#include <stdlib.h>

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
	t_pokemon objetivo[];
	t_posicion posicion;
	t_pokemon atrapados[];

} t_entrenador;

typedef struct
{
	t_pokemon objetivoGlobal[];
	t_posicion posicion;
	t_pokemon atrapadosGlobal[];

} t_team;

t_posicion moverse_A(t_posicion* posicion,t_posicion* posicionAMoverse);

void moverse_derecha(t_posicion* posicion);

void moverse_arriba(t_posicion* posicion);

void moverse_abajo(t_posicion* posicion);

_Bool sePuedeMover(int posicion);

void ciclos_de_cpu(int ciclos);

int transformarCiclos(int ciclos);
