#ifndef NEURONA_H
#define NEURONA_H

typedef struct
{
    // Parámetros del modelo de Izhikevich
    float a; // tasa de recuperación
    float b; // sensibilidad de la recuperación al potencial de membrana
    float c; // valor de reinicio del potencial de membrana
    float d; // incremento de la variable de recuperación tras un spike

    // Estado dinámico
    float v; // membrana
    float u; // recuperación

    // Entrada actual (corriente)
    float I;

} Neurona;

// Inicializa una neurona Izhikevich
// a,b,c,d Parámetros del modelo
// v0 Valor inicial del potencial de membrana
// u0 Valor inicial de la variable de recuperación
Neurona crear_neurona(float a, float b, float c, float d);

// Funciones observadoras para cada paramámetro y variable de estado
float neurona_get_a(Neurona *n);
float neurona_get_b(Neurona *n);
float neurona_get_c(Neurona *n);
float neurona_get_d(Neurona *n);
float neurona_get_v(Neurona *n);
float neurona_get_u(Neurona *n);
float neurona_get_I(Neurona *n);

float probabilidad_spike(Neurona *n);

// Regular Spiking (RS)
Neurona crear_neurona_RS();

// Intrinsically Bursting (IB)
Neurona crear_neurona_IB();

// Chattering (CH)
Neurona crear_neurona_CH();

// Fast Spiking (FS)
Neurona crear_neurona_FS();

// Thalamo-cortical1 (TC1)
Neurona crear_neurona_TC1();

// Thalamo-cortical2 (TC2)
Neurona crear_neurona_TC2();

// Resonator (RZ)
Neurona crear_neurona_RZ();

// low-Threshold Spiking (LTS)
Neurona crear_neurona_LTS();

// Calcula el siguiente estado de la neurona usando el método de Euler
void spike_neurona(Neurona *n);

#endif
