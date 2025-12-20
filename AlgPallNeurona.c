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
#include "Estructuras/neurona.h"
#include "Operadores/seeds.h"

int *AlgPallNeurona(int ngens, int TPoblacion, grafo *MCostes, int num_hilos, int tipo_neurona) 
{
    int interval_migracion = 200;
    int num_migrantes = 5;

    omp_set_num_threads(num_hilos);
    init_seeds_num_threads(num_hilos);

    // Crear subpoblaciones independientes para cada isla
    poblacion *islas = malloc(num_hilos * sizeof(poblacion));
    int isla_size = TPoblacion / num_hilos;
    for (int i = 0; i < num_hilos; i++)
        islas[i] = crear_poblacion(isla_size, MCostes->num_nodos);

    // Mejor global
    int *MejorSolucion = malloc(sizeof(int) * MCostes->num_nodos);
    int CosteMejorSolucion = -1;

    // Arrays temporales para migración
    int **mejores_islas = malloc(num_hilos * sizeof(int*));
    int *costes_islas = malloc(num_hilos * sizeof(int));

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
        }

        int tid = omp_get_thread_num();
        poblacion *pob = &islas[tid];

        // Mejor local de esta isla
        int *mejor_local = malloc(MCostes->num_nodos * sizeof(int));
        memcpy(mejor_local, pob->individuos[0], sizeof(int) * MCostes->num_nodos);
        int coste_mejor_local = evaluar(mejor_local, MCostes);

        for (int g = 0; g < ngens; g++) {
            for (int k = 0; k < pob->num_individuos; k++) {
                int padre[MCostes->num_nodos], madre[MCostes->num_nodos];
                emparejamiento(*pob, padre, madre, MCostes->num_nodos);

                int *hijo = cruce(padre, madre, MCostes);

                if ((double)RAND()/RAND_MAX < probabilidad_spike(&n))
                    mutacion(hijo, MCostes->num_nodos);
                spike_neurona(&n);

                int coste_hijo = evaluar(hijo, MCostes);
                seleccion(pob, MCostes, hijo);

                if (coste_hijo != -1 && (coste_mejor_local == -1 || coste_hijo < coste_mejor_local)) {
                    memcpy(mejor_local, hijo, sizeof(int) * MCostes->num_nodos);
                    coste_mejor_local = coste_hijo;
                }

                free(hijo);
            }

            // Migración periódica
            if ((g+1) % interval_migracion == 0) {
                // Guardar el mejor de cada isla en arrays temporales
                mejores_islas[tid] = malloc(MCostes->num_nodos * sizeof(int));
                memcpy(mejores_islas[tid], mejor_local, MCostes->num_nodos * sizeof(int));
                costes_islas[tid] = coste_mejor_local;

                #pragma omp barrier

                // Migrar de forma cíclica
                int origen = (tid - 1 + num_hilos) % num_hilos;
                for (int m = 0; m < num_migrantes; m++) {
                    int idx_dest = m % pob->num_individuos;
                    memcpy(pob->individuos[idx_dest], mejores_islas[origen],
                           MCostes->num_nodos * sizeof(int));
                }

                // Actualizar mejor global
                #pragma omp critical
                {
                    if (CosteMejorSolucion == -1 || coste_mejor_local < CosteMejorSolucion) {
                        memcpy(MejorSolucion, mejor_local, MCostes->num_nodos * sizeof(int));
                        CosteMejorSolucion = coste_mejor_local;
                    }
                }
            }
        }

        free(mejor_local);
    }

    // Liberar memoria
    for (int i = 0; i < num_hilos; i++)
        free(mejores_islas[i]);
    free(mejores_islas);
    free(costes_islas);

    for (int i = 0; i < num_hilos; i++)
        liberar_poblacion(&islas[i]);
    free(islas);

    free(seeds);
    seeds = NULL;

    return MejorSolucion;
}
