#include "pruebas.h"

int main()
{
    printf("Algoritmo Genético para el Ciclo Hamiltoniano\n");
    printf("=============================================\n");
    int ngens = 1000, TPoblacion = 100; // ngens = numero de generaciones     TPoblacion = tamaño poblacion

    printf("Cargando grafo desde archivo...\n");
    grafo MCostes = cargar_grafo("Pruebas/eil51.tsp");
    int* SolucionOptima = cargar_solucion("Pruebas/eil51.opt.tour");

    printf("Ejecutando Pruebas Tiempos\n");
    pruebaSecuencial(TPoblacion, ngens, &MCostes);
    pruebaParalela(TPoblacion, ngens, &MCostes);
    pruebaSecuencialSpike(TPoblacion, ngens, &MCostes);
    pruebaParalelaSpike(TPoblacion, ngens, &MCostes);
    printf("\nPruebas Finalizadas\n");

    printf("Ejecutando Pruebas Comportamiento\n");
    AlgSec(ngens, TPoblacion, &MCostes, 1, 1);
    for(int i = 1; i <= 8; i++)
        AlgSecNeurona(ngens, TPoblacion, &MCostes, i, 1, 1);
    AlgPall(ngens, TPoblacion, &MCostes, 8, 1, 1);
    for(int i = 1; i <= 8; i++)
        AlgPallNeurona(ngens, TPoblacion, &MCostes, 8, i, 1, 1);
    printf("\nPruebas Finalizadas\n");

    free(SolucionOptima);
    liberar_grafo(&MCostes);
    return 0;
}
