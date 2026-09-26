// raytracer.c — cálculo: esferas con sombras y suavizado; sale píxel a píxel, más rápido cuanta más CPU recibe
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ventana.h"

#define MUESTRAS  8     // MUESTRAS x MUESTRAS rayos por píxel (suavizado): súbelo o bájalo según la máquina
#define N_ESFERAS 3

typedef struct {
    double x, y, z;
} vec;

typedef struct {
    vec    centro;
    double radio;
    vec    color;
} esfera_t;

vec suma(vec a, vec b) {
    return (vec){ a.x + b.x, a.y + b.y, a.z + b.z };
}

vec resta(vec a, vec b) {
    return (vec){ a.x - b.x, a.y - b.y, a.z - b.z };
}

vec escala(vec a, double k) {
    return (vec){ a.x * k, a.y * k, a.z * k };
}

double punto(vec a, vec b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec normaliza(vec a) {
    return escala(a, 1 / sqrt(punto(a, a)));
}

// gira p un ángulo a alrededor del eje y
vec rota_y(vec p, double a) {
    return (vec){ p.x * cos(a) + p.z * sin(a), p.y, -p.x * sin(a) + p.z * cos(a) };
}

// distancia a la esfera a lo largo del rayo o + t·dir (dir unitario), o -1 si no la corta
double corta_esfera(esfera_t e, vec o, vec dir) {
    vec oc = resta(o, e.centro);
    double b = punto(oc, dir);
    double disc = b * b - punto(oc, oc) + e.radio * e.radio;
    if (disc < 0) {
        return -1;
    }
    double t = -b - sqrt(disc);
    return t > 1e-4 ? t : -1;
}

// color que ve el rayo o + t·dir
vec traza(esfera_t esferas[], vec o, vec dir) {
    double t_min = 1e9;
    vec normal = { 0, 1, 0 };
    vec color = { 0.5, 0.7, 1.0 };   // cielo
    for (int i = 0; i < N_ESFERAS; i++) {
        double t = corta_esfera(esferas[i], o, dir);
        if (t > 0 && t < t_min) {
            t_min = t;
            normal = normaliza(resta(suma(o, escala(dir, t)), esferas[i].centro));
            color = esferas[i].color;
        }
    }
    if (dir.y < 0 && (-1 - o.y) / dir.y < t_min) {   // suelo a cuadros en y = -1
        t_min = (-1 - o.y) / dir.y;
        vec p = suma(o, escala(dir, t_min));
        normal = (vec){ 0, 1, 0 };
        color = ((int)floor(p.x) + (int)floor(p.z)) & 1 ? (vec){ 0.9, 0.9, 0.9 } : (vec){ 0.3, 0.3, 0.3 };
    }
    if (t_min == 1e9) {
        return color;
    }

    // luz difusa, y sombra si alguna esfera tapa la luz
    vec p = suma(o, escala(dir, t_min));
    vec luz = normaliza(resta((vec){ 5, 5, 5 }, p));
    double difusa = fmax(0, punto(normal, luz));
    for (int i = 0; i < N_ESFERAS; i++) {
        if (corta_esfera(esferas[i], p, luz) > 0) {
            difusa = 0;
        }
    }
    return escala(color, 0.2 + 0.8 * difusa);
}

int main(int argc, char *argv[]) {
    Display *d = XOpenDisplay(NULL);
    int s = DefaultScreen(d);
    Window w = abre_ventana(d, argc, argv);

    unsigned int pixeles[ALTO * ANCHO];
    XImage *img = XCreateImage(d, DefaultVisual(d, s), DefaultDepth(d, s), ZPixmap, 0, (char *)pixeles, ANCHO, ALTO, 32, 0);
    esfera_t esferas[N_ESFERAS] = {
        { {  0.0,  0.0,  0.0 }, 1.0, { 0.9, 0.2, 0.2 } },
        { { -2.0, -0.3,  1.0 }, 0.7, { 0.2, 0.8, 0.3 } },
        { {  1.8, -0.5, -1.0 }, 0.5, { 0.2, 0.4, 0.9 } },
    };

    for (int f = 1;; f++) {
        double angulo = f * 0.2;   // la cámara solo gira al acabar un fotograma
        vec camara = rota_y((vec){ 0, 1, 5 }, angulo);
        for (int py = 0; py < ALTO; py++) {
            for (int px = 0; px < ANCHO; px++) {
                vec total = { 0, 0, 0 };
                for (int sy = 0; sy < MUESTRAS; sy++) {
                    for (int sx = 0; sx < MUESTRAS; sx++) {
                        double u = (px + (sx + 0.5) / MUESTRAS - ANCHO / 2.0) / ALTO;
                        double v = -(py + (sy + 0.5) / MUESTRAS - ALTO / 2.0) / ALTO - 0.2;
                        vec dir = rota_y(normaliza((vec){ u, v, -1 }), angulo);
                        total = suma(total, traza(esferas, camara, dir));
                    }
                }
                vec c = escala(total, 1.0 / (MUESTRAS * MUESTRAS));
                pixeles[py * ANCHO + px] = (unsigned int)(255 * c.x) << 16 | (unsigned int)(255 * c.y) << 8 | (unsigned int)(255 * c.z);
                XPutImage(d, w, DefaultGC(d, s), img, px, py, px, py, 1, 1);   // cada píxel se ve en cuanto está calculado
                XFlush(d);
            }
        }
        pon_titulo(d, w, "raytracer fotograma %d", f);
    }
}
