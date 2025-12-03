#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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

void saltar_lineas(FILE *fp, size_t lineas) {
    char line[256];
    for(size_t i = 0; i < lineas;  ++i)
        fgets(line, 256, fp);
}

void imprimir_linea(FILE *fp) {
    char line[256];
    fgets(line, 256, fp);
    fputs(line, stdout);
}

grafo cargar_grafo(const char *path) {

    FILE *f = fopen(path, "r");
    if (!f) {
        perror("[ERROR]: ");
        exit(1);
    } 

    saltar_lineas(f, 3);

    int num_nodos = 0;
    
    if(!fscanf(f, "DIMENSION%*s%d", &num_nodos)) {
        fclose(f);
        perror("[ERROR]: ");
        exit(1);       
    }

    //printf("Num nodos: %d\n", num_nodos);
    grafo ret = crear_grafo(num_nodos);

    saltar_lineas(f, 3);

    int** nodos = (int**)malloc(sizeof(int*) * num_nodos);
    for(size_t i = 0; i < num_nodos; ++i)
        nodos[i] = (int*)malloc(sizeof(int) * 2);

    while(1) {

        int a, b, c;
        int continuar = fscanf(f, "%d %d %d", &a, &b, &c);

        if(continuar <= 0)
            break;

        nodos[a-1][0] = b;
        nodos[a-1][1] = c;
    }

    fclose(f);

    for(size_t i = 0; i < num_nodos; ++i)
    for(size_t j = 0; j < num_nodos; ++j) {

        if(i == j || ret.coste[i][j] != -1) 
            continue;

        int dist_x = nodos[i][0] - nodos[j][0],
            dist_y = nodos[i][1] - nodos[j][1],
            dist = (int)sqrt((double)(dist_x * dist_x) + (double)(dist_y * dist_y));

        ret.coste[i][j] = dist;
        ret.coste[j][i] = dist;
    }

    for(size_t i = 0; i < num_nodos; ++i) free(nodos[i]);
    free(nodos);

    return ret;
}