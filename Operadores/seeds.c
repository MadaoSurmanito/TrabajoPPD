#include "seeds.h"
#include <stdio.h>
#include <time.h>

unsigned int nthreads = 0;
unsigned int *seeds = NULL;

void init_seeds_num_threads(unsigned int num_hilos)
{
    if (seeds)
        free(seeds);

    nthreads = num_hilos;
    seeds = malloc(sizeof(unsigned int) * nthreads);
    if (!seeds) {
        perror("malloc seeds");
        exit(EXIT_FAILURE);
    }

    unsigned int base = (unsigned int)time(NULL);
    for (unsigned int i = 0; i < nthreads; i++)
        seeds[i] = base ^ (i + 1);

    srand(base); // para el caso secuencial
}
