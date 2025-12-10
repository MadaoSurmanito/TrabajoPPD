#include <stdio.h>
#include <stdlib.h>

#include "poblacion.h"

poblacion crear_poblacion(int num_individuos, int num_nodos)
{
poblacion ret;
    ret.num_individuos = num_individuos;
    ret.num_nodos = num_nodos;

    ret.individuos = (int **)malloc(num_individuos * sizeof(int *));

    for (int i = 0; i < num_individuos; ++i)
    {
        ret.individuos[i] = (int *)malloc(num_nodos * sizeof(int));

        // Inicializar como 0..n-1
        for (int j = 0; j < num_nodos; ++j)
            ret.individuos[i][j] = j;

        for (int j = num_nodos - 1; j > 0; --j)
        {
            int k = rand() % (j + 1);

            int tmp = ret.individuos[i][j];
            ret.individuos[i][j] = ret.individuos[i][k];
            ret.individuos[i][k] = tmp;
        }
    }

    return ret;
}

void liberar_poblacion(poblacion *p)
{
    for (int i = 0; i < p->num_individuos; ++i)
        free(p->individuos[i]);
    free(p->individuos);
}

void intercambiar_poblaciones(poblacion *a, poblacion *b)
{
    poblacion tmp = *a;
    *a = *b;
    *b = tmp;
}
