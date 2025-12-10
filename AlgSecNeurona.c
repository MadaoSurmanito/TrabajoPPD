#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include "Estructuras/grafo.h"
#include "Estructuras/poblacion.h"
#include "Operadores/evaluar.h"
#include "Operadores/cruce.h"
#include "Operadores/mutacion.h"
#include "Operadores/seleccion.h"
#include "Estructuras/neurona.h"

int *algGen_CHamiltoniano(int ngens, int TPoblacion, grafo *MCostes, Neurona *n)
{
    printf("Generando población inicial...\n");
    poblacion pob = crear_poblacion(TPoblacion, MCostes->num_nodos); // Genera los primeros individuos
    int *MejorSolucion = malloc(sizeof(int) * MCostes->num_nodos);   // Inicialmente cualquiera vale
    memcpy(MejorSolucion, pob.individuos[1], sizeof(int) * MCostes->num_nodos);  

    printf("Iniciando evolución genética...\n");
    int CosteMejorSolucion = evaluar(MejorSolucion, MCostes);

    printf("Evolucionando por %d generaciones...\n", ngens);
    for (int i = 0; i < ngens; i++)
    {
        int madre[MCostes->num_nodos], padre[MCostes->num_nodos], *hijo;
        printf("Generación %d/%d\n", i + 1, ngens);

        emparejamiento(pob, padre, madre, MCostes->num_nodos); // Devvuelve 2 individuos(padre y madre)

        hijo = cruce(padre, madre, MCostes); // Crea un nuevo individuo válido(hijo)

        // La prob de mutacion va fuera, no en la funcion
        int prob_mutacion = rand() % 100;
        if (prob_mutacion < probabilidad_spike(n))
            mutacion(hijo, MCostes->num_nodos); // Modifica al hijo dentro de una probabilidad
        spike_neurona(n);                       // Actualiza el estado de la neurona
        seleccion(&pob, MCostes, hijo);         // Intenta insertar al hijo en la población

        if (CosteMejorSolucion == -1 || evaluar(hijo, MCostes) < CosteMejorSolucion)
        {
            memcpy(MejorSolucion, hijo, sizeof(int) * MCostes->num_nodos);
            CosteMejorSolucion = evaluar(hijo, MCostes);
        }
    }

    // liberar_poblacion(&pob); (No funciona por alguna razón)

    return MejorSolucion;
}

int *AlgSecNeurona(int ngens, int TPoblacion, grafo *MCostes, int tipo_neurona)
{
    Neurona n;
    switch (tipo_neurona)
    {
    case 1:
        n = crear_neurona_RS();
    case 2:
        n = crear_neurona_IB();
    case 3:
        n = crear_neurona_CH();
    case 4:
        n = crear_neurona_FS();
    case 5:
        n = crear_neurona_TC1();
    case 6:
        n = crear_neurona_TC2();
    case 7:
        n = crear_neurona_RZ();
    case 8:
        n = crear_neurona_LTS();
    default:
        printf("Tipo de neurona no válido. Usando Regular Spiking (RS) por defecto.\n");
        n = crear_neurona_RS();
    }
    return algGen_CHamiltoniano(ngens, TPoblacion, MCostes, &n);
}
