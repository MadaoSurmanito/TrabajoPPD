#ifndef ALG_H
#define ALG_H

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Estructuras/grafo.h"
#include "Estructuras/poblacion.h"
#include "Estructuras/neurona.h"
#include "Operadores/evaluar.h"
#include "Operadores/cruce.h"
#include "Operadores/mutacion.h"
#include "Operadores/seleccion.h"

int *AlgSec(int ngens, int TPoblacion, grafo *MCostes);

int *AlgSecNeurona(int ngens, int TPoblacion, grafo *MCostes, int tipo_neurona);

int *algGen_CHamiltoniano(int ngens, int TPoblacion, grafo *MCostes, Neurona *n);

#endif // ALG_H
