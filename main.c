#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include "grafo.h"
#include "poblacion.h"
#include "evaluar.h"
#include "cruce.h"
#include "mutacion.h"
#include "seleccion.h"

int *algGen_CHamiltoniano(int, int, grafo *);

int main()
{
    printf("Algoritmo Genético para el Ciclo Hamiltoniano\n");
    printf("=============================================\n");
    int ngens = 10000, TPoblacion = 20; // ngens = numero de generaciones     TPoblacion = tamaño poblacion

    printf("Cargando grafo desde archivo...\n");
    grafo MCostes = cargar_grafo("Pruebas/eil51.tsp");

    printf("Ejecutando algoritmo genético...\n");
    int *MejorSolucion = algGen_CHamiltoniano(ngens, TPoblacion, &MCostes);

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

    free(SolucionOptima);
    liberar_grafo(&MCostes);

    return 0;
}

int *algGen_CHamiltoniano(int ngens, int TPoblacion, grafo *MCostes)
{
    printf("Generando población inicial...\n");
    poblacion pob = crear_poblacion(TPoblacion, MCostes->num_nodos); // Genera los primeros individuos
    int *MejorSolucion = pob.individuos[1];                          // Inicialmente cualquiera vale
    printf("Iniciando evolución genética...\n");
    int CosteMejorSolucion = evaluar(MejorSolucion, MCostes);

    printf("Evolucionando por %d generaciones...\n", ngens);
    for (int i = 0; i < ngens; i++)
    {
        int madre[MCostes->num_nodos], padre[MCostes->num_nodos], *hijo;
        printf("Generación %d/%d\n", i + 1, ngens);
        printf("---------------------\n");
        printf("Ejecutando emparejamiento...\n");
        emparejamiento(pob, padre, madre, MCostes->num_nodos); // Devvuelve 2 individuos(padre y madre)
        printf("---------------------\n");
        printf("Ejecutando cruce...\n");
        hijo = cruce(padre, madre, MCostes); // Crea un nuevo individuo válido(hijo)
        printf("---------------------\n");
        printf("Ejecutando mutación...\n");
        mutacion(hijo, MCostes->num_nodos, 5); // Modifica al hijo dentro de una probabilidad
        printf("---------------------\n");
        printf("Ejecutando selección...\n");
        seleccion(&pob, MCostes, hijo); // Intenta insertar al hijo en la población
        printf("---------------------\n");
        
        if (CosteMejorSolucion != -1 && CosteMejorSolucion > evaluar(hijo, MCostes))
        {
            MejorSolucion = hijo;
            CosteMejorSolucion = evaluar(hijo, MCostes);
        }
    }

    //liberar_poblacion(&pob); (No funciona por alguna razón)

    return MejorSolucion;
}