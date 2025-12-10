#include <stdlib.h>
#include "mutacion.h"
#include "seeds.h"
// HAY QUE HACER EL SRAND(TIME(NULL)) EN EL MAIN

void mutacion(int *individuo, int tamaño)
{
    int temp;

    // Cambio de dos posiciones aleatorias
    int pos1 = RAND() % tamaño;
    int pos2 = RAND() % tamaño;
    // Asegurarse de que las posiciones sean diferentes
    while (pos1 == pos2)
    {
        pos2 = RAND() % tamaño;
    }
    // Intercambio de los valores en las posiciones seleccionadas
    temp = individuo[pos1];
    individuo[pos1] = individuo[pos2];
    individuo[pos2] = temp;
}