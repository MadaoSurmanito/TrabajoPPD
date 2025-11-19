#include "evaluar.h"
#include "grafo.h"

int evaluar(int* individuo, grafo* g) {
    int ret = 0;
    for(int i = 0; i < g->num_nodos - 1; ++i) {
        int coste = g->coste[individuo[i]][individuo[i+1]];
        if(coste < 0)
            return -1;
        ret += coste;
    }
    return ret;
}