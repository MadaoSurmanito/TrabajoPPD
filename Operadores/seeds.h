#ifndef _SEEDS_H
#define _SEEDS_H

#include <omp.h>
#include <stdlib.h>
#include <string.h>

extern unsigned int nthreads;
extern unsigned int *seeds;

static inline int RAND()
{
#ifdef _OPENMP
    int tid = 0;

    if (omp_in_parallel())
        tid = omp_get_thread_num();

    if (seeds && tid < (int)nthreads)
        return rand_r(&seeds[tid]);
#endif

    return rand();
}

void init_seeds(void);

#endif
