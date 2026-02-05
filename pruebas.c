#include "pruebas.h"

#define NUM_PRUEBAS 30

// =================== PRUEBAS SECUENCIALES ===================

void pruebaSecuencial(int tPoblacion, int nGeneraciones, grafo *g)
{
    double tiempo_inicio, tiempo_fin;

    mkdir("Analisis", 0777);
    mkdir("Analisis/Datos", 0777);
    mkdir("Analisis/Datos/Secuencial", 0777);
    mkdir("Analisis/Datos/Secuencial/SinNeurona", 0777);
    mkdir("Analisis/Datos/Secuencial/SinNeurona/Tiempos", 0777);

    printf("=== COMIENZO de Pruebas Secuencial ===\n");
    FILE *f = fopen("Analisis/Datos/Secuencial/SinNeurona/Tiempos/Tiempos.txt", "w");
    if (!f) { perror("Error al abrir resultados.txt"); exit(1); }

    tiempo_inicio = omp_get_wtime();

    for (int i = 0; i < NUM_PRUEBAS; i++)
        AlgSec_DEF(nGeneraciones, tPoblacion, g);

    tiempo_fin = omp_get_wtime();
    double tiempo_total = (tiempo_fin - tiempo_inicio) / NUM_PRUEBAS;

    fprintf(f, "0 %f\n", tiempo_total);
    fclose(f);

    printf("0\tSin Neurona\t%.6f\n", tiempo_total);
    printf("=== FIN de Pruebas Secuencial ===\n\n");
}

void pruebaSecuencialSpike(int tPoblacion, int nGeneraciones, grafo *g)
{
    double tiempo_inicio, tiempo_fin;

    printf("=== COMIENZO de Pruebas Secuencial Neurona ===\n");

    mkdir("Analisis/Datos/Secuencial", 0777);

    const char *nombreNeurona[] =
        {"RS","IB","CH","FS","TC1","TC2","RZ","LTS"};

    for (int neurona = 1; neurona <= 8; neurona++)
    {
        char base[256], dirT[256], fichero[256];

        snprintf(base, sizeof(base),
                 "Analisis/Datos/Secuencial/Neurona_%s",
                 nombreNeurona[neurona-1]);
        snprintf(dirT, sizeof(dirT), "%s/Tiempos", base);
        snprintf(fichero, sizeof(fichero), "%s/Tiempos.txt", dirT);

        mkdir(base, 0777);
        mkdir(dirT, 0777);

        tiempo_inicio = omp_get_wtime();

        for (int i = 0; i < NUM_PRUEBAS; i++)
            AlgSecNeurona_DEF(nGeneraciones, tPoblacion, g, neurona);

        tiempo_fin = omp_get_wtime();
        double tiempo_total = (tiempo_fin - tiempo_inicio) / NUM_PRUEBAS;

        FILE *f = fopen(fichero, "w");
        if (!f) { perror(fichero); exit(1); }
        fprintf(f, "0 %f\n", tiempo_total);
        fclose(f);

        printf("0\t%s\t%.6f\n",
               nombreNeurona[neurona-1], tiempo_total);
    }

    printf("=== FIN de Pruebas Secuencial Neurona ===\n\n");
}

// =================== PRUEBAS PARALELAS ===================

void pruebaParalela(int tPoblacion, int nGeneraciones, grafo *g)
{
    double tiempo_inicio, tiempo_fin;

    printf("=== COMIENZO de Pruebas Paralelas ===\n");

    mkdir("Analisis/Datos/Paralelo", 0777);
    mkdir("Analisis/Datos/Paralelo/SinNeurona", 0777);
    mkdir("Analisis/Datos/Paralelo/SinNeurona/Tiempos", 0777);

    FILE *f = fopen(
        "Analisis/Datos/Paralelo/SinNeurona/Tiempos/Tiempos.txt", "w");
    if (!f) { perror("Tiempos.txt"); exit(1); }

    for (int num_hilos = 1; num_hilos <= 12;
         num_hilos += (num_hilos == 1 ? 1 : 2))
    {
        tiempo_inicio = omp_get_wtime();

        for (int i = 0; i < NUM_PRUEBAS; i++)
            AlgPall_DEF(nGeneraciones, tPoblacion, g, num_hilos);

        tiempo_fin = omp_get_wtime();
        double tiempo_total = (tiempo_fin - tiempo_inicio) / NUM_PRUEBAS;

        fprintf(f, "%d %f\n", num_hilos, tiempo_total);
        printf("%d\tSin Neurona\t%.6f\n",
               num_hilos, tiempo_total);
    }

    fclose(f);
    printf("=== FIN de Pruebas Paralelas ===\n\n");
}

void pruebaParalelaSpike(int tPoblacion, int nGeneraciones, grafo *g)
{
    double tiempo_inicio, tiempo_fin;

    printf("=== COMIENZO de Pruebas Paralelas Neurona ===\n");

    const char *nombreNeurona[] =
        {"RS","IB","CH","FS","TC1","TC2","RZ","LTS"};

    mkdir("Analisis/Datos/Paralelo", 0777);

    for (int neurona = 1; neurona <= 8; neurona++)
    {
        char base[256], dirT[256], fichero[256];

        snprintf(base, sizeof(base),
                 "Analisis/Datos/Paralelo/Neurona_%s",
                 nombreNeurona[neurona-1]);
        snprintf(dirT, sizeof(dirT), "%s/Tiempos", base);
        snprintf(fichero, sizeof(fichero), "%s/Tiempos.txt", dirT);

        mkdir(base, 0777);
        mkdir(dirT, 0777);

        FILE *f = fopen(fichero, "w");
        if (!f) { perror(fichero); exit(1); }

        for (int num_hilos = 1; num_hilos <= 12;
             num_hilos += (num_hilos == 1 ? 1 : 2))
        {
            tiempo_inicio = omp_get_wtime();

            for (int i = 0; i < NUM_PRUEBAS; i++)
                AlgPallNeurona_DEF(nGeneraciones, tPoblacion,
                                   g, num_hilos, neurona);

            tiempo_fin = omp_get_wtime();
            double tiempo_total =
                (tiempo_fin - tiempo_inicio) / NUM_PRUEBAS;

            fprintf(f, "%d %f\n", num_hilos, tiempo_total);
            printf("%d\t%s\t%.6f\n",
                   num_hilos,
                   nombreNeurona[neurona-1],
                   tiempo_total);
        }

        fclose(f);
    }

    printf("=== FIN de Pruebas Paralelas Neurona ===\n\n");
}
