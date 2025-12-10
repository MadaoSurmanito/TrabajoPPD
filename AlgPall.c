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

int *AlgPall(int ngens, int TPoblacion, grafo *MCostes)
{
    init_seeds();

    printf("Generando población inicial...\n");
    poblacion pob = crear_poblacion(TPoblacion, MCostes->num_nodos); // Genera los primeros individuos
    int *MejorSolucion = malloc(sizeof(int) * MCostes->num_nodos);   // Inicialmente cualquiera vale
    memcpy(MejorSolucion, pob.individuos[1], sizeof(int) * MCostes->num_nodos);      

    printf("Iniciando evolución genética...\n");
    int CosteMejorSolucion = evaluar(MejorSolucion, MCostes);

    printf("Evolucionando por %d generaciones...\n", ngens);
    for (int i = 0; i < ngens; i++)
    {
        #pragma omp parallel for shared(pob)
        for (int k = 0; k < TPoblacion; k++) {
            int madre[MCostes->num_nodos], padre[MCostes->num_nodos], *hijo;
            emparejamiento(pob, padre, madre, MCostes->num_nodos); // Devuelve 2 individuos(padre y madre)
            hijo = cruce(padre, madre, MCostes); // Crea un nuevo individuo válido(hijo)
            
            double prob_mutacion = (double)RAND() / (double)RAND_MAX;
            if (prob_mutacion < 0.5)
                mutacion(hijo, MCostes->num_nodos); // Modifica al hijo dentro de una probabilidad
            
            #pragma omp critical
            {
                seleccion(&pob, MCostes, hijo); // Intenta insertar al hijo en la población
                if (CosteMejorSolucion == -1 || evaluar(hijo, MCostes) < CosteMejorSolucion)
                {
                    memcpy(MejorSolucion, hijo, sizeof(int) * MCostes->num_nodos);
                    CosteMejorSolucion = evaluar(hijo, MCostes);
                }
            }

            free(hijo);
        }
    }

    free(seeds);
    seeds = NULL;
    //liberar_poblacion(&pob); (No funciona por alguna razón)

    return MejorSolucion;
}
