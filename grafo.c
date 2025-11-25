#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"

grafo crear_grafo(int num_nodos){
    grafo ret;
    ret.num_nodos = num_nodos;
    ret.coste = (int**)malloc(num_nodos * sizeof(int*));
    for(int i = 0; i < num_nodos; ++i) {
        ret.coste[i] = (int*)malloc(num_nodos * sizeof(int));
        memset(ret.coste[i], -1, num_nodos * sizeof(int));
    }
    return ret;
}

void liberar_grafo(grafo* g) {
    for(int i = 0; i < g->num_nodos; ++i)
        free(g->coste[i]); 
    free(g->coste);
}

void imprimir_grafo(grafo* g) {
    for(int x = 0; x < g->num_nodos; ++x) {
        printf("|\t");
        for(int y = 0; y < g->num_nodos; ++y)
            printf("%i\t", g->coste[x][y]);
        printf("|\n");
    }
    printf("\n");
}

grafo cargar_grafo(const char *path) {

    FILE *f = fopen(path, "r");
    if (!f) {
        perror("[ERROR]:\tNo existe el archivo");
        exit(1);
    } 

    int num_nodos;
    if(!fscanf(f, "%d", &num_nodos)) {
        fclose(f);
        perror("[ERROR]:\tError de formato");
        exit(1);       
    }

    grafo ret = crear_grafo(num_nodos);

    while(1) {
        int a, b, c;
        int continuar = fscanf(f, "%d %d %d", &a, &b, &c);
        if(continuar <= 0)
            break;
        ret.coste[a][b] = c;
        ret.coste[b][a] = c;
    }

    fclose(f);

    return ret;
}