#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "cruce.h"
#include "seeds.h"

// Implementación del cruce PMX (Partially Mapped Crossover)
//      padre1: primer padre
//      padre2: segundo padre
//      hijo: hijo resultante
//      n: tamaño de los arreglos
//      c1, c2: puntos de cruce
void pmx(int p1[], int p2[], int hijo[], int n, int c1, int c2)
{
    int used[n];
    for(int i = 0; i < n; i++){
        hijo[i] = -1;
        used[i] = 0;
    }

    // 1. Copiar el segmento fijo desde padre1
    for(int i = c1; i <= c2; i++){
        hijo[i] = p1[i];
        used[p1[i]] = 1;
    }

    // 2. Rellenar con elementos de p2, respetando correspondencias
    for(int i = c1; i <= c2; i++){
        int val = p2[i];

        // Si ya está, saltar
        if(used[val]) continue;

        int pos = i;

        // Seguir el mapeo hasta encontrar hueco fuera del segmento
        while(pos >= c1 && pos <= c2){
            int target = p1[pos];
            // buscar target en p2
            for(int k = 0; k < n; k++){
                if(p2[k] == target){
                    pos = k;
                    break;
                }
            }
        }

        hijo[pos] = val;
        used[val] = 1;
    }

    // 3. Rellenar los huecos restantes con genes de p2 sin duplicarlos
    for(int i = 0; i < n; i++){
        if(hijo[i] == -1){
            int val = p2[i];
            if(!used[val]){
                hijo[i] = val;
                used[val] = 1;
            }
            else{
                // buscar primer valor no usado
                for(int v = 0; v < n; v++){
                    if(!used[v]){
                        hijo[i] = v;
                        used[v] = 1;
                        break;
                    }
                }
            }
        }
    }
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
    int c1 = RAND() % n;
    int c2 = RAND() % n;
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

    int indicePadre = RAND() % tam_poblacion;
    int indiceMadre = RAND() % tam_poblacion;
    // Asegurarse de que el padre y la madre no sean el mismo individuo
    while (indiceMadre == indicePadre)
    {
        indiceMadre = RAND() % tam_poblacion;
    }
    // Copiar los individuos seleccionados en padre y madre
    for (int i = 0; i < num_nodos; i++)
    {

        padre[i] = pob.individuos[indicePadre][i];
        madre[i] = pob.individuos[indiceMadre][i];
    }
}