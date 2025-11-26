#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"
#include "poblacion.h"
#include "evaluar.h"

grafo cargarGrafoFichero(const char*);
int* algGen_CHamiltoniano(int, int, grafo*);

int main() {
    int ngens = 200, TPoblacion = 20;   //ngens = numero de generaciones     TPoblacion = tamaño poblacion
    int *MejorSolucion;
    
    grafo MCostes = cargarGrafoFichero("m8.txt");    // El fichero que contiene el grafo del problema

    int *MejorSolucion = algGen_CHamiltoniano(ngens, TPoblacion, &MCostes);
}

int* algGen_CHamiltoniano(int ngens, int TPoblacion, grafo* MCostes){
    poblacion *pob = generarPoblacionInicial(TPoblacion, MCostes->num_nodos);   // Genera los primeros individuos
    int *MejorSolucion = pob->individuos[1];                                                // Inicialmente cualquiera vale
    int CosteMejorSolucion = evaluar(MejorSolucion, MCostes);

    for (int i = 0; i < ngens; i++){
        int *madre, *padre, *hijo;

        //emparejamiento(pob, padre, madre, MCostes->num_nodos);                  //Devvuelve 2 individuos(padre y madre)
        //hijo = cruce(padre,madre, MCostes);                                     //Crea un nuevo individuo válido(hijo)
        //mutacion(hijo, MCostes->num_nodos);                                     //Modifica al hijo dentro de una probabilidad
        //seleccion(pob, hijo, TPoblacion);                                       // Modifica o no la Poblacion

        if(CosteMejorSolucion != -1 && CosteMejorSolucion > evaluar(hijo, MCostes)){
            MejorSolucion = hijo;
            CosteMejorSolucion = evaluar(hijo, MCostes);
        }
    }
}

grafo cargarGrafoFichero(const char* nombre) {

    FILE* f = fopen(nombre, "r");
    if (!f) {
        perror("Error al abrir fichero");
        exit(1);
    }

    int n;

    // Primera línea: número de nodos
    if (fscanf(f, "%d", &n) != 1) {
        fprintf(stderr, "Error: no se pudo leer el número de nodos del fichero.\n");
        exit(1);
    }

    // Crear grafo con n nodos
    grafo g = crear_grafo(n);

    // Leer matriz de costes n x n
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (fscanf(f, "%d", &g.coste[i][j]) != 1) {
                fprintf(stderr, "Error leyendo coste (%d,%d) del fichero.\n", i, j);
                exit(1);
            }
        }
    }

    fclose(f);
    return g;
}
