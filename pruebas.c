#include "pruebas.h"
#include "grafo.h"
#include "neurona.h"
#include <omp.h>
#include <stdio.h>
#include "algSec.h"

#define NUM_PRUEBAS 30

void pruebaSecuencial(int tPoblacion, int nGeneraciones, grafo *g)
{
    int tiempo_inicio, tiempo_fin;
    tiempo_inicio = omp_get_wtime();
    for(int i = 0; i < NUM_PRUEBAS; i++)
    {
        int *MejorSolucion = algGen_CHamiltoniano(nGeneraciones, tPoblacion, g);
    }
    tiempo_fin = omp_get_wtime();
    printf("Tiempo secuencial promedio para %d pruebas: %f segundos\n", NUM_PRUEBAS, (tiempo_fin - tiempo_inicio) / NUM_PRUEBAS);
}

void pruebaNeuronaParalela(int tPoblacion, int nGeneraciones, grafo *g, Neurona *neurona)
{
    int tiempo_inicio, tiempo_fin;
    tiempo_inicio = omp_get_wtime();
}