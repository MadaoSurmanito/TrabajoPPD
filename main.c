#include "pruebas.h"

int main()
{
    printf("Algoritmo Genético para el Ciclo Hamiltoniano\n");
    printf("=============================================\n");
    int ngens = 200, TPoblacion = 50; // ngens = numero de generaciones     TPoblacion = tamaño poblacion

    printf("Cargando grafo desde archivo...\n");
    grafo MCostes = cargar_grafo("Pruebas/eil51.tsp");
    int* SolucionOptima = cargar_solucion("Pruebas/eil51.opt.tour");
    int costeOptimo = evaluar(SolucionOptima, &MCostes);

    printf("Ejecutando Pruebas\n");
    pruebaSecuencial(TPoblacion, ngens, &MCostes, costeOptimo);
    pruebaParalela(TPoblacion, ngens, &MCostes, costeOptimo);
    pruebaSecuencialSpike(TPoblacion, ngens, &MCostes, costeOptimo);
    pruebaParalelaSpike(TPoblacion, ngens, &MCostes, costeOptimo);
    printf("\nPruebas Finalizadas\n");

    /*
    int *MejorSolucion = AlgSecNeurona(ngens, TPoblacion, &MCostes, 1);

    printf("\nMejor Solucion Encontrada:\n");

    for (int i = 0; i < MCostes.num_nodos; i++)
        printf("%d ", MejorSolucion[i]);

    printf("\nCoste : %i\n", evaluar(MejorSolucion, &MCostes));

    printf("\nSolución Óptima del fichero:\n");
    printf("%d ", SolucionOptima[50]);

    for (int i = 0; i < MCostes.num_nodos; i++)
        printf("%d ", SolucionOptima[i]);
    
    printf("\nCoste : %i\n", evaluar(SolucionOptima, &MCostes));   

    free(SolucionOptima);

    liberar_grafo(&MCostes);
    return 0;
    */
}
