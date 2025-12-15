#ifndef PRUEBAS_H
#define PRUEBAS_H

#include "Estructuras/grafo.h"

void pruebaSecuencial(int , int , grafo *, int);
void pruebaSecuencialSpike(int , int , grafo *, int);
void pruebaParalela(int tPoblacion, int nGeneraciones, grafo *g, int costeOptimo);
void pruebaParalelaSpike(int tPoblacion, int nGeneraciones, grafo *g, int costeOptimo);

#endif // PRUEBAS_H