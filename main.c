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
    printf("Ejecutando AlgSec (general secuencial)...\n");
    AlgSec(ngens, TPoblacion, &MCostes, 1, 1);
    printf("Ejecutando AlgPall (general paralelo)...\n");
    AlgPall(ngens, TPoblacion, &MCostes, 8, 1, 1);

    // --- BLOQUE 1: Factor 0.8 ---
    printf("Pruebas por neurona con factor 0.8\n");
    for(int i = 1; i <= 8; i++) {
        printf("AlgSecNeurona: neurona %d, factor 0.8\n", i);
        AlgSecNeurona(ngens, TPoblacion, &MCostes, i, 0.8f, 1, 1);
    }
    for(int i = 1; i <= 8; i++) {
        printf("AlgPallNeurona: neurona %d, factor 0.8\n", i);
        AlgPallNeurona(ngens, TPoblacion, &MCostes, 8, i, 0.8f, 1, 1);
    }

    // --- BLOQUE 2: Factor 0.6 ---
    printf("Pruebas por neurona con factor 0.6\n");
    for(int i = 1; i <= 8; i++) {
        printf("AlgSecNeurona: neurona %d, factor 0.6\n", i);
        AlgSecNeurona(ngens, TPoblacion, &MCostes, i, 0.6f, 1, 1);
    }
    for(int i = 1; i <= 8; i++) {
        printf("AlgPallNeurona: neurona %d, factor 0.6\n", i);
        AlgPallNeurona(ngens, TPoblacion, &MCostes, 8, i, 0.6f, 1, 1);
    }

    // --- BLOQUE 3: Factor 0.4 ---
    printf("Pruebas por neurona con factor 0.4\n");
    for(int i = 1; i <= 8; i++) {
        printf("AlgSecNeurona: neurona %d, factor 0.4\n", i);
        AlgSecNeurona(ngens, TPoblacion, &MCostes, i, 0.4f, 1, 1);
    }
    for(int i = 1; i <= 8; i++) {
        printf("AlgPallNeurona: neurona %d, factor 0.4\n", i);
        AlgPallNeurona(ngens, TPoblacion, &MCostes, 8, i, 0.4f, 1, 1);
    }

    printf("\nPruebas Finalizadas\n");

    free(SolucionOptima);
    liberar_grafo(&MCostes);
    return 0;
}
