#ifndef SELECCION_H
#define SELECCION_H

#include "poblacion.h"
#include "grafo.h"
// Prototipo de la función
// void seleccion(poblacion *poblacion, int *individuo, int **costes, int tamaño)
void seleccion(poblacion *poblacion, grafo* g, int *individuo);

#endif // SELECCION_H