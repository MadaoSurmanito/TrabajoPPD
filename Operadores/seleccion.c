#include "seleccion.h"

#define PROB_MEJOR 70

int seleccion(poblacion *p, grafo *g, int *individuo, int costeIndividuo)
{
    if (costeIndividuo == -1) return 0;

    int indice = rand() % p->num_individuos;
    int *indPob = p->individuos[indice];

    int costePob = evaluar(indPob, g);
    int prob = rand() % 100;

    if (prob < PROB_MEJOR)
    {
        if (costePob == -1 || costeIndividuo < costePob)
        {
            memcpy(indPob, individuo, g->num_nodos * sizeof(int));
            return 1;
        }
    }
    else
    {
        memcpy(indPob, individuo, g->num_nodos * sizeof(int));
        return 1;
    }

    return 0;
}
