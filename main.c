#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include "grafo.h"

int main() {

    grafo g = crear_grafo(4);

    imprimir_grafo(&g);

    liberar_grafo(&g);

    return 0;
}