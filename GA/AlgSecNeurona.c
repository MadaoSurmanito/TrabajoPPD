#include "Alg.h"

int *algGen_CHamiltoniano(int ngens, int TPoblacion, grafo *MCostes, Neurona *n)
{
    poblacion pob = crear_poblacion(TPoblacion, MCostes->num_nodos); // Genera los primeros individuos
    int *MejorSolucion = malloc(sizeof(int) * MCostes->num_nodos);   // Inicialmente cualquiera vale
    memcpy(MejorSolucion, pob.individuos[0], sizeof(int) * MCostes->num_nodos);  

    int CosteMejorSolucion = evaluar(MejorSolucion, MCostes);
    float sinmejorar = 0;

    /*
    FILE *fN = fopen("Analisis/Datos/DatosNeurona.txt", "w");
    FILE *fP = fopen("Analisis/Datos/DatosPoblacion.txt", "w");
    if (!fN || !fP) {
        perror("Error al abrir el fichero");
        exit(1);
    }
    */

    for (int i = 0; i < ngens; i++)
    {
        for (int k = 0; k < TPoblacion; k++) {
            int madre[MCostes->num_nodos], padre[MCostes->num_nodos], *hijo;

            emparejamiento_random(pob, padre, madre, MCostes->num_nodos); // Devuelve 2 individuos(padre y madre)

            hijo = cruce(padre, madre, MCostes); // Crea un nuevo individuo válido(hijo)

            if (neurona_get_v(n) > 0.0f)
                mutacion(hijo, MCostes->num_nodos); // Modifica al hijo dentro de una probabilidad
            seleccion(&pob, MCostes, hijo);         // Intenta insertar al hijo en la población

            int costeHijo = evaluar(hijo, MCostes);
            if (CosteMejorSolucion == -1 || costeHijo < CosteMejorSolucion)
            {
                memcpy(MejorSolucion, hijo, sizeof(int) * MCostes->num_nodos);
                CosteMejorSolucion = costeHijo;
                sinmejorar = 0;
            }
            else
            {
                sinmejorar += 1;
            }
            spike_neurona(n, 10.0f);           // Actualiza el estado de la neurona

            /*
            // (float)(i*TPoblacion+k)/(TPoblacion*ngens/50)+5.0f
            fprintf(fN, "%.4f\t%.4f\t%.4f\n", neurona_get_v(n), neurona_get_u(n), 10.0f);

            float mejorfPob, promfPob, peorfPob;
            evaluarPob(&pob, TPoblacion, MCostes, &mejorfPob, &promfPob, &peorfPob);
            fprintf(fP, "%.4f\t%.4f\t%.4f\n", mejorfPob, promfPob, peorfPob);
            */

            free(hijo);
        }
    }

    /*
    fclose(fN);
    fclose(fP);
    */

    liberar_poblacion(&pob);

    return MejorSolucion;
}

int *AlgSecNeurona(int ngens, int TPoblacion, grafo *MCostes, int tipo_neurona)
{
    Neurona n;
    switch (tipo_neurona)
    {
    case 1:
        n = crear_neurona_RS();
        break;
    case 2:
        n = crear_neurona_IB();
        break;
    case 3:
        n = crear_neurona_CH();
        break;
    case 4:
        n = crear_neurona_FS();
        break;
    case 5:
        n = crear_neurona_TC1();
        break;
    case 6:
        n = crear_neurona_TC2();
        break;
    case 7:
        n = crear_neurona_RZ();
        break;
    case 8:
        n = crear_neurona_LTS();
        break;
    }
    return algGen_CHamiltoniano(ngens, TPoblacion, MCostes, &n);
}
