#include "Alg.h"

int *AlgPall(int ngens, int TPoblacion, grafo *MCostes,
             int num_hilos, int datosPoblacion, int datosMutacion)
{
    int interval_migracion = 200;
    int num_migrantes = 5;

    omp_set_num_threads(num_hilos);
    init_seeds_num_threads(num_hilos);

    int num_nodos = MCostes->num_nodos;

    /* ================= CREAR ISLAS ================= */
    poblacion *islas = malloc(num_hilos * sizeof(poblacion));
    int isla_size = TPoblacion / num_hilos;
    for (int i = 0; i < num_hilos; i++)
        islas[i] = crear_poblacion(isla_size, num_nodos);

    /* ================= MEJOR GLOBAL ================= */
    int *MejorSolucion = malloc(sizeof(int) * num_nodos);
    int CosteMejorSolucion = -1;

    /* ================= MIGRACIÓN ================= */
    int **mejores_islas = malloc(num_hilos * sizeof(int *));
    int *costes_islas = malloc(num_hilos * sizeof(int));

    /* ================= DIRECTORIOS ================= */
    if (datosPoblacion || datosMutacion) {
        mkdir("Analisis", 0777);
        mkdir("Analisis/Datos", 0777);
        mkdir("Analisis/Datos/Paralelo", 0777);
        mkdir("Analisis/Datos/Paralelo/SinNeurona", 0777);
        mkdir("Analisis/Datos/Paralelo/SinNeurona/Mutacion", 0777);
        mkdir("Analisis/Datos/Paralelo/SinNeurona/Poblacion", 0777);
    }

    /* ================= MUTACIÓN ================= */
    FILE **fMut = NULL;
    if (datosMutacion) {
        fMut = malloc(num_hilos * sizeof(FILE *));
        for (int i = 0; i < num_hilos; i++) {
            char nombre[256];
            sprintf(nombre,
                    "Analisis/Datos/Paralelo/SinNeurona/Mutacion/Isla%d.txt", i);
            fMut[i] = fopen(nombre, "w");
        }
    }

    /* ================= POBLACIÓN ================= */
    if (datosPoblacion) {
        for (int i = 0; i < num_hilos; i++) {
            char dir[256];
            sprintf(dir,
                    "Analisis/Datos/Paralelo/SinNeurona/Poblacion/Isla%d", i);
            mkdir(dir, 0777);
        }
    }

    /* ================= ALGORITMO PARALELO ================= */
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        poblacion *pob = &islas[tid];

        int *mejor_local = malloc(num_nodos * sizeof(int));
        memcpy(mejor_local, pob->individuos[0], sizeof(int) * num_nodos);
        int coste_mejor_local = evaluar(mejor_local, MCostes);

        for (int g = 0; g < ngens; g++) {

            for (int k = 0; k < pob->num_individuos; k++) {

                int padre[num_nodos], madre[num_nodos];
                emparejamiento_random(*pob, padre, madre, num_nodos);

                int *hijo = cruce(padre, madre, MCostes);

                int muta = 0;
                int entra = 0;

                if ((double)RAND() / RAND_MAX < 0.5) {
                    mutacion(hijo, num_nodos);
                    muta = 1;
                }

                int coste_hijo = evaluar(hijo, MCostes);
                entra = seleccion(pob, MCostes, hijo, coste_hijo);

                if (datosMutacion) {
                    fprintf(fMut[tid], "%d %d %d\n", g, muta, entra);
                }

                if (coste_hijo != -1 &&
                    (coste_mejor_local == -1 ||
                     coste_hijo < coste_mejor_local)) {

                    memcpy(mejor_local, hijo, sizeof(int) * num_nodos);
                    coste_mejor_local = coste_hijo;
                }

                free(hijo);
            }

            /* ===== GUARDAR POBLACIÓN ===== */
            if (datosPoblacion) {
                char nombre[256];
                sprintf(nombre,
                        "Analisis/Datos/Paralelo/SinNeurona/Poblacion/Isla%d/Gen%d.txt",
                        tid, g);

                FILE *f = fopen(nombre, "w");
                if (f) {
                    for (int j = 0; j < pob->num_individuos; j++) {
                        fprintf(f, "%d ", j);
                        for (int n = 0; n < num_nodos; n++) {
                            fprintf(f, "%d", pob->individuos[j][n]);
                            if (n < num_nodos - 1) fprintf(f, "-");
                        }
                        fprintf(f, " %d\n",
                                evaluar(pob->individuos[j], MCostes));
                    }
                    fclose(f);
                }
            }

            /* ===== MIGRACIÓN ===== */
            if ((g + 1) % interval_migracion == 0) {

                mejores_islas[tid] = malloc(num_nodos * sizeof(int));
                memcpy(mejores_islas[tid], mejor_local,
                       num_nodos * sizeof(int));
                costes_islas[tid] = coste_mejor_local;

                #pragma omp barrier

                int origen = (tid - 1 + num_hilos) % num_hilos;
                for (int m = 0; m < num_migrantes; m++) {
                    int idx = m % pob->num_individuos;
                    memcpy(pob->individuos[idx],
                           mejores_islas[origen],
                           num_nodos * sizeof(int));
                }

                #pragma omp critical
                {
                    if (CosteMejorSolucion == -1 ||
                        coste_mejor_local < CosteMejorSolucion) {

                        memcpy(MejorSolucion, mejor_local,
                               num_nodos * sizeof(int));
                        CosteMejorSolucion = coste_mejor_local;
                    }
                }
            }
        }

        free(mejor_local);
    }

    /* ================= LIBERAR ================= */
    if (datosMutacion) {
        for (int i = 0; i < num_hilos; i++)
            fclose(fMut[i]);
        free(fMut);
    }

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

int *AlgPall_DEF(int ngens, int TPoblacion, grafo *MCostes, int num_hilos)
{
    return AlgPall(ngens, TPoblacion, MCostes, num_hilos, 0, 0);
}
