#include <stdlib.h>
#include "evaluar.h"
#include <string.h>
#include "seleccion.h"
#include "seeds.h"

#define PROB_MEJOR 70

void seleccion(poblacion *poblacion, grafo* g, int *individuo)
{
    int tam_poblacion = poblacion->num_individuos;

    int indice = RAND() % tam_poblacion;
    int *individuoPoblacion = poblacion->individuos[indice];
    int valorPoblacion = evaluar(individuoPoblacion, g);
    int valorIndividuo = evaluar(individuo, g);
    int probabilidad = RAND() % 100; // Genera un número aleatorio entre 0 y 99


    if (probabilidad < PROB_MEJOR) // probabilidad de que se compruebe si es mejor el que ya hay
    {
        // En caso de que la probabilidad se de, si el valor del individuo nuestro es mejor se sustituye
        if (valorPoblacion == -1)
            memcpy(individuoPoblacion, individuo, g->num_nodos * sizeof(int));
        else if (valorIndividuo != -1 && valorPoblacion > valorIndividuo)
            memcpy(individuoPoblacion, individuo, g->num_nodos * sizeof(int));
    }
    else
        memcpy(individuoPoblacion, individuo, g->num_nodos * sizeof(int));
}
