#include "evaluarPob.h"

void evaluarPob(poblacion* pob, int TPoblacion, grafo* MCostes,
                float* mejorfPob, float* promfPob, float* peorfPob)
{
    float actualf;

    *mejorfPob = 0;
    *peorfPob  = 1e9;
    *promfPob  = 0;

    for (int i = 0; i < TPoblacion; i++)
    {
        actualf = (float)evaluar(pob->individuos[i], MCostes);

        *promfPob += actualf;

        if (actualf > *mejorfPob)
            *mejorfPob = actualf;

        if (actualf < *peorfPob)
            *peorfPob = actualf;
    }

    *promfPob /= TPoblacion;
}
