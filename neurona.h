#ifndef NEURONA_H
#define NEURONA_H

typedef struct
{
    // Parámetros del modelo de Izhikevich
    float a;
    float b;
    float c;
    float d;

    // Estado dinámico
    float v; // membrana
    float u; // recuperación

    // Entrada actual (corriente)
    float I;

    // Flag de spike
    int spike;
} Neurona;

// Inicializa una neurona Izhikevich
// a,b,c,d Parámetros del modelo
// v0 Valor inicial del potencial de membrana
// u0 Valor inicial de la variable de recuperación
Neurona crear_neurona(float a, float b, float c, float d, float v0, float u0);

// Devuelve si la neurona ha generado un spike en el último paso.
int neurona_spike(Neurona *n);

// Funciones observadoras para cada paramámetro y variable de estado
float neurona_get_a(Neurona *n);
float neurona_get_b(Neurona *n);
float neurona_get_c(Neurona *n);
float neurona_get_d(Neurona *n);
float neurona_get_v(Neurona *n);
float neurona_get_u(Neurona *n);
float neurona_get_I(Neurona *n);

// Regular Spiking (RS)
Neurona crear_neurona_RS(float v0, float u0);

// Intrinsically Bursting (IB)
Neurona crear_neurona_IB(float v0, float u0);

// Chattering (CH)
Neurona crear_neurona_CH(float v0, float u0);

// Fast Spiking (FS)
Neurona crear_neurona_FS(float v0, float u0);

// Thalamo-cortical (TC)
Neurona crear_neurona_TC(float v0, float u0);

// Thalamic Reticular (RE)
Neurona crear_neurona_RE(float v0, float u0);

// Resonator (RZ)
Neurona crear_neurona_RZ(float v0, float u0);

// low-Threshold Spiking (LTS)
Neurona crear_neurona_LTS(float v0, float u0);

#endif
