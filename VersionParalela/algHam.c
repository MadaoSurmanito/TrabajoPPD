#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "poblacion.h"
#include "evaluar.h"
#include "cruce.h"
#include "mutacion.h"
#include "seleccion.h"
#include "algHam.h"
#include "grafo.h"

#include "seeds.h"

#define PROB_MUTACION 5 // Probabilidad de mutación en porcentaje

int *algGen_CHamiltoniano(int ngens, int TPoblacion, grafo *MCostes)
{

    printf("Generando población inicial...\n");
    poblacion pob = crear_poblacion(TPoblacion, MCostes->num_nodos); // Genera los primeros individuos
    int *MejorSolucion = pob.individuos[1];                          // Inicialmente cualquiera vale
    printf("Iniciando evolución genética...\n");
    int CosteMejorSolucion = evaluar(MejorSolucion, MCostes);

    srand(time(NULL));

    printf("Evolucionando por %d generaciones...\n", ngens);

    #pragma omp parallel for shared(seeds, pob)
    for (int i = 0; i < ngens; i++)
    {
        int madre[MCostes->num_nodos], padre[MCostes->num_nodos], *hijo;
        printf("Generación %d/%d\n", i + 1, ngens);

        emparejamiento(pob, padre, madre, MCostes->num_nodos); // Devvuelve 2 individuos(padre y madre)

        hijo = cruce(padre, madre, MCostes); // Crea un nuevo individuo válido(hijo)
        printf("---------------------\n");
        printf("Ejecutando mutación...\n");

        //La prob de mutacion va fuera, no en la funcion
        int prob_mutacion = RAND() % 100;
        if (prob_mutacion < PROB_MUTACION) mutacion(hijo, MCostes->num_nodos); // Modifica al hijo dentro de una probabilidad
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