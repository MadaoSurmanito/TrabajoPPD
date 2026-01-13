#include "emparejamiento.h"

int torneo_binario_local(poblacion *pob, int *vecinos, int nvec, grafo *MCostes);

void emparejamiento_random(poblacion pob, int padre[], int madre[], int num_nodos)
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

void emparejamiento_celular_C9(poblacion *pob, int idx,
                               int filas, int cols,
                               grafo *MCostes,
                               int *padre, int *madre)
{
    int x = idx / cols;
    int y = idx % cols;

    int vecinos[9];
    int k = 0;

    for (int dx = -1; dx <= 1; dx++)
        for (int dy = -1; dy <= 1; dy++)
        {
            int nx = (x + dx + filas) % filas;
            int ny = (y + dy + cols) % cols;
            vecinos[k++] = nx * cols + ny;
        }

    int p1 = torneo_binario_local(pob, vecinos, 9, MCostes);
    int p2 = torneo_binario_local(pob, vecinos, 9, MCostes);

    memcpy(padre, pob->individuos[p1], sizeof(int)*MCostes->num_nodos);
    memcpy(madre, pob->individuos[p2], sizeof(int)*MCostes->num_nodos);
}

int torneo_binario_local(poblacion *pob, int *vecinos, int nvec,
                         grafo *MCostes)
{
    int a = vecinos[rand() % nvec];
    int b = vecinos[rand() % nvec];

    int fa = evaluar(pob->individuos[a], MCostes);
    int fb = evaluar(pob->individuos[b], MCostes);

    return (fa < fb) ? a : b;
}
