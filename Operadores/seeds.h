#ifndef _SEEDS_H
#define _SEEDS_H

#include <omp.h>
#include <stdlib.h>

extern unsigned int nthreads;
extern unsigned int *seeds;

static inline int RAND() {
#ifdef _OPENMP
    if (omp_in_parallel()) {
        int tid = omp_get_thread_num();
        return rand_r(&seeds[tid]);
    } else
#endif
    {
        return rand();
    }
}

void init_seeds();

#endif
