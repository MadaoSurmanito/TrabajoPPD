// mainPruebasCruce.c
// Programa de pruebas que genera 2 vectores aleatorios (permutaciones) de 10 elementos
// y aplica el cruce PMX definido abajo.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cruce.h"
#include "grafo.h"

// Funciones auxiliares
void imprimir_array(const char* etiqueta, int a[], int n) {
    printf("%s: [", etiqueta);
    for (int i = 0; i < n; i++) {
        printf("%d", a[i]);
        if (i < n-1) printf(", ");
    }
    printf("]\n");
}

void mezclar(int a[], int n) {
    for (int i = n-1; i > 0; i--) {
        int j = rand() % (i+1);
        int tmp = a[i];
        a[i] = a[j];
        a[j] = tmp;
    }
}

int main(void) {
    srand((unsigned)time(NULL));

    const int n = 10;
    int padre[n], madre[n];

    // Generar permutaciones 0..n-1 y mezclar
    for (int i = 0; i < n; i++) {
        padre[i] = i;
        madre[i] = i;
    }
    mezclar(padre, n);
    mezclar(madre, n);

    imprimir_array("Padre", padre, n);
    imprimir_array("Madre", madre, n);

    grafo MCostes;
    MCostes.num_nodos = n;

    int* hijo = cruce(padre, madre, &MCostes);

    imprimir_array("Hijo (resultado PMX)", hijo, n);

    free(hijo);
    return 0;
}