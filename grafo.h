typedef struct {
    int** coste, num_nodos;
} grafo;

// Crea un grafo con (num_nodos) nodos
grafo crear_grafo(int num_nodos);

// Libera la memoria asociada con los nodos
void liberar_grafo(grafo* g);

// Imprime el grafo a consola
void imprimir_grafo(grafo* g);

