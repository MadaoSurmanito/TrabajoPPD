#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"
#include "poblacion.h"
#include "evaluar.h"
#include "cruce.h"

int *algGen_CHamiltoniano(int, int, grafo *);

int main()
{
    printf("Algoritmo Genético para el Ciclo Hamiltoniano\n");
    printf("=============================================\n");
    int ngens = 200, TPoblacion = 20; // ngens = numero de generaciones     TPoblacion = tamaño poblacion

    printf("Cargando grafo desde archivo...\n");
    grafo MCostes = cargar_grafo("Pruebas/eil51.tsp");

    printf("Ejecutando algoritmo genético...\n");
    int *MejorSolucion = algGen_CHamiltoniano(ngens, TPoblacion, &MCostes);

    printf("Mejor Solucion Encontrada:\n");
    for (int i = 0; i < MCostes.num_nodos; i++)
    {
        printf("%d ", MejorSolucion[i]);
    }
    printf("\n");
    return 0;
}

int *algGen_CHamiltoniano(int ngens, int TPoblacion, grafo *MCostes)
{
    printf("Generando población inicial...\n");
    poblacion pob = crear_poblacion(TPoblacion, MCostes->num_nodos); // Genera los primeros individuos
    int *MejorSolucion = pob.individuos[1];                          // Inicialmente cualquiera vale
    printf("Iniciando evolución genética...\n");
    int CosteMejorSolucion = evaluar(MejorSolucion, MCostes->coste, MCostes->num_nodos);

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
        mutacion(hijo, MCostes->num_nodos); // Modifica al hijo dentro de una probabilidad
        printf("---------------------\n");
        printf("Ejecutando selección...\n");
        seleccion(pob, hijo, TPoblacion); // Modifica o no la Poblacion

        if (CosteMejorSolucion != -1 && CosteMejorSolucion > evaluar(hijo, MCostes->coste, MCostes->num_nodos))
        {
            MejorSolucion = hijo;
            CosteMejorSolucion = evaluar(hijo, MCostes->coste, MCostes->num_nodos);
        }
    }
    return MejorSolucion;
}