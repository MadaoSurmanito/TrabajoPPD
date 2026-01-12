#include "neurona.h"

Neurona crear_neurona(float a, float b, float c, float d)
{
    Neurona n;
    n.a = a;
    n.b = b;
    n.c = c;
    n.d = d;
    n.v = c;
    n.u = b * c;
    n.I = 10.0f;
    return n;
}

float neurona_get_a(Neurona *n)
{
    return n->a;
}

float neurona_get_b(Neurona *n)
{
    return n->b;
}

float neurona_get_c(Neurona *n)
{
    return n->c;
}

float neurona_get_d(Neurona *n)
{
    return n->d;
}

float neurona_get_v(Neurona *n)
{
    return n->v;
}

float neurona_get_u(Neurona *n)
{
    return n->u;
}

float neurona_get_I(Neurona *n)
{
    return n->I;
}

float probabilidad_spike(Neurona *n)
{
    float p = (n->v - n->c) / (30.0f - n->c);

    // limitar a [0, 1]
    if (p < 0.0f) p = 0.0f;
    if (p > 1.0f) p = 1.0f;

    return p;
}

// Regular Spiking (RS)
Neurona crear_neurona_RS()
{
    return crear_neurona(0.02f, 0.2f, -65.0f, 8.0f);
}

// Intrinsically Bursting (IB)
Neurona crear_neurona_IB()
{
    return crear_neurona(0.02f, 0.2f, -55.0f, 4.0f);
}

// Chattering (CH)
Neurona crear_neurona_CH()
{
    return crear_neurona(0.02f, 0.2f, -50.0f, 2.0f);
}

// Fast Spiking (FS)
Neurona crear_neurona_FS()
{
    return crear_neurona(0.1f, 0.2f, -65.0f, 2.0f);
}

// Thalamo-cortical1 (TC1)
Neurona crear_neurona_TC1()
{
    return crear_neurona(0.02f, 0.25f, -65.0f, 0.05f);
}

// Thalamo-cortical2 (TC2)
Neurona crear_neurona_TC2()
{
    return crear_neurona(0.1f, 0.25f, -65.0f, 2.0f);
}

// Resonator (RZ)
Neurona crear_neurona_RZ()
{
    return crear_neurona(0.1f, 0.26f, -65.0f, 2.0f);
}

// low-Threshold Spiking (LTS)
Neurona crear_neurona_LTS()
{
    return crear_neurona(0.02f, 0.25f, -65.0f, 2.0f);
}

// Calcula el siguiente estado de la neurona usando el método de Euler
void spike_neurona(Neurona *n)
{
    // Parámetros de integración
    const float dt = 0.5f;

    // Spike externo único (pulso)
    n->I = 15.0f;

    // Integración corta (como recomienda Izhikevich)
    for (int i = 0; i < 2; i++)
    {
        float v = n->v;
        float u = n->u;

        float dv = 0.04f * v * v + 5.0f * v + 140.0f - u + n->I;
        float du = n->a * (n->b * v - u);

        n->v += dv * dt;
        n->u += du * dt;

        // Spike interno
        if (n->v >= 30.0f)
        {
            n->v = n->c;
            n->u += n->d;
        }
    }

    // Apagamos la corriente (pulso único)
    n->I = 0.0f;
}
