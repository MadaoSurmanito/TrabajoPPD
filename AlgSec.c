#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include "Estructuras/grafo.h"
#include "Estructuras/poblacion.h"
#include "Operadores/evaluar.h"
#include "Operadores/cruce.h"
#include "Operadores/mutacion.h"
#include "Operadores/seleccion.h"

int *AlgSec(int ngens, int TPoblacion, grafo *MCostes)
{
    printf("Generando población inicial...\n");
    poblacion pob = crear_poblacion(TPoblacion, MCostes->num_nodos); // Genera los primeros individuos
    int *MejorSolucion = malloc(sizeof(int) * MCostes->num_nodos);   // Inicialmente cualquiera vale
    memcpy(MejorSolucion, pob.individuos[1], sizeof(int) * MCostes->num_nodos);      

    printf("Iniciando evolución genética...\n");
    int CosteMejorSolucion = evaluar(MejorSolucion, MCostes);

    printf("Evolucionando por %d generaciones...\n", ngens);
    for (int i = 0; i < ngens; i++)
    {
        int madre[MCostes->num_nodos], padre[MCostes->num_nodos], *hijo;
        printf("Generación %d/%d\n", i + 1, ngens);
        emparejamiento(pob, padre, madre, MCostes->num_nodos); // Devuelve 2 individuos(padre y madre)
        hijo = cruce(padre, madre, MCostes); // Crea un nuevo individuo válido(hijo)
        mutacion(hijo, MCostes->num_nodos); // Modifica al hijo dentro de una probabilidad
        seleccion(&pob, MCostes, hijo); // Intenta insertar al hijo en la población
        
        if (CosteMejorSolucion == -1 || evaluar(hijo, MCostes) < CosteMejorSolucion)
        {
            memcpy(MejorSolucion, hijo, sizeof(int) * MCostes->num_nodos);
            CosteMejorSolucion = evaluar(hijo, MCostes);
        }
    }

    //liberar_poblacion(&pob); (No funciona por alguna razón)

    return MejorSolucion;
}
