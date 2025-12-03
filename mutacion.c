#include <stdlib.h>

// HAY QUE HACER EL SRAND(TIME(NULL)) EN EL MAIN

void mutacion(int *individuo, int tamaño, int PROBABILIDAD_MUTACION)
{
    int probabilidad = rand() % 100;          // Genera un número aleatorio entre 0 y 99
    if (probabilidad < PROBABILIDAD_MUTACION) // 5% de probabilidad de mut
    {
        // Decidir el tipo de mutación

        // Intercambio de 2 posiciones aleatorias del individuo
        int temp;

        // Cambio de dos posiciones aleatorias
        int pos1 = rand() % tamaño;
        int pos2 = rand() % tamaño;
        // Asegurarse de que las posiciones sean diferentes
        while (pos1 == pos2)
        {
            pos2 = rand() % tamaño;
        }
        // Intercambio de los valores en las posiciones seleccionadas
        temp = individuo[pos1];
        individuo[pos1] = individuo[pos2];
        individuo[pos2] = temp;
    }
}