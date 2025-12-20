#include <stdlib.h>
#include <string.h>
#include "evaluar.h"

int evaluar(int *individuo, grafo* g)
{

    unsigned char *repetidos = malloc(g->num_nodos * sizeof(unsigned char));
    memset(repetidos, 0, g->num_nodos);

    int ret = 0;

    for (int i = 0; i < g->num_nodos; ++i)
    {
        // Se comprueba si ya se ha pasado por ese nodo
        if (repetidos[individuo[i]]){
            free(repetidos);
            return -1;
        }

        repetidos[individuo[i]] = 1;
        // Se obtiene el coste de ir de un nodo al siguiente en el camino
        
        int coste = g->coste[individuo[i]][individuo[(i + 1) % g->num_nodos]];
        if (coste < 0) {
            free(repetidos);
            return -1;
        }

        ret += coste;
    }

    free(repetidos);

    return ret;
}
