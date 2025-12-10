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
void pmx(int p1[], int p2[], int hijo[], int n, int c1, int c2)
{

    int i;

    // Inicializar hijo con -1
    for (i = 0; i < n; i++)
        hijo[i] = -1;

    // Copiar la parte fija del padre1
    for (i = c1; i <= c2; i++)
        hijo[i] = p1[i];
    // Para cada posición del segmento en padre2
    for (i = c1; i <= c2; i++)
    {
        int val = p2[i];

        // Si el valor ya está en el hijo, saltamos
        int existe = 0;
        for (int k = c1; k <= c2; k++)
        {
            if (hijo[k] == val)
            {
                existe = 1;
                break;
            }
        }
        if (existe)
            continue;

        // Buscar dónde debería ir: posición del val en p1
        int pos = i;
        int target = val;

        while (1)
        {
            // Encontrar la posición de target en p1
            int idx = -1;
            for (int j = 0; j < n; j++)
            {
                if (p1[j] == target)
                {
                    idx = j;
                    break;
                }
            }

            // Si la posición idx está fuera del segmento, colocamos el valor allí
            if (idx < c1 || idx > c2)
            {
                pos = idx;
                break;
            }
            else
            {
                // Si está dentro del segmento, buscamos el valor correspondiente en p2
                target = p2[idx];
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
int *cruce(int padre[], int madre[], grafo *MCostes)
{
    int n = MCostes->num_nodos;
    int *hijo = (int *)malloc(n * sizeof(int));
    if (!hijo)
    {
        perror("Error al asignar memoria para el hijo");
        exit(1);
    }

    // Seleccionar dos puntos de cruce aleatorios
    int c1 = rand() % n;
    int c2 = rand() % n;
    if (c1 > c2)
    {
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

void emparejamiento(poblacion pob, int padre[], int madre[], int num_nodos)
{
    int tam_poblacion = pob.num_individuos;

    int indicePadre = rand() % tam_poblacion;
    int indiceMadre = rand() % tam_poblacion;
    // Asegurarse de que el padre y la madre no sean el mismo individuo
    while (indiceMadre == indicePadre)
    {
        indiceMadre = rand() % tam_poblacion;
    }
    // Copiar los individuos seleccionados en padre y madre
    for (int i = 0; i < num_nodos; i++)
    {

        padre[i] = pob.individuos[indicePadre][i];
        madre[i] = pob.individuos[indiceMadre][i];
    }
}