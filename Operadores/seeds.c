#include "seeds.h"
#include <stdio.h>
#include <time.h>

unsigned int nthreads = 0;
unsigned int *seeds = NULL;

void init_seeds() {
    nthreads = omp_get_max_threads();
    seeds = malloc(sizeof(unsigned int) * nthreads);
    if (!seeds) {
        perror("malloc seeds");
        exit(EXIT_FAILURE);
    }

    for (unsigned int i = 0; i < nthreads; i++) {
        seeds[i] = (unsigned int)time(NULL) + i;
    }
}
