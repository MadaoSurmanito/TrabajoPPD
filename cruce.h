#ifndef CRUCE_H
#define CRUCE_H

#include "grafo.h"
#include "poblacion.h"

// padre: primer padre
// madre: segundo padre
// MCostes: grafo con los costes
int* cruce(int padre[], int madre[], grafo* MCostes);

// pob: población actual
// padre: arreglo donde se guardará el padre seleccionado
// madre: arreglo donde se guardará la madre seleccionada
// num_nodos: número de nodos en el grafo
void emparejamiento(poblacion pob, int padre[], int madre[], int num_nodos);

#endif // CRUCE_H