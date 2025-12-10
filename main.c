#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include "Estructuras/grafo.h"
#include "Estructuras/poblacion.h"
#include "Operadores/evaluar.h"
#include "Operadores/cruce.h"
#include "Operadores/mutacion.h"
#include "Operadores/seleccion.h"
#include "Alg.h"
#include "pruebas.h"

int main()
{
    printf("Algoritmo Genético para el Ciclo Hamiltoniano\n");
    printf("=============================================\n");
    int ngens = 20000, TPoblacion = 20; // ngens = numero de generaciones     TPoblacion = tamaño poblacion

    printf("Cargando grafo desde archivo...\n");
    grafo MCostes = cargar_grafo("Pruebas/eil51.tsp");
    int* SolucionOptima = cargar_solucion("Pruebas/eil51.opt.tour");
    int costeOptimo = evaluar(SolucionOptima, &MCostes);

    printf("Ejecutando algoritmo genético...\n");
    pruebaSecuencial(TPoblacion, ngens, &MCostes, costeOptimo);
    pruebaSecuencialSpike(TPoblacion, ngens, &MCostes, costeOptimo);

    liberar_grafo(&MCostes);
    return 0;
}