#ifndef ALG_H
#define ALG_H

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../Estructuras/grafo.h"
#include "../Estructuras/poblacion.h"
#include "../Estructuras/neurona.h"

#include "../Operadores/emparejamiento.h"
#include "../Operadores/cruce.h"
#include "../Operadores/mutacion.h"
#include "../Operadores/seleccion.h"
#include "../Operadores/evaluar.h"
#include "../Operadores/seeds.h"
#include "../Operadores/evaluarPob.h"

int *AlgSec(int ngens, int TPoblacion, grafo *MCostes, int datosPoblacion, int datosMutacion);
int *AlgSec_DEF(int ngens, int TPoblacion, grafo *MCostes);

int *AlgSecNeurona(int ngens, int TPoblacion, grafo *MCostes, int tipo_neurona, int datosPoblacion, int datosMutacion);
int *AlgSecNeurona_DEF(int ngens, int TPoblacion, grafo *MCostes, int tipo_neurona);

int *AlgPall(int ngens, int TPoblacion, grafo *MCostes, int num_hilos, int datosPoblacion, int datosMutacion);
int *AlgPall_DEF(int ngens, int TPoblacion, grafo *MCostes, int num_hilos);

int *AlgPallNeurona(int ngens, int TPoblacion, grafo *MCostes, int num_hilos, int tipo_neurona, int datosPoblacion, int datosMutacion);
int *AlgPallNeurona_DEF(int ngens, int TPoblacion, grafo *MCostes, int num_hilos, int tipo_neurona);

int *algGen_CHamiltoniano(int ngens, int TPoblacion, grafo *MCostes,
                          Neurona *n, const char *nombreNeurona,
                          int datosPoblacion, int datosMutacion);

#endif // ALG_H
