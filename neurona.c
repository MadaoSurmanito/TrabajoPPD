#include "neurona.h"

Neurona crear_neurona(float a, float b, float c, float d, float v0, float u0)
{
    Neurona n;
    n.a = a;
    n.b = b;
    n.c = c;
    n.d = d;
    n.v = v0;
    n.u = u0;
    n.I = 0.0f;
    n.spike = 0;
    return n;
}

int neurona_spike(Neurona *n)
{
    return n->spike;
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

// Regular Spiking (RS)
Neurona crear_neurona_RS(float v0, float u0)
{
    return crear_neurona(0.02f, 0.2f, -65.0f, 8.0f, v0, u0);
}

// Intrinsically Bursting (IB)
Neurona crear_neurona_IB(float v0, float u0)
{
    return crear_neurona(0.02f, 0.2f, -55.0f, 4.0f, v0, u0);
}

// Chattering (CH)
Neurona crear_neurona_CH(float v0, float u0)
{
    return crear_neurona(0.02f, 0.2f, -50.0f, 2.0f, v0, u0);
}

// Fast Spiking (FS)
Neurona crear_neurona_FS(float v0, float u0)
{
    return crear_neurona(0.1f, 0.2f, -65.0f, 2.0f, v0, u0);
}

// Thalamo-cortical (TC)
Neurona crear_neurona_TC(float v0, float u0)
{
    return crear_neurona(0.02f, 0.25f, -65.0f, 0.05f, v0, u0);
}

// Thalamic Reticular (RE)
Neurona crear_neurona_RE(float v0, float u0)
{
    return crear_neurona(0.1f, 0.25f, -65.0f, 2.0f, v0, u0);
}

// Resonator (RZ)
Neurona crear_neurona_RZ(float v0, float u0)
{
    return crear_neurona(0.1f, 0.26f, -65.0f, 2.0f, v0, u0);
}

// low-Threshold Spiking (LTS)
Neurona crear_neurona_LTS(float v0, float u0)
{
    return crear_neurona(0.02f, 0.25f, -65.0f, 2.0f, v0, u0);
}