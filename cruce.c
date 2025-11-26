#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "cruce.h"

// Implementación del cruce PMX (Partially Mapped Crossover)
//      padre1: primer padre
//      padre2: segundo padre
//      hijo: hijo resultante
//      n: tamaño de los arreglos
//      c1, c2: puntos de cruce
void pmx(int p1[], int p2[], int hijo[], int n, int c1, int c2) {
    int i;

    // Inicializar hijo con -1
    for (i = 0; i < n; i++)
        hijo[i] = -1;

    // Copiar la parte fija del padre1
    for (i = c1; i <= c2; i++)
        hijo[i] = p1[i];

    // Para cada posición del segmento en padre2
    for (i = c1; i <= c2; i++) {
        int val = p2[i];

        // Si el valor ya está en el hijo, saltamos
        int existe = 0;
        for (int k = c1; k <= c2; k++) {
            if (hijo[k] == val) {
                existe = 1;
                break;
            }
        }
        if (existe) continue;

        // Buscar dónde debería ir: posición del val en p1
        int pos = i;
        int target = val;

        while (hijo[pos] != -1) {
            // Buscamos en p1 donde está el elemento de p2[pos]
            int elem = p1[pos];
            // Encontramos la posición de elem en p2
            for (int j = 0; j < n; j++) {
                if (p2[j] == elem) {
                    pos = j;
                    break;
                }
            }
        }

        hijo[pos] = val;
    }

    // Relleno final con padre2
    for (i = 0; i < n; i++)
        if (hijo[i] == -1)
            hijo[i] = p2[i];
}



// Genera un hijo usando PMX
int* cruce(int padre[], int madre[], grafo* MCostes) {
    int n = MCostes->num_nodos;
    int* hijo = (int*)malloc(n * sizeof(int));
    if (!hijo) {
        perror("Error al asignar memoria para el hijo");
        exit(1);
    }

    // Seleccionar dos puntos de cruce aleatorios
    int c1 = rand() % n;
    int c2 = rand() % n;
    if (c1 > c2) {
        int temp = c1;
        c1 = c2;
        c2 = temp;
    }

    pmx(padre, madre, hijo, n, c1, c2);

    // ---------------------------------------------------------------
    // Trabajo futuro, decidir que hacer si un hijo es igual a un padre
    // ---------------------------------------------------------------

    return hijo;
}