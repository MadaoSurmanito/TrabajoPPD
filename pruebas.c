#include "pruebas.h"
#include "Estructuras/grafo.h"
#include "Operadores/evaluar.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include "Alg.h"

#define NUM_PRUEBAS 30

void pruebaSecuencial(int tPoblacion, int nGeneraciones, grafo *g, int costeOptimo)
{
    double tiempo_inicio, tiempo_fin;
    int costeEncontrado;
    int error;
    int errorAcumulado = 0;
    int *MejorSolucion;

    FILE *f = fopen("resultados.txt", "a");
    if (f == NULL)
    {
        printf("Error al abrir el fichero resultados.txt\n");
        exit(1);
    }

    tiempo_inicio = omp_get_wtime();

    for (int i = 0; i < NUM_PRUEBAS; i++)
    {
        MejorSolucion = AlgSec(nGeneraciones, tPoblacion, g);

        // Calculo error
        costeEncontrado = evaluar(MejorSolucion, g);
        error = abs(costeEncontrado - costeOptimo);
        errorAcumulado += error;

        free(MejorSolucion);
    }

    tiempo_fin = omp_get_wtime();

    double tiempo_total = (tiempo_fin - tiempo_inicio) / NUM_PRUEBAS;
    double errorPromedio = (double)errorAcumulado / NUM_PRUEBAS;

    fprintf(f, "0 %f %f\n", tiempo_total, errorPromedio);

    fclose(f);
}

void pruebaSecuencialSpike(int tPoblacion, int nGeneraciones, grafo *g, int costeOptimo)
{
    double tiempo_inicio, tiempo_fin;
    int costeEncontrado;
    int error;
    int errorAcumulado = 0;
    int *MejorSolucion;

    char nombreFichero[50];

    for (int neurona = 1; neurona <= 8; neurona++)
    {
        tiempo_inicio = omp_get_wtime();
        errorAcumulado = 0;

        for (int i = 0; i < NUM_PRUEBAS; i++)
        {
            MejorSolucion = AlgSecNeurona(nGeneraciones, tPoblacion, g, neurona);

            costeEncontrado = evaluar(MejorSolucion, g);
            error = abs(costeEncontrado - costeOptimo);
            errorAcumulado += error;

            free(MejorSolucion);
        }
        tiempo_fin = omp_get_wtime();

        double tiempo_total = (tiempo_fin - tiempo_inicio) / NUM_PRUEBAS;
        double errorPromedio = (double)errorAcumulado / NUM_PRUEBAS;

        sprintf(nombreFichero, "resultadosSpike%d.txt", neurona);

        FILE *f = fopen(nombreFichero, "a");

        if (f == NULL)
        {
            printf("Error al abrir el fichero %s\n", nombreFichero);
            exit(1);
        }

        fprintf(f, "0 %f %f\n", tiempo_total, errorPromedio);
        fclose(f);
    }
}

void pruebaParalela(int tPoblacion, int nGeneraciones, grafo *g, int costeOptimo)
{
    double tiempo_inicio, tiempo_fin;
    int costeEncontrado;
    int error;
    int errorAcumulado;
    int *MejorSolucion;

    FILE *f = fopen("resultados.txt", "a");
    if (f == NULL)
    {
        printf("Error al abrir el fichero resultados.txt\n");
        exit(1);
    }

    int num_hilos = 1;
    errorAcumulado = 0;

    tiempo_inicio = omp_get_wtime();

    for (int i = 0; i < NUM_PRUEBAS; i++)
    {
        MejorSolucion = AlgPall(nGeneraciones, tPoblacion, g, num_hilos);

        // Calcular error
        costeEncontrado = evaluar(MejorSolucion, g);
        error = abs(costeEncontrado - costeOptimo);
        errorAcumulado += error;

        free(MejorSolucion);
    }

    tiempo_fin = omp_get_wtime();

    double tiempo_total = (tiempo_fin - tiempo_inicio) / NUM_PRUEBAS;
    double errorPromedio = (double)errorAcumulado / NUM_PRUEBAS;

    fprintf(f, "%d %f %f\n", num_hilos, tiempo_total, errorPromedio);

    for (int num_hilos = 2; num_hilos <= 12; num_hilos += 2)
    {
        errorAcumulado = 0;

        tiempo_inicio = omp_get_wtime();

        for (int i = 0; i < NUM_PRUEBAS; i++)
        {
            MejorSolucion = AlgPall(nGeneraciones, tPoblacion, g, num_hilos);

            // Calcular error
            costeEncontrado = evaluar(MejorSolucion, g);
            error = abs(costeEncontrado - costeOptimo);
            errorAcumulado += error;

            free(MejorSolucion);
        }

        tiempo_fin = omp_get_wtime();

        double tiempo_total = (tiempo_fin - tiempo_inicio) / NUM_PRUEBAS;
        double errorPromedio = (double)errorAcumulado / NUM_PRUEBAS;

        fprintf(f, "%d %f %f\n", num_hilos, tiempo_total, errorPromedio);
    }

    fclose(f);
}

void pruebaParalelaSpike(int tPoblacion, int nGeneraciones, grafo *g, int costeOptimo)
{
    double tiempo_inicio, tiempo_fin;
    int costeEncontrado;
    int error;
    int errorAcumulado;
    int *MejorSolucion;

    FILE *f;
    char nombreFichero[50];

    int num_hilos = 1;
    for (int neurona = 1; neurona <= 8; neurona++)
    {
        errorAcumulado = 0;

        tiempo_inicio = omp_get_wtime();

        for (int i = 0; i < NUM_PRUEBAS; i++)
        {
            // Llamamos a tu AlgPall con neurona y número de hilos
            MejorSolucion = AlgPallNeurona(nGeneraciones, tPoblacion, g, num_hilos, neurona);

            // Calcular error
            costeEncontrado = evaluar(MejorSolucion, g);
            error = abs(costeEncontrado - costeOptimo);
            errorAcumulado += error;

            free(MejorSolucion);
        }

        tiempo_fin = omp_get_wtime();

        double tiempo_total = (tiempo_fin - tiempo_inicio) / NUM_PRUEBAS;
        double errorPromedio = (double)errorAcumulado / NUM_PRUEBAS;

        sprintf(nombreFichero, "resultadosSpike%d.txt", neurona);
        f = fopen(nombreFichero, "a");
        if (!f)
        {
            printf("Error al abrir el fichero %s\n", nombreFichero);
            exit(1);
        }

        fprintf(f, "%d %f %f\n", num_hilos, tiempo_total, errorPromedio);
        fclose(f);
    }

    // Probamos con hilos de 2 en 2
    for (int num_hilos = 2; num_hilos <= 12; num_hilos += 2)
    {
        for (int neurona = 1; neurona <= 8; neurona++)
        {
            errorAcumulado = 0;

            tiempo_inicio = omp_get_wtime();

            for (int i = 0; i < NUM_PRUEBAS; i++)
            {
                MejorSolucion = AlgPallNeurona(nGeneraciones, tPoblacion, g, num_hilos, neurona);

                // Calcular error
                costeEncontrado = evaluar(MejorSolucion, g);
                error = abs(costeEncontrado - costeOptimo);
                errorAcumulado += error;

                free(MejorSolucion);
            }

            tiempo_fin = omp_get_wtime();

            double tiempo_total = (tiempo_fin - tiempo_inicio) / NUM_PRUEBAS;
            double errorPromedio = (double)errorAcumulado / NUM_PRUEBAS;

            sprintf(nombreFichero, "resultadosSpike%d.txt", neurona);
            f = fopen(nombreFichero, "a");
            if (!f)
            {
                printf("Error al abrir el fichero %s\n", nombreFichero);
                exit(1);
            }

            fprintf(f, "%d %f %f\n", num_hilos, tiempo_total, errorPromedio);
            fclose(f);
        }
    }
}
