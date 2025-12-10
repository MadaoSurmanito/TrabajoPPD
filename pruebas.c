#include "pruebas.h"
#include "grafo.h"
//#include "neurona.h"
#include "evaluar.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "algSec.h"

#define NUM_PRUEBAS 30

void pruebaSecuencial(int tPoblacion, int nGeneraciones, grafo *g, int costeOptimo)
{
    double tiempo_inicio, tiempo_fin;
    int costeEncontrado;
    int error;
    int errorAcumulado = 0;
    int *MejorSolucion;

    tiempo_inicio = omp_get_wtime();
    for(int i = 0; i < NUM_PRUEBAS; i++)
    {
        MejorSolucion = algGen_CHamiltoniano(nGeneraciones, tPoblacion, g);

        //Calculo error
        costeEncontrado = evaluar(MejorSolucion, g);
        error = abs(costeEncontrado - costeOptimo);
        errorAcumulado += error;
    }
    tiempo_fin = omp_get_wtime();

    double tiempo_total = (tiempo_fin - tiempo_inicio) / NUM_PRUEBAS;
    double errorPromedio = (double)errorAcumulado / NUM_PRUEBAS;

    // Resultados
    printf("Tiempo secuencial promedio para %d pruebas: %f segundos\n", NUM_PRUEBAS, tiempo_total);
    printf("Error promedio: %f\n", errorPromedio);
}

/*void pruebaNeuronaParalela(int tPoblacion, int nGeneraciones, grafo *g, Neurona *neurona)
{
    int tiempo_inicio, tiempo_fin;
    tiempo_inicio = omp_get_wtime();
}*/