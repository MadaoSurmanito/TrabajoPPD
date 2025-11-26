#include "cruce.h"

void pmx(int padre1[], int padre2[], int hijo[], int n, int c1, int c2) {
    int i, j;

    // Inicializar hijo con -1 (vacío)
    for (i = 0; i < n; i++)
        hijo[i] = -1;

    // Copiar el segmento central de padre1 al hijo
    for (i = c1; i <= c2; i++)
        hijo[i] = padre1[i];

    // Rellenar el resto usando mapeo
    for (i = c1; i <= c2; i++) {
        int elemento = padre2[i];

        // Si el elemento ya está en el hijo, saltar
        int yaExiste = 0;
        for (j = c1; j <= c2; j++) {
            if (hijo[j] == elemento) {
                yaExiste = 1;
                break;
            }
        }
        if (yaExiste) continue;

        // Buscar posición correspondiente (mapeo)
        int pos = i;
        int valor = elemento;

        while (1) {
            // buscar valor en padre1
            int posPadre1 = -1;
            for (j = 0; j < n; j++) {
                if (padre1[j] == valor) {
                    posPadre1 = j;
                    break;
                }
            }

            // nueva posición es la de padre2
            valor = padre2[posPadre1];

            // si cae fuera del segmento, rellenar ahí
            if (posPadre1 < c1 || posPadre1 > c2) {
                hijo[posPadre1] = elemento;
                break;
            }
        }
    }

    // Rellenar los huecos restantes con padre2 directamente
    for (i = 0; i < n; i++) {
        if (hijo[i] == -1)
            hijo[i] = padre2[i];
    }
}