#ifndef FITNESS_H
#define FITNESS_H

#include <stdlib.h>
#include <string.h>
#include "../Estructuras/grafo.h"

// Devuelve el coste (-1 si el camino es imposible)
int evaluar(int* individuo, grafo* g);

#endif // FITNESS_H
