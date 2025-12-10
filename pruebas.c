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

    // Abrimos el fichero.
    // "a" (append) añade al final. Usa "w" (write) si quieres borrar el fichero cada vez.
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

        // IMPORTANTE: Liberar memoria en cada iteración para evitar fugas
        free(MejorSolucion);
    }

    tiempo_fin = omp_get_wtime();

    double tiempo_total = (tiempo_fin - tiempo_inicio) / NUM_PRUEBAS;
    double errorPromedio = (double)errorAcumulado / NUM_PRUEBAS;

    // Guardar en fichero con el formato: 1 tiempo error
    // %.6f define la precisión de los decimales (puedes cambiarlo si quieres)
    fprintf(f, "1 %f %f\n", tiempo_total, errorPromedio);

    // Cerrar el fichero para guardar los cambios
    fclose(f);

    // Opcional: Avisar por pantalla que ha terminado
    printf("Prueba secuencial finalizada. Resultados guardados en resultados.txt\n");
}

void pruebaSecuencialSpike(int tPoblacion, int nGeneraciones, grafo *g, int costeOptimo)
{
    double tiempo_inicio, tiempo_fin;
    int costeEncontrado;
    int error;
    int errorAcumulado = 0;
    int *MejorSolucion;
    
    // 1. Declaramos un buffer para guardar el nombre del fichero
    char nombreFichero[50]; 

    for (int neurona = 0; neurona < 8; neurona++)
    {
        tiempo_inicio = omp_get_wtime();
        errorAcumulado = 0;
        
        // Ejecutamos las pruebas
        for (int i = 0; i < NUM_PRUEBAS; i++)
        {
            //ESTO AUN HAY QUE CAMBIARLO A LA QUE TIENE EL SPIKE REAL
            MejorSolucion = AlgSecNeurona(nGeneraciones, tPoblacion, g, neurona);

            // Calculo error
            costeEncontrado = evaluar(MejorSolucion, g);
            error = abs(costeEncontrado - costeOptimo);
            errorAcumulado += error;

            free(MejorSolucion);
        }
        tiempo_fin = omp_get_wtime();
        
        double tiempo_total = (tiempo_fin - tiempo_inicio) / NUM_PRUEBAS;
        double errorPromedio = (double)errorAcumulado / NUM_PRUEBAS;

        // --- ZONA DE ESCRITURA EN FICHERO (Movida al final por seguridad) ---

        // 2. Construimos el nombre del fichero dinámicamente
        // %d se sustituye por el valor de 'neurona'
        sprintf(nombreFichero, "resultadosSpike%d.txt", neurona);

        // 3. Abrimos el fichero usando la variable nombreFichero
        FILE *f = fopen(nombreFichero, "a");
        
        if (f == NULL)
        {
            printf("Error al abrir el fichero %s\n", nombreFichero);
            exit(1);
        }

        fprintf(f, "1 %f %f\n", tiempo_total, errorPromedio);
        fclose(f);
    }

    printf("Prueba secuencial finalizada.\n");
}
