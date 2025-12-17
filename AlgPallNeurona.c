#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include "Estructuras/grafo.h"
#include "Estructuras/poblacion.h"
#include "Operadores/evaluar.h"
#include "Operadores/cruce.h"
#include "Operadores/mutacion.h"
#include "Operadores/seleccion.h"
#include "Estructuras/neurona.h"
#include "Operadores/seeds.h"

int *AlgPallNeurona(int ngens, int TPoblacion, grafo *MCostes, int num_hilos, int tipo_neurona)
{
    omp_set_num_threads(num_hilos);
    init_seeds();

    poblacion pob = crear_poblacion(TPoblacion, MCostes->num_nodos); // Genera los primeros individuos
    int *MejorSolucion = malloc(sizeof(int) * MCostes->num_nodos);   // Inicialmente cualquiera vale
    memcpy(MejorSolucion, pob.individuos[1], sizeof(int) * MCostes->num_nodos);      

    int CosteMejorSolucion = evaluar(MejorSolucion, MCostes);

    #pragma omp parallel
    {
        Neurona n;
        switch (tipo_neurona)
        {
        case 1:
            n = crear_neurona_RS();
            break;
        case 2:
            n = crear_neurona_IB();
            break;
        case 3:
            n = crear_neurona_CH();
            break;
        case 4:
            n = crear_neurona_FS();
            break;
        case 5:
            n = crear_neurona_TC1();
            break;
        case 6:
            n = crear_neurona_TC2();
            break;
        case 7:
            n = crear_neurona_RZ();
            break;
        case 8:
            n = crear_neurona_LTS();
            break;
        default:
            n = crear_neurona_RS();
        }

        for (int i = 0; i < ngens; i++)
        {
            #pragma omp for
            for (int k = 0; k < TPoblacion; k++) {
                int madre[MCostes->num_nodos], padre[MCostes->num_nodos], *hijo;

                #pragma omp critical
                {
                    emparejamiento(pob, padre, madre, MCostes->num_nodos); // Devuelve 2 individuos(padre y madre)
                }
                
                hijo = cruce(padre, madre, MCostes); // Crea un nuevo individuo válido(hijo)
                
                double prob_mutacion = (double)RAND() / (double)RAND_MAX;
                if (prob_mutacion < probabilidad_spike(&n))
                    mutacion(hijo, MCostes->num_nodos); // Modifica al hijo dentro de una probabilidad
                spike_neurona(&n);
                int coste_hijo = evaluar(hijo, MCostes);
                
                #pragma omp critical
                {
                    seleccion(&pob, MCostes, hijo);
                    if (CosteMejorSolucion == -1 || coste_hijo < CosteMejorSolucion) {
                        memcpy(MejorSolucion, hijo, sizeof(int) * MCostes->num_nodos);
                        CosteMejorSolucion = coste_hijo;
                    }
                }

                free(hijo);
            }
        }
    }

    free(seeds);
    seeds = NULL;
    //liberar_poblacion(&pob); (No funciona por alguna razón)

    return MejorSolucion;
}
