// pintar.c — interactivo: una estela sigue al ratón; con lag se queda atrás y las curvas salen quebradas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define ANCHO      300
#define ALTO       200
#define PUNTOS     64
#define PERIODO_US 10000   // 100 fotogramas por segundo

long ahora_ms(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1000 + t.tv_nsec / 1000000;
}

int main(int argc, char *argv[]) {
    Display *d = XOpenDisplay(NULL);
    int s = DefaultScreen(d);
    XSizeHints h = { .flags = USPosition | USSize, .x = argc > 1 ? atoi(argv[1]) : 0, .y = argc > 2 ? atoi(argv[2]) : 0, .width = ANCHO, .height = ALTO };
    Window w = XCreateSimpleWindow(d, RootWindow(d, s), h.x, h.y, ANCHO, ALTO, 0, 0, 0);
    XSetWMNormalHints(d, w, &h);
    XMapWindow(d, w);
    GC gc = XCreateGC(d, w, 0, NULL);
    XSetLineAttributes(d, gc, 3, LineSolid, CapRound, JoinRound);
    Pixmap buffer = XCreatePixmap(d, w, ANCHO, ALTO, DefaultDepth(d, s));   // doble buffer: sin parpadeo

    XPoint estela[PUNTOS];
    for (int i = 0; i < PUNTOS; i++) {
        estela[i] = (XPoint){ ANCHO / 2, ALTO / 2 };
    }

    long inicio = ahora_ms();
    long anterior = inicio;
    long max_hueco = 0;
    for (;;) {
        Window raiz, hijo;
        int rx, ry, wx, wy;
        unsigned int botones;
        XQueryPointer(d, w, &raiz, &hijo, &rx, &ry, &wx, &wy, &botones);
        memmove(&estela[1], &estela[0], (PUNTOS - 1) * sizeof(XPoint));
        estela[0] = (XPoint){ wx, wy };

        XSetForeground(d, gc, 0x000000);
        XFillRectangle(d, buffer, gc, 0, 0, ANCHO, ALTO);
        XSetForeground(d, gc, 0x40c0ff);
        XDrawLines(d, buffer, gc, estela, PUNTOS, CoordModeOrigin);
        XCopyArea(d, buffer, w, gc, 0, 0, ANCHO, ALTO, 0, 0);
        XFlush(d);

        // hueco = tiempo entre dos fotogramas; lo normal es ~PERIODO_US
        long ahora = ahora_ms();
        if (ahora - anterior > max_hueco) {
            max_hueco = ahora - anterior;
        }
        anterior = ahora;
        if (ahora - inicio >= 1000) {
            char titulo[64];
            snprintf(titulo, sizeof(titulo), "pintar hueco max %ld ms", max_hueco);
            XStoreName(d, w, titulo);
            max_hueco = 0;
            inicio = ahora;
        }
        usleep(PERIODO_US);
    }
}
