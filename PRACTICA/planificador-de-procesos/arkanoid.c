// arkanoid.c — interactivo: pala con el ratón, pelota y ladrillos a 60 fps; con lag va a cámara lenta y a saltos
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ventana.h"

#define FILAS       3
#define COLUMNAS    10
#define LADRILLO_AN (ANCHO / COLUMNAS)
#define LADRILLO_AL 10
#define LADRILLO_Y  20        // altura a la que empiezan los ladrillos
#define PALA_AN     50
#define PALA_Y      (ALTO - 10)
#define PERIODO_US  16000     // ~60 fotogramas por segundo

int main(int argc, char *argv[]) {
    Display *d = XOpenDisplay(NULL);
    Window w = abre_ventana(d, argc, argv);
    GC gc = XCreateGC(d, w, 0, NULL);
    Pixmap buffer = XCreatePixmap(d, w, ANCHO, ALTO, DefaultDepth(d, DefaultScreen(d)));   // doble buffer: sin parpadeo
    unsigned long colores[FILAS] = { 0xe04040, 0xe0a040, 0x40a0e0 };

    int ladrillos[FILAS][COLUMNAS];
    int quedan = 0;
    double bx = ANCHO / 2;
    double by = ALTO / 2;
    double vx = 2;
    double vy = -2;
    fps_t fps = { .inicio = ahora_ms() };
    for (;;) {
        if (quedan == 0) {   // nivel nuevo
            for (int f = 0; f < FILAS; f++) {
                for (int c = 0; c < COLUMNAS; c++) {
                    ladrillos[f][c] = 1;
                }
            }
            quedan = FILAS * COLUMNAS;
        }

        // la pala sigue la x del ratón
        Window raiz, hijo;
        int rx, ry, wx, wy;
        unsigned int botones;
        XQueryPointer(d, w, &raiz, &hijo, &rx, &ry, &wx, &wy, &botones);
        int pala_x = wx - PALA_AN / 2;
        pala_x = pala_x < 0 ? 0 : pala_x > ANCHO - PALA_AN ? ANCHO - PALA_AN : pala_x;

        // física
        bx += vx;
        by += vy;
        if (bx < 0 || bx > ANCHO) {
            vx = -vx;
        }
        if (by < 0) {
            vy = -vy;
        }
        if (vy > 0 && by >= PALA_Y && by < PALA_Y + 4 && bx >= pala_x && bx <= pala_x + PALA_AN) {
            vy = -vy;
        }
        if (by > ALTO) {     // se cae: vuelve al centro
            bx = ANCHO / 2;
            by = ALTO / 2;
            vy = -2;
        }
        if (by >= LADRILLO_Y && by < LADRILLO_Y + FILAS * LADRILLO_AL && bx >= 0 && bx < ANCHO) {
            int f = (by - LADRILLO_Y) / LADRILLO_AL;
            int c = bx / LADRILLO_AN;
            if (ladrillos[f][c]) {
                ladrillos[f][c] = 0;
                quedan--;
                vy = -vy;
            }
        }

        // dibujo
        XSetForeground(d, gc, 0x000000);
        XFillRectangle(d, buffer, gc, 0, 0, ANCHO, ALTO);
        for (int f = 0; f < FILAS; f++) {
            XSetForeground(d, gc, colores[f]);
            for (int c = 0; c < COLUMNAS; c++) {
                if (ladrillos[f][c]) {
                    XFillRectangle(d, buffer, gc, c * LADRILLO_AN, LADRILLO_Y + f * LADRILLO_AL, LADRILLO_AN - 2, LADRILLO_AL - 2);
                }
            }
        }
        XSetForeground(d, gc, 0xffffff);
        XFillRectangle(d, buffer, gc, pala_x, PALA_Y, PALA_AN, 4);
        XFillArc(d, buffer, gc, bx - 3, by - 3, 6, 6, 0, 360 * 64);
        XCopyArea(d, buffer, w, gc, 0, 0, ANCHO, ALTO, 0, 0);
        XFlush(d);

        medir_fps(d, w, &fps, "arkanoid");
        usleep(PERIODO_US);
    }
}
