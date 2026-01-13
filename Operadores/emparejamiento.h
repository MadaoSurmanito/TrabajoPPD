#ifndef EMPAREJAMIENTO_H
#define EMPAREJAMIENTO_H

#include <string.h>

#include "../Estructuras/grafo.h"
#include "../Estructuras/poblacion.h"

#include "seeds.h"
#include "evaluar.h"

// Función de emparejamiento aleatorio
// pob: población actual de individuos
// padre: arreglo donde se guardará el índice del padre seleccionado
// madre: arreglo donde se guardará el índice de la madre seleccionada
// num_nodos: número de nodos en el grafo, puede usarse para determinar el tamaño de los individuos o para restricciones
void emparejamiento_random(poblacion pob, int padre[], int madre[], int num_nodos);

// Función de emparejamiento basado en un vecindario celular tipo C9
// pob: puntero a la población actual
// idx: índice del individuo que se va a emparejar
// filas, cols: dimensiones de la “rejilla” donde están organizados los individuos
// MCostes: puntero al grafo de costes, utilizado para calcular la aptitud o cercanía entre individuos
// padre, madre: punteros a enteros donde se guardarán los índices de los individuos seleccionados como padre y madre
void emparejamiento_celular_C9(poblacion *pob, int idx,
                               int filas, int cols,
                               grafo *MCostes,
                               int *padre, int *madre);

#endif // EMPAREJAMIENTO_H
