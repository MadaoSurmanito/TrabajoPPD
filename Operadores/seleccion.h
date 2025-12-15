#ifndef SELECCION_H
#define SELECCION_H

#include "../Estructuras/poblacion.h"
#include "../Estructuras/grafo.h"
// void seleccion(poblacion *poblacion, int *individuo, int **costes, int tamaño)
void seleccion(poblacion *poblacion, grafo* g, int *individuo);

#endif // SELECCION_H