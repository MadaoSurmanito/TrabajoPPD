#include <stdio.h>
#include <stdlib.h>

#include "poblacion.h"

poblacion crear_poblacion(int num_individuos, int num_nodos) {

    poblacion ret;

    ret.num_individuos = num_individuos;
    ret.num_nodos = num_nodos;

    ret.individuos = (int**)malloc(num_individuos * sizeof(int*));
    for(int i = 0; i < num_individuos; ++i) {
        ret.individuos[i] = (int*)malloc(num_nodos * sizeof(int));
        for(int j = 0; j < num_nodos; ++j)
            ret.individuos[i][j] = rand()%num_nodos;
    }

    return ret;
}

void liberar_poblacion(poblacion* p) {
    for(int i = 0; i < p->num_individuos; ++i)
        free(p->individuos[i]); 
    free(p->individuos);
}

void intercambiar_poblaciones(poblacion* a, poblacion* b) {
    int** tmp = b->individuos;
    b->individuos = a->individuos;
    a->individuos = tmp;
}