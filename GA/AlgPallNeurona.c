#include "Alg.h"

int *AlgPallNeurona(int ngens, int TPoblacion, grafo *MCostes,
                    int num_hilos, int tipo_neurona,
                    float pUmbral, int datosPoblacion, int datosMutacion)
{
    int interval_migracion = 200;
    int num_migrantes = 5;
    int num_nodos = MCostes->num_nodos;

    omp_set_num_threads(num_hilos);
    init_seeds_num_threads(num_hilos);

    /* ========== ISLAS ========== */
    poblacion *islas = malloc(num_hilos * sizeof(poblacion));
    int isla_size = TPoblacion / num_hilos;
    for (int i = 0; i < num_hilos; i++)
        islas[i] = crear_poblacion(isla_size, num_nodos);

    /* ========== MEJOR GLOBAL ========== */
    int *MejorSolucion = malloc(num_nodos * sizeof(int));
    int CosteMejorSolucion = -1;

    /* ========== MIGRACIÓN ========== */
    int **mejores_islas = malloc(num_hilos * sizeof(int *));
    int *costes_islas = malloc(num_hilos * sizeof(int));
    for (int i = 0; i < num_hilos; i++)
        mejores_islas[i] = NULL;

    /* ========== CARPETAS ========== */
    const char *nombreNeurona[] =
        {"RS","IB","CH","FS","TC1","TC2","RZ","LTS"};

    char base[256];
    snprintf(base, sizeof(base),
             "Analisis/Datos/Paralelo/Neurona_%s",
             nombreNeurona[tipo_neurona-1]);

    if (datosPoblacion || datosMutacion) {
        mkdir("Analisis", 0777);
        mkdir("Analisis/Datos", 0777);
        mkdir("Analisis/Datos/Paralelo", 0777);
        mkdir(base, 0777);
    }

    /* ========== MUTACIÓN ========== */
    FILE **fMut = NULL;
    if (datosMutacion) {
        char dirM[256];
        snprintf(dirM, sizeof(dirM), "%s/Mutacion_%.1f", base, pUmbral);
        mkdir(dirM, 0777);

        fMut = malloc(num_hilos * sizeof(FILE *));
        for (int i = 0; i < num_hilos; i++) {
            char nombre[256];
            snprintf(nombre, sizeof(nombre),
                     "%s/Isla%d.txt", dirM, i);
            fMut[i] = fopen(nombre, "w");
        }
    }

    /* ========== POBLACIÓN ========== */
    char dirP[256];
    if (datosPoblacion) {
        snprintf(dirP, sizeof(dirP), "%s/Poblacion_%.1f", base, pUmbral);
        mkdir(dirP, 0777);

        for (int i = 0; i < num_hilos; i++) {
            char dirIsla[256];
            snprintf(dirIsla, sizeof(dirIsla),
                     "%s/Isla%d", dirP, i);
            mkdir(dirIsla, 0777);
        }
    }

    /* ========== ALGORITMO PARALELO ========== */
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        poblacion *pob = &islas[tid];

        /* Neurona por hilo */
        Neurona n;
        switch (tipo_neurona) {
            case 1: n = crear_neurona_RS();  break;
            case 2: n = crear_neurona_IB();  break;
            case 3: n = crear_neurona_CH();  break;
            case 4: n = crear_neurona_FS();  break;
            case 5: n = crear_neurona_TC1(); break;
            case 6: n = crear_neurona_TC2(); break;
            case 7: n = crear_neurona_RZ();  break;
            case 8: n = crear_neurona_LTS(); break;
        }

        int *mejor_local = malloc(num_nodos * sizeof(int));
        memcpy(mejor_local, pob->individuos[0],
               num_nodos * sizeof(int));
        int coste_mejor_local = evaluar(mejor_local, MCostes);

        int *padre = malloc(num_nodos * sizeof(int));
        int *madre = malloc(num_nodos * sizeof(int));

        for (int g = 0; g < ngens; g++) {

            for (int k = 0; k < pob->num_individuos; k++) {

                emparejamiento_random(*pob, padre, madre, num_nodos);
                int *hijo = cruce(padre, madre, MCostes);

                int muta = 0;
                if (neurona_get_v(&n) > (30.0f - ((1 - pUmbral) * (30.0f - neurona_get_c(&n))))) {
                    mutacion(hijo, num_nodos);
                    muta = 1;
                }
                spike_neurona(&n, 15.0f);

                int coste_hijo = evaluar(hijo, MCostes);
                int entra = seleccion(pob, MCostes, hijo, coste_hijo);

                if (datosMutacion && fMut[tid])
                    fprintf(fMut[tid], "%d %d %d\n", g, muta, entra);

                if (coste_hijo != -1 &&
                    coste_hijo < coste_mejor_local) {
                    memcpy(mejor_local, hijo,
                           num_nodos * sizeof(int));
                    coste_mejor_local = coste_hijo;
                }

                free(hijo);
            }

            /* Guardar población */
            if (datosPoblacion) {
                char nombre[256];
                snprintf(nombre, sizeof(nombre),
                         "%s/Isla%d/Gen%d.txt",
                         dirP, tid, g);

                FILE *f = fopen(nombre, "w");
                if (f) {
                    for (int j = 0; j < pob->num_individuos; j++) {
                        fprintf(f, "%d ", j);
                        for (int n2 = 0; n2 < num_nodos; n2++) {
                            fprintf(f, "%d", pob->individuos[j][n2]);
                            if (n2 < num_nodos - 1) fprintf(f, "-");
                        }
                        fprintf(f, " %d\n",
                                evaluar(pob->individuos[j], MCostes));
                    }
                    fclose(f);
                }
            }

            /* Migración */
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

        free(padre);
        free(madre);
        free(mejor_local);
    }

    /* ========== LIBERAR ========== */
    if (datosMutacion) {
        for (int i = 0; i < num_hilos; i++)
            if (fMut[i]) fclose(fMut[i]);
        free(fMut);
    }

    for (int i = 0; i < num_hilos; i++)
        if (mejores_islas[i]) free(mejores_islas[i]);
    free(mejores_islas);
    free(costes_islas);

    for (int i = 0; i < num_hilos; i++)
        liberar_poblacion(&islas[i]);
    free(islas);

    free(seeds);
    seeds = NULL;

    return MejorSolucion;
}

int *AlgPallNeurona_DEF(int ngens, int TPoblacion, grafo *MCostes,
                    int num_hilos, int tipo_neurona)
{
    return AlgPallNeurona(ngens, TPoblacion, MCostes, num_hilos, tipo_neurona, 0.6f, 0, 0);
}
