#include "pruebas.h"
#include "grafo.h"
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

    // Abrimos el fichero. 
    // "a" (append) añade al final. Usa "w" (write) si quieres borrar el fichero cada vez.
    FILE *f = fopen("resultados.txt", "a");
    if (f == NULL)
    {
        printf("Error al abrir el fichero resultados.txt\n");
        exit(1);
    }

    tiempo_inicio = omp_get_wtime();
    
    for(int i = 0; i < NUM_PRUEBAS; i++)
    {
        MejorSolucion = algGen_CHamiltoniano(nGeneraciones, tPoblacion, g);

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