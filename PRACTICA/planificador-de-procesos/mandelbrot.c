// mandelbrot.c — cálculo: Mandelbrot que gira y hace zoom; los fps del título muestran la CPU que recibe.
// Con [fotogramas] termina tras dibujarlos y guarda el último en mandelbrot_AAAAMMDD_HHMMSS.ppm
// (útil como urgente); sin él, no termina
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "ventana.h"

#define MAX_ITER 256
#define CENTRO_X -0.6
#define CENTRO_Y 0.0

// guarda la imagen en PPM (P6): una cabecera de texto y después 3 bytes RGB por píxel
void guarda_ppm(unsigned int pixeles[]) {
    char nombre[64];
    time_t t = time(NULL);
    strftime(nombre, sizeof(nombre), "mandelbrot_%Y%m%d_%H%M%S.ppm", localtime(&t));
    FILE *f = fopen(nombre, "wb");
    fprintf(f, "P6\n%d %d\n255\n", ANCHO, ALTO);
    for (int i = 0; i < ANCHO * ALTO; i++) {
        unsigned char rgb[3] = { pixeles[i] >> 16, pixeles[i] >> 8, pixeles[i] };
        fwrite(rgb, 1, 3, f);
    }
    fclose(f);
    printf("mandelbrot: guardado %s\n", nombre);
}

int main(int argc, char *argv[]) {
    int fotogramas_max = argc > 3 ? atoi(argv[3]) : 0;
    Display *d = XOpenDisplay(NULL);
    int s = DefaultScreen(d);
    Window w = abre_ventana(d, argc, argv);

    unsigned int pixeles[ALTO * ANCHO];
    XImage *img = XCreateImage(d, DefaultVisual(d, s), DefaultDepth(d, s), ZPixmap, 0, (char *)pixeles, ANCHO, ALTO, 32, 0);

    fps_t fps = { .inicio = ahora_ms() };
    for (int f = 1; fotogramas_max == 0 || f <= fotogramas_max; f++) {
        double angulo = f * 0.03;
        double radio = 0.8 + 0.5 * sin(angulo / 3);   // zoom que va y viene
        double co = cos(angulo);
        double si = sin(angulo);
        for (int py = 0; py < ALTO; py++) {
            for (int px = 0; px < ANCHO; px++) {
                double u = (px - ANCHO / 2.0) * radio / (ALTO / 2.0);
                double v = (py - ALTO / 2.0) * radio / (ALTO / 2.0);
                double cr = CENTRO_X + u * co - v * si;
                double ci = CENTRO_Y + u * si + v * co;
                int n = 0;
                for (double zr = 0, zi = 0; n < MAX_ITER && zr * zr + zi * zi < 4; n++) {
                    double t = zr * zr - zi * zi + cr;
                    zi = 2 * zr * zi + ci;
                    zr = t;
                }
                pixeles[py * ANCHO + px] = n == MAX_ITER ? 0 : (n * 9 % 256) << 16 | (n * 5 % 256) << 8 | (n * 15 % 256);
            }
        }
        XPutImage(d, w, DefaultGC(d, s), img, 0, 0, 0, 0, ANCHO, ALTO);
        XFlush(d);
        medir_fps(d, w, &fps, "mandelbrot");
    }
    guarda_ppm(pixeles);
    return 0;
}
