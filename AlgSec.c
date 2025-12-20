#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Estructuras/grafo.h"
#include "Estructuras/poblacion.h"
#include "Operadores/evaluar.h"
#include "Operadores/cruce.h"
#include "Operadores/mutacion.h"
#include "Operadores/seleccion.h"
#include "Operadores/seeds.h"

int *AlgSec(int ngens, int TPoblacion, grafo *MCostes)
{
    poblacion pob = crear_poblacion(TPoblacion, MCostes->num_nodos);
    int *MejorSolucion = malloc(sizeof(int) * MCostes->num_nodos);
    memcpy(MejorSolucion, pob.individuos[0], sizeof(int) * MCostes->num_nodos);

    int CosteMejorSolucion = evaluar(MejorSolucion, MCostes);

    for (int i = 0; i < ngens; i++)
    {
        for (int k = 0; k < TPoblacion; k++) {
            int madre[MCostes->num_nodos], padre[MCostes->num_nodos], *hijo;
            emparejamiento(pob, padre, madre, MCostes->num_nodos);
            hijo = cruce(padre, madre, MCostes);

            double prob_mutacion = (double)RAND() / (double)RAND_MAX;
            if (prob_mutacion < 0.5)
                mutacion(hijo, MCostes->num_nodos);

            seleccion(&pob, MCostes, hijo);

            int costeHijo = evaluar(hijo, MCostes);
            if (CosteMejorSolucion == -1 || costeHijo < CosteMejorSolucion)
            {
                memcpy(MejorSolucion, hijo, sizeof(int) * MCostes->num_nodos);
                CosteMejorSolucion = costeHijo;
            }

            free(hijo);
        }
    }

    liberar_poblacion(&pob);

    return MejorSolucion;
}
