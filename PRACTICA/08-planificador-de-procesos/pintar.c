// pintar.c — interactivo: una estela sigue al ratón; con lag se queda atrás y las curvas salen quebradas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ventana.h"

#define PUNTOS     64
#define PERIODO_US 10000   // 100 fotogramas por segundo

int main(int argc, char *argv[]) {
    Display *d = XOpenDisplay(NULL);
    Window w = abre_ventana(d, argc, argv);
    GC gc = XCreateGC(d, w, 0, NULL);
    XSetLineAttributes(d, gc, 3, LineSolid, CapRound, JoinRound);
    Pixmap buffer = XCreatePixmap(d, w, ANCHO, ALTO, DefaultDepth(d, DefaultScreen(d)));   // doble buffer: sin parpadeo

    XPoint estela[PUNTOS];
    for (int i = 0; i < PUNTOS; i++) {
        estela[i] = (XPoint){ ANCHO / 2, ALTO / 2 };
    }

    fps_t fps = { .inicio = ahora_ms() };
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

        medir_fps(d, w, &fps, "pintar");
        usleep(PERIODO_US);
    }
}
