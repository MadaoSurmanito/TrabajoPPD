#ifndef _SEEDS_H
#define _SEEDS_H

#include <omp.h>
#include <stdlib.h>

extern unsigned int nthreads;
extern unsigned int *seeds;

#define RAND() rand_r(seeds + omp_get_thread_num())

#endif