#include <stdio.h>
#include <stdlib.h>

#include "grafo.h"
#include "poblacion.h"
#include "evaluar.h"
#include "cruce.h"
#include "mutacion.h"
#include "seleccion.h"

#include "algHam.h"
#include "seeds.h"

int main()
{

    nthreads = 4;

    printf("Número de hilos: %i\n", nthreads);

    seeds = malloc(sizeof(unsigned int) * nthreads);
    
    for(unsigned int i = 0; i < nthreads; ++i)
        seeds[i] = rand() % nthreads;

    omp_set_num_threads(nthreads);  

    printf("Algoritmo Genético para el Ciclo Hamiltoniano\n");
    printf("=============================================\n");
    int ngens = 10000, TPoblacion = 20; // ngens = numero de generaciones     TPoblacion = tamaño poblacion

    printf("Cargando grafo desde archivo...\n");
    grafo MCostes = cargar_grafo("../Pruebas/eil51.tsp");

    printf("Ejecutando algoritmo genético...\n");
    int *MejorSolucion = algGen_CHamiltoniano(ngens, TPoblacion, &MCostes);

    printf("\nMejor Solucion Encontrada:\n");

    for (int i = 0; i < MCostes.num_nodos; i++)
        printf("%d ", MejorSolucion[i]);

    printf("\nCoste : %i\n", evaluar(MejorSolucion, &MCostes));

    printf("\nSolución Óptima del fichero:\n");

    int* SolucionOptima = cargar_solucion("../Pruebas/eil51.opt.tour");
    printf("%d ", SolucionOptima[50]);

    for (int i = 0; i < MCostes.num_nodos; i++)
        printf("%d ", SolucionOptima[i]);
    
    printf("\nCoste : %i\n", evaluar(SolucionOptima, &MCostes));

    free(seeds);
    free(SolucionOptima);
    liberar_grafo(&MCostes);

    return 0;
}