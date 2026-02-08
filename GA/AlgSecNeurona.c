#include "Alg.h"

int *algGen_CHamiltoniano(int ngens, int TPoblacion, grafo *MCostes,
                          Neurona *n, const char *nombreNeurona, float pUmbral,
                          int datosPoblacion, int datosMutacion)
{
    int num_nodos = MCostes->num_nodos;

    poblacion pob = crear_poblacion(TPoblacion, num_nodos);
    int *madre = malloc(sizeof(int) * num_nodos);
    int *padre = malloc(sizeof(int) * num_nodos);

    int *MejorSolucion = malloc(sizeof(int) * num_nodos);
    memcpy(MejorSolucion, pob.individuos[0], sizeof(int) * num_nodos);
    int CosteMejorSolucion = evaluar(MejorSolucion, MCostes);

    char carpeta[256], carpetaM[256], carpetaP[256];
    sprintf(carpeta, "Analisis/Datos/Secuencial/Neurona_%s", nombreNeurona);
    sprintf(carpetaM, "%s/Mutacion_%.1f", carpeta, pUmbral);
    sprintf(carpetaP, "%s/Poblacion_%.1f", carpeta, pUmbral);

    if (datosPoblacion || datosMutacion)
    {
        mkdir("Analisis", 0777);
        mkdir("Analisis/Datos", 0777);
        mkdir("Analisis/Datos/Secuencial", 0777);
        mkdir(carpeta, 0777);
        mkdir(carpetaM, 0777);
        mkdir(carpetaP, 0777);
    }

    FILE *fMut = NULL;
    if (datosMutacion)
    {
        char fMutNombre[512];
        sprintf(fMutNombre, "%s/Mutacion.txt", carpetaM);
        fMut = fopen(fMutNombre, "w");
    }

    for (int i = 0; i < ngens; i++)
    {
        for (int k = 0; k < TPoblacion; k++)
        {
            int muta = 0;
            int entra = 0;

            emparejamiento_random(pob, padre, madre, num_nodos);
            int *hijo = cruce(padre, madre, MCostes);

            if (neurona_get_v(n) > (30.0f - ((1 - pUmbral) * (30.0f - neurona_get_c(n)))))
            {
                mutacion(hijo, num_nodos);
                muta = 1;
            }

            int costeHijo = evaluar(hijo, MCostes);
            entra = seleccion(&pob, MCostes, hijo, costeHijo);

            if (datosMutacion && fMut)
                fprintf(fMut, "%d %d %d\n", i, muta, entra);

            if (costeHijo < CosteMejorSolucion)
            {
                memcpy(MejorSolucion, hijo, sizeof(int) * num_nodos);
                CosteMejorSolucion = costeHijo;
            }

            spike_neurona(n, 10.0f);
            free(hijo);
        }

        if (datosPoblacion)
        {
            char fPob[512];
            sprintf(fPob, "%s/Gen%d.txt", carpetaP, i);

            FILE *f = fopen(fPob, "w");
            if (!f) continue;

            for (int j = 0; j < TPoblacion; j++)
            {
                fprintf(f, "%d ", j);
                for (int n = 0; n < num_nodos; n++)
                {
                    fprintf(f, "%d", pob.individuos[j][n]);
                    if (n < num_nodos - 1) fprintf(f, "-");
                }
                fprintf(f, " %d\n", evaluar(pob.individuos[j], MCostes));
            }
            fclose(f);
        }
    }

    free(madre);
    free(padre);

    if (fMut) fclose(fMut);
    liberar_poblacion(&pob);

    return MejorSolucion;
}

int *AlgSecNeurona(int ngens, int TPoblacion, grafo *MCostes, int tipo_neurona, 
                   float pUmbral, int datosPoblacion, int datosMutacion)
{
    Neurona n;
    const char *nombreNeurona;

    switch (tipo_neurona)
    {
        case 1: n = crear_neurona_RS();  nombreNeurona = "RS";  break;
        case 2: n = crear_neurona_IB();  nombreNeurona = "IB";  break;
        case 3: n = crear_neurona_CH();  nombreNeurona = "CH";  break;
        case 4: n = crear_neurona_FS();  nombreNeurona = "FS";  break;
        case 5: n = crear_neurona_TC1(); nombreNeurona = "TC1"; break;
        case 6: n = crear_neurona_TC2(); nombreNeurona = "TC2"; break;
        case 7: n = crear_neurona_RZ();  nombreNeurona = "RZ";  break;
        case 8: n = crear_neurona_LTS(); nombreNeurona = "LTS"; break;
        default: n = crear_neurona_RS(); nombreNeurona = "RS"; break;
    }

    return algGen_CHamiltoniano(ngens, TPoblacion, MCostes, &n,
                                nombreNeurona, pUmbral, datosPoblacion, datosMutacion);
}

int *AlgSecNeurona_DEF(int ngens, int TPoblacion, grafo *MCostes, int tipo_neurona)
{
    return AlgSecNeurona(ngens, TPoblacion, MCostes, tipo_neurona, 0.6f, 0, 0);
}
