#ifndef SELECCION_H
#define SELECCION_H

#include <stdlib.h>
#include <string.h>
#include "evaluar.h"
#include "seeds.h"
#include "../Estructuras/poblacion.h"
#include "../Estructuras/grafo.h"

// void seleccion(poblacion *poblacion, int *individuo, int **costes, int tamaño)
int seleccion(poblacion *p, grafo *g, int *individuo, int costeIndividuo);

#endif // SELECCION_H