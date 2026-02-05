#include "Alg.h"

int *AlgSec(int ngens, int TPoblacion, grafo *MCostes, int datosPoblacion, int datosMutacion)
{
    srand(time(NULL));

    int num_nodos = MCostes->num_nodos;

    poblacion pob = crear_poblacion(TPoblacion, num_nodos);
    int *madre = malloc(sizeof(int) * num_nodos);
    int *padre = malloc(sizeof(int) * num_nodos);

    int *MejorSolucion = malloc(sizeof(int) * num_nodos);
    memcpy(MejorSolucion, pob.individuos[0], sizeof(int) * num_nodos);
    int CosteMejorSolucion = evaluar(MejorSolucion, MCostes);

    if (datosPoblacion || datosMutacion)
    {
        mkdir("Analisis", 0777);
        mkdir("Analisis/Datos", 0777);
        mkdir("Analisis/Datos/Secuencial", 0777);
        mkdir("Analisis/Datos/Secuencial/SinNeurona", 0777);
        mkdir("Analisis/Datos/Secuencial/SinNeurona/Mutacion", 0777);
        mkdir("Analisis/Datos/Secuencial/SinNeurona/Poblacion", 0777);
    }

    FILE *fMut = NULL;
    if (datosMutacion)
        fMut = fopen("Analisis/Datos/Secuencial/SinNeurona/Mutacion/Mutacion.txt", "w");

    for (int i = 0; i < ngens; i++)
    {
        for (int k = 0; k < TPoblacion; k++)
        {
            int muta = 0;
            int entra = 0;

            emparejamiento_random(pob, padre, madre, num_nodos);
            int *hijo = cruce(padre, madre, MCostes);

            double prob_mutacion = (double)rand() / RAND_MAX;
            if (prob_mutacion < 0.5)
            {
                mutacion(hijo, num_nodos);
                muta = 1;
            }

            int costeHijo = evaluar(hijo, MCostes);
            entra = seleccion(&pob, MCostes, hijo, costeHijo);

            if (datosMutacion && fMut)
            {
                fprintf(fMut, "%d %d %d\n", i, muta, entra);
            }

            if (costeHijo < CosteMejorSolucion)
            {
                memcpy(MejorSolucion, hijo, sizeof(int) * num_nodos);
                CosteMejorSolucion = costeHijo;
            }

            free(hijo);
        }

        if (datosPoblacion)
        {
            char nombre[256];
            sprintf(nombre, "Analisis/Datos/Secuencial/SinNeurona/Poblacion/Gen%d.txt", i);

            FILE *f = fopen(nombre, "w");
            if (!f) continue;

            for (int j = 0; j < TPoblacion; j++)
            {
                fprintf(f, "%d ", j);
                for (int n = 0; n < num_nodos; n++)
                {
                    fprintf(f, "%d", pob.individuos[j][n]);
                    if (n < num_nodos - 1) fprintf(f, "-");
                }
                fprintf(f, " %d\n", evaluar(pob.individuos[j], MCostes));
            }
            fclose(f);
        }
    }

    free(madre);
    free(padre);

    if (fMut) fclose(fMut);
    liberar_poblacion(&pob);

    return MejorSolucion;
}

int *AlgSec_DEF(int ngens, int TPoblacion, grafo *MCostes)
{
    return AlgSec(ngens, TPoblacion, MCostes, 0, 0);
}
