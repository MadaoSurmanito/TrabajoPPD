#include "pruebas.h"

#define NUM_PRUEBAS 30

void pruebaSecuencial(int tPoblacion, int nGeneraciones, grafo *g, int costeOptimo)
{
    double tiempo_inicio, tiempo_fin;
    int costeEncontrado, error, errorAcumulado = 0;
    int *MejorSolucion;

    printf("=== COMIENZO de Pruebas Secuencial ===\n");
    FILE *f = fopen("Analisis/Datos/resultados.txt", "a");
    if (!f) { perror("Error al abrir resultados.txt"); exit(1); }

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

    fprintf(f, "0 %f %f\n", tiempo_total, errorPromedio);
    fclose(f);

    printf("0\tSin Neurona\t%.6f\t%.6f\n", tiempo_total, errorPromedio);
    printf("=== FIN de Pruebas Secuencial ===\n\n");
}

void pruebaSecuencialSpike(int tPoblacion, int nGeneraciones, grafo *g, int costeOptimo)
{
    double tiempo_inicio, tiempo_fin;
    int costeEncontrado, error, errorAcumulado;
    int *MejorSolucion;
    char nombreFichero[50];

    printf("=== COMIENZO de Pruebas Secuencial Neurona ===\n");

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

        sprintf(nombreFichero, "Analisis/Datos/resultadosSpike%d.txt", neurona);
        FILE *f = fopen(nombreFichero, "a");
        if (!f) { perror(nombreFichero); exit(1); }
        fprintf(f, "0 %f %f\n", tiempo_total, errorPromedio);
        fclose(f);

        printf("0\t%d\t%.6f\t%.6f\n", neurona, tiempo_total, errorPromedio);
    }

    printf("=== FIN de Pruebas Secuencial Neurona ===\n\n");
}

void pruebaParalela(int tPoblacion, int nGeneraciones, grafo *g, int costeOptimo)
{
    double tiempo_inicio, tiempo_fin;
    int costeEncontrado, error, errorAcumulado;
    int *MejorSolucion;

    printf("=== COMIENZO de Pruebas Paralelas ===\n");

    FILE *f = fopen("Analisis/Datos/resultados.txt", "a");
    if (!f) { perror("resultados.txt"); exit(1); }

    for (int num_hilos = 1; num_hilos <= 12; num_hilos += (num_hilos == 1 ? 1 : 2))
    {
        errorAcumulado = 0;
        tiempo_inicio = omp_get_wtime();

        for (int i = 0; i < NUM_PRUEBAS; i++)
        {
            MejorSolucion = AlgPall(nGeneraciones, tPoblacion, g, num_hilos);

            costeEncontrado = evaluar(MejorSolucion, g);
            error = abs(costeEncontrado - costeOptimo);
            errorAcumulado += error;

            free(MejorSolucion);
        }

        tiempo_fin = omp_get_wtime();
        double tiempo_total = (tiempo_fin - tiempo_inicio) / NUM_PRUEBAS;
        double errorPromedio = (double)errorAcumulado / NUM_PRUEBAS;

        fprintf(f, "%d %f %f\n", num_hilos, tiempo_total, errorPromedio);
        printf("%d\tSin Neurona\t%.6f\t%.6f\n", num_hilos, tiempo_total, errorPromedio);
    }

    fclose(f);
    printf("=== FIN de Pruebas Paralelas ===\n\n");
}

void pruebaParalelaSpike(int tPoblacion, int nGeneraciones, grafo *g, int costeOptimo)
{
    double tiempo_inicio, tiempo_fin;
    int costeEncontrado, error, errorAcumulado;
    int *MejorSolucion;
    char nombreFichero[50];

    printf("=== COMIENZO de Pruebas Paralelas Neurona ===\n");

    for (int neurona = 1; neurona <= 8; neurona++)
    {
        for (int num_hilos = 1; num_hilos <= 12; num_hilos += (num_hilos == 1 ? 1 : 2))
        {
            errorAcumulado = 0;
            tiempo_inicio = omp_get_wtime();

            for (int i = 0; i < NUM_PRUEBAS; i++)
            {
                MejorSolucion = AlgPallNeurona(nGeneraciones, tPoblacion, g, num_hilos, neurona);

                costeEncontrado = evaluar(MejorSolucion, g);
                error = abs(costeEncontrado - costeOptimo);
                errorAcumulado += error;

                free(MejorSolucion);
            }

            tiempo_fin = omp_get_wtime();
            double tiempo_total = (tiempo_fin - tiempo_inicio) / NUM_PRUEBAS;
            double errorPromedio = (double)errorAcumulado / NUM_PRUEBAS;

            sprintf(nombreFichero, "Analisis/Datos/resultadosSpike%d.txt", neurona);
            FILE *f = fopen(nombreFichero, "a");
            if (!f) { perror(nombreFichero); exit(1); }
            fprintf(f, "%d %f %f\n", num_hilos, tiempo_total, errorPromedio);
            fclose(f);

            printf("%d\t%d\t%.6f\t%.6f\n", num_hilos, neurona, tiempo_total, errorPromedio);
        }
    }

    printf("=== FIN de Pruebas Paralelas Neurona ===\n\n");
}
