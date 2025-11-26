#include <stdlib.h>
#define PROB_MEJOR 70
#include "evaluar.h"
#include <string.h>

void seleccion(poblacion *poblacion, int *individuo, int **costes, int tamaño)
{
    int tam_poblacion = poblacion->tamaño;
    int numNodos = poblacion->numNodos;
    
    int indice = rand() % tam_poblacion;
    int *individuoPoblacion = poblacion->individuos[indice];
    int valorPoblacion = evaluar(individuoPoblacion, costes, numNodos);
    int valorIndividuo = evaluar(individuo, costes, numNodos);
    int probabilidad = rand() % 100; // Genera un número aleatorio entre 0 y 99

    if(probabilidad < PROB_MEJOR) // probabilidad de que se compruebe si es mejor el que ya hay
    {
        //En caso de que la probabilidad se de, si el valor del individuo nuestro es mejor se sustituye
        if(valorPoblacion == -1) memcpy(individuoPoblacion, individuo, tamaño * sizeof(int));
        else if(valorPoblacion > valorIndividuo) memcpy(individuoPoblacion, individuo, tamaño * sizeof(int));
    }
    else memcpy(individuoPoblacion, individuo, tamaño * sizeof(int));
}