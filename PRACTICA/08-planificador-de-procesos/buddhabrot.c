// buddhabrot.c — cálculo: acumula órbitas que escapan del Mandelbrot; la imagen se revela según la CPU recibida
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "ventana.h"

#define MAX_ITER    500
#define POR_DIBUJO  20000   // muestras entre dos redibujados

int main(int argc, char *argv[]) {
    Display *d = XOpenDisplay(NULL);
    int s = DefaultScreen(d);
    Window w = abre_ventana(d, argc, argv);

    unsigned int cuenta[ALTO * ANCHO] = {0};   // veces que una órbita pasa por cada píxel
    unsigned int pixeles[ALTO * ANCHO];
    XImage *img = XCreateImage(d, DefaultVisual(d, s), DefaultDepth(d, s), ZPixmap, 0, (char *)pixeles, ANCHO, ALTO, 32, 0);
    double orbita[MAX_ITER][2];
    srand(getpid());

    for (long muestras = 1;; muestras++) {
        // c al azar en [-2, 1] x [-1, 1]; se guarda su órbita
        double cr = -2 + 3.0 * rand() / RAND_MAX;
        double ci = -1 + 2.0 * rand() / RAND_MAX;
        int n = 0;
        for (double zr = 0, zi = 0; n < MAX_ITER && zr * zr + zi * zi < 4; n++) {
            double t = zr * zr - zi * zi + cr;
            zi = 2 * zr * zi + ci;
            zr = t;
            orbita[n][0] = zr;
            orbita[n][1] = zi;
        }
        // solo las órbitas que escapan suman a la imagen
        for (int i = 0; n < MAX_ITER && i < n; i++) {
            double fx = (orbita[i][0] + 2) / 3 * ANCHO;
            double fy = (orbita[i][1] + 1) / 2 * ALTO;
            if (fx >= 0 && fx < ANCHO && fy >= 0 && fy < ALTO) {
                cuenta[(int)fy * ANCHO + (int)fx]++;
            }
        }

        if (muestras % POR_DIBUJO == 0) {
            unsigned int max = 1;
            for (int i = 0; i < ALTO * ANCHO; i++) {
                if (cuenta[i] > max) {
                    max = cuenta[i];
                }
            }
            for (int i = 0; i < ALTO * ANCHO; i++) {
                unsigned int b = 255 * sqrt((double)cuenta[i] / max);
                pixeles[i] = b << 16 | b << 8 | b;
            }
            XPutImage(d, w, DefaultGC(d, s), img, 0, 0, 0, 0, ANCHO, ALTO);
            pon_titulo(d, w, "buddhabrot %ld mil muestras", muestras / 1000);
            XFlush(d);
        }
    }
}
