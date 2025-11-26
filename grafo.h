#ifndef GRAFO_H
#define GRAFO_H


typedef struct {
    int** coste, num_nodos;
} grafo;

// Crea un grafo con (num_nodos) nodos
grafo crear_grafo(int num_nodos);

// Libera la memoria asociada con los nodos
void liberar_grafo(grafo* g);

// Imprime el grafo a consola
void imprimir_grafo(grafo* g);

// Carga la información de un archivo de texto a un grafo
// El formato es el siguiente
/*
    x               <- Número de nodos del grafo
    a b c           <- El camino de a a b es de coste c
    x y z           <- El camino de x a y es de coste z
    ...             <- El resto de caminos
*/
// No es recomendable añadir espacios ni identación ni carácteres extra
// Retornará 1 si se ha encontrado algún error
grafo cargar_grafo(const char *path);

#endif