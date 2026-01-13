#ifndef CRUCE_H
#define CRUCE_H

#include "../Estructuras/grafo.h"
#include "../Estructuras/poblacion.h"

// padre: primer padre
// madre: segundo padre
// MCostes: grafo con los costes
int* cruce(int padre[], int madre[], grafo* MCostes);

#endif // CRUCE_H