typedef struct {
    int** individuos, num_individuos, num_nodos;
} poblacion;

// Crea una población con (num_individuos) individuos, cada uno es una lista de (num_nodos) nodos del camino
poblacion crear_poblacion(int num_individuos, int num_nodos);

// Libera la memoria asociada con la población
void liberar_poblacion(poblacion* g);

// Intercambiar las poblaciones
void intercambiar_poblaciones(poblacion* a, poblacion* b);