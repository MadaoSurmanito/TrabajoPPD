#include "pruebas.h"
#include "Estructuras/grafo.h"
#include "Operadores/evaluar.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include "Alg.h"

#define NUM_PRUEBAS 30

// ======================= PRUEBAS SECUENCIALES =======================
void pruebaSecuencial(int tPoblacion, int nGeneraciones, grafo *g, int costeOptimo)
{
    double tiempo_inicio, tiempo_fin;
    int costeEncontrado, error;
    int errorAcumulado = 0;
    int *MejorSolucion;

    FILE *f = fopen("resultados.txt", "a");
    if (!f) { printf("Error al abrir resultados.txt\n"); exit(1); }

    printf("\n=== Iniciando pruebas secuenciales ===\n");

    tiempo_inicio = omp_get_wtime();

    for (int i = 0; i < NUM_PRUEBAS; i++)
    {
        MejorSolucion = AlgSec(nGeneraciones, tPoblacion, g);

        costeEncontrado = evaluar(MejorSolucion, g);
        error = abs(costeEncontrado - costeOptimo);
        errorAcumulado += error;

        free(MejorSolucion);
    }

    tiempo_fin = omp_get_wtime();
    double tiempo_total = (tiempo_fin - tiempo_inicio) / NUM_PRUEBAS;
    double errorPromedio = (double)errorAcumulado / NUM_PRUEBAS;

    fprintf(f, "1 %f %f\n", tiempo_total, errorPromedio);
    fclose(f);

    printf("Neuron None | Secuencial | Tiempo medio: %.6f | Error medio: %.2f\n", tiempo_total, errorPromedio);
}

// ======================= PRUEBAS SECUENCIALES SPIKE =======================
void pruebaSecuencialSpike(int tPoblacion, int nGeneraciones, grafo *g, int costeOptimo)
{
    printf("\n=== Iniciando pruebas secuenciales Spike ===\n");

    for (int neurona = 1; neurona <= 8; neurona++)
    {
        int errorAcumulado = 0;
        double tiempo_inicio = omp_get_wtime();
        char nombreFichero[50];

        for (int i = 0; i < NUM_PRUEBAS; i++)
        {
            int *MejorSolucion = AlgSecNeurona(nGeneraciones, tPoblacion, g, neurona);

            int costeEncontrado = evaluar(MejorSolucion, g);
            int error = abs(costeEncontrado - costeOptimo);
            errorAcumulado += error;

            free(MejorSolucion);
        }

        double tiempo_fin = omp_get_wtime();
        double tiempo_total = (tiempo_fin - tiempo_inicio) / NUM_PRUEBAS;
        double errorPromedio = (double)errorAcumulado / NUM_PRUEBAS;

        sprintf(nombreFichero, "resultadosSpike%d.txt", neurona);
        FILE *f = fopen(nombreFichero, "a");
        if (!f) { printf("Error al abrir %s\n", nombreFichero); exit(1); }
        fprintf(f, "1 %f %f\n", tiempo_total, errorPromedio);
        fclose(f);

        printf("Neuron %d | Secuencial | Tiempo medio: %.6f | Error medio: %.2f\n",
               neurona, tiempo_total, errorPromedio);
    }
}

// ======================= PRUEBAS PARALELAS =======================
void pruebaParalela(int tPoblacion, int nGeneraciones, grafo *g, int costeOptimo)
{
    printf("\n=== Iniciando pruebas paralelas ===\n");

    FILE *f = fopen("resultados.txt", "a");
    if (!f) { printf("Error al abrir resultados.txt\n"); exit(1); }

    for (int num_hilos = 2; num_hilos <= 12; num_hilos += 2)
    {
        int errorAcumulado = 0;
        double tiempo_inicio = omp_get_wtime();

        for (int i = 0; i < NUM_PRUEBAS; i++)
        {
            int *MejorSolucion = AlgPall(nGeneraciones, tPoblacion, g, num_hilos);

            int costeEncontrado = evaluar(MejorSolucion, g);
            int error = abs(costeEncontrado - costeOptimo);
            errorAcumulado += error;

            free(MejorSolucion);
        }

        double tiempo_fin = omp_get_wtime();
        double tiempo_total = (tiempo_fin - tiempo_inicio) / NUM_PRUEBAS;
        double errorPromedio = (double)errorAcumulado / NUM_PRUEBAS;

        fprintf(f, "%d %f %f\n", num_hilos, tiempo_total, errorPromedio);
        printf("Neuron None | Hilos %d | Tiempo medio: %.6f | Error medio: %.2f\n", num_hilos, tiempo_total, errorPromedio);
    }

    fclose(f);
}

// ======================= PRUEBAS PARALELAS SPIKE =======================
void pruebaParalelaSpike(int tPoblacion, int nGeneraciones, grafo *g, int costeOptimo)
{
    printf("\n=== Iniciando pruebas paralelas Spike ===\n");

    for (int num_hilos = 2; num_hilos <= 12; num_hilos += 2)
    {

        for (int neurona = 1; neurona <= 8; neurona++)
        {
            int errorAcumulado = 0;
            double tiempo_inicio = omp_get_wtime();
            char nombreFichero[50];

            for (int i = 0; i < NUM_PRUEBAS; i++)
            {
                int *MejorSolucion = AlgPallNeurona(nGeneraciones, tPoblacion, g, num_hilos, neurona);

                int costeEncontrado = evaluar(MejorSolucion, g);
                int error = abs(costeEncontrado - costeOptimo);
                errorAcumulado += error;

                free(MejorSolucion);
            }

            double tiempo_fin = omp_get_wtime();
            double tiempo_total = (tiempo_fin - tiempo_inicio) / NUM_PRUEBAS;
            double errorPromedio = (double)errorAcumulado / NUM_PRUEBAS;

            sprintf(nombreFichero, "resultadosSpike%d.txt", neurona);
            FILE *f = fopen(nombreFichero, "a");
            if (!f) { printf("Error al abrir %s\n", nombreFichero); exit(1); }
            fprintf(f, "%d %f %f\n", num_hilos, tiempo_total, errorPromedio);
            fclose(f);

            printf("Neuron %d | Hilos %d | Tiempo medio: %.6f | Error medio: %.2f\n",
                   neurona, num_hilos, tiempo_total, errorPromedio);
        }
    }
}
