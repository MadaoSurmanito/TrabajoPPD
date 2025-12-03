#include <stdlib.h>
#include <string.h>
#include "evaluar.h"

int evaluar(int *individuo, int **costes, int num_nodos)
{

    unsigned char *repetidos = malloc(num_nodos);
    memset(repetidos, 0, num_nodos);

    int ret = 0;

    for (int i = 0; i < num_nodos; ++i)
    {
        // Se comprueba si ya se ha pasado por ese nodo
        if (repetidos[i])
            return -1;
        repetidos[i] = 1;
        // Se obtiene el coste de ir de un nodo al siguiente en el camino
        
        int coste = costes[individuo[i]][individuo[(i + 1) % num_nodos]];
        if (coste < 0)
            return -1;
        ret += coste;
    }

    return ret;
}