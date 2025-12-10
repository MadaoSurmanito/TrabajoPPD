#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include "grafo.h"
#include "poblacion.h"
#include "evaluar.h"
#include "cruce.h"
#include "mutacion.h"
#include "seleccion.h"
#include "algSec.h"
#include "pruebas.h"

int main()
{
    printf("Algoritmo Genético para el Ciclo Hamiltoniano\n");
    printf("=============================================\n");
    int ngens = 2000000, TPoblacion = 20; // ngens = numero de generaciones     TPoblacion = tamaño poblacion

    printf("Cargando grafo desde archivo...\n");
    grafo MCostes = cargar_grafo("Pruebas/eil51.tsp");
    int* SolucionOptima = cargar_solucion("Pruebas/eil51.opt.tour");
    int costeOptimo = evaluar(SolucionOptima, &MCostes);

    printf("Ejecutando algoritmo genético...\n");
    pruebaSecuencial(TPoblacion, ngens, &MCostes, costeOptimo);
    pruebaSecuencialSpike(TPoblacion, ngens, &MCostes, costeOptimo);
    /*int *MejorSolucion = algGen_CHamiltoniano(ngens, TPoblacion, &MCostes);

    printf("\nMejor Solucion Encontrada:\n");

    for (int i = 0; i < MCostes.num_nodos; i++)
        printf("%d ", MejorSolucion[i]);

    printf("\nCoste : %i\n", evaluar(MejorSolucion, &MCostes));

    printf("\nSolución Óptima del fichero:\n");

    int* SolucionOptima = cargar_solucion("Pruebas/eil51.opt.tour");
    printf("%d ", SolucionOptima[50]);

    for (int i = 0; i < MCostes.num_nodos; i++)
        printf("%d ", SolucionOptima[i]);
    
    printf("\nCoste : %i\n", evaluar(SolucionOptima, &MCostes));

    free(SolucionOptima);*/
    liberar_grafo(&MCostes);

    return 0;
}