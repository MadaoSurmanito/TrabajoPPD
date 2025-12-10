#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include "grafo.h"
#include "poblacion.h"
#include "evaluar.h"
#include "cruce.h"
#include "mutacion.h"
#include "seleccion.h"
#include "neurona.h"

#define PROB_MUTACION 5 // Probabilidad de mutación en porcentaje
int *algGen_CHamiltoniano_ini(int, int, grafo *, int);
int *algGen_CHamiltoniano(int, int, grafo *, Neurona *);

int main()
{
    printf("Algoritmo Genético para el Ciclo Hamiltoniano\n");
    printf("=============================================\n");
    int ngens = 2000000, TPoblacion = 20; // ngens = numero de generaciones     TPoblacion = tamaño poblacion

    printf("Cargando grafo desde archivo...\n");
    grafo MCostes = cargar_grafo("Pruebas/eil51.tsp");

    printf("Ejecutando algoritmo genético...\n");
    int *MejorSolucion = algGen_CHamiltoniano_ini(ngens, TPoblacion, &MCostes, 1);

    printf("\nMejor Solucion Encontrada:\n");

    for (int i = 0; i < MCostes.num_nodos; i++)
        printf("%d ", MejorSolucion[i]);

    printf("\nCoste : %i\n", evaluar(MejorSolucion, &MCostes));

    printf("\nSolución Óptima del fichero:\n");

    int *SolucionOptima = cargar_solucion("Pruebas/eil51.opt.tour");
    printf("%d ", SolucionOptima[50]);

    for (int i = 0; i < MCostes.num_nodos; i++)
        printf("%d ", SolucionOptima[i]);

    printf("\nCoste : %i\n", evaluar(SolucionOptima, &MCostes));

    free(SolucionOptima);
    liberar_grafo(&MCostes);

    return 0;
}

int *algGen_CHamiltoniano_ini(int ngens, int TPoblacion, grafo *MCostes, int tipo_neurona)
{
    Neurona n;
    switch (tipo_neurona)
    {
    case 1:
        n = crear_neurona_RS();
    case 2:
        n = crear_neurona_IB();
    case 3:
        n = crear_neurona_CH();
    case 4:
        n = crear_neurona_FS();
    case 5:
        n = crear_neurona_TC1();
    case 6:
        n = crear_neurona_TC2();
    case 7:
        n = crear_neurona_RZ();
    case 8:
        n = crear_neurona_LTS();
    default:
        printf("Tipo de neurona no válido. Usando Regular Spiking (RS) por defecto.\n");
        n = crear_neurona_RS();
    }
    return algGen_CHamiltoniano(ngens, TPoblacion, MCostes, &n);
}

int *algGen_CHamiltoniano(int ngens, int TPoblacion, grafo *MCostes, Neurona *n)
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

        emparejamiento(pob, padre, madre, MCostes->num_nodos); // Devvuelve 2 individuos(padre y madre)

        hijo = cruce(padre, madre, MCostes); // Crea un nuevo individuo válido(hijo)

        // La prob de mutacion va fuera, no en la funcion
        int prob_mutacion = rand() % 100;
        if (prob_mutacion < probabilidad_spike(n))
            mutacion(hijo, MCostes->num_nodos); // Modifica al hijo dentro de una probabilidad
        spike_neurona(n);                       // Actualiza el estado de la neurona
        seleccion(&pob, MCostes, hijo);         // Intenta insertar al hijo en la población

        if (CosteMejorSolucion != -1 && CosteMejorSolucion > evaluar(hijo, MCostes))
        {
            MejorSolucion = hijo;
            CosteMejorSolucion = evaluar(hijo, MCostes);
        }
    }

    // liberar_poblacion(&pob); (No funciona por alguna razón)

    return MejorSolucion;
}