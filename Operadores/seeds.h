#ifndef _SEEDS_H
#define _SEEDS_H

#include <omp.h>
#include <stdlib.h>

extern unsigned int nthreads;
extern unsigned int *seeds;

/* Inicializa semillas para un número específico de hilos */
void init_seeds_num_threads(unsigned int num_hilos);

/* RAND thread-safe */
static inline int RAND()
{
#ifdef _OPENMP
    if (omp_in_parallel()) {
        int tid = omp_get_thread_num();
        if (seeds && tid < (int)nthreads)
            return rand_r(&seeds[tid]);
    }
#endif
    /* Secuencial o fallback */
    return rand();
}

#endif
