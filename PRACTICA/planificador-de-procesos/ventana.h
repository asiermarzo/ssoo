// ventana.h — lo común a los programas de prueba con ventana X11
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define ANCHO 300
#define ALTO  200

typedef struct {
    long inicio;       // instante en que empezó el segundo actual
    int  fotogramas;   // fotogramas dibujados en el segundo actual
} fps_t;

static long ahora_ms(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1000 + t.tv_nsec / 1000000;
}

// crea y muestra una ventana ANCHO x ALTO en x = argv[1], y = argv[2] (0 si faltan)
static Window abre_ventana(Display *d, int argc, char *argv[]) {
    XSizeHints h = { .flags = USPosition | USSize, .x = argc > 1 ? atoi(argv[1]) : 0, .y = argc > 2 ? atoi(argv[2]) : 0, .width = ANCHO, .height = ALTO };
    Window w = XCreateSimpleWindow(d, DefaultRootWindow(d), h.x, h.y, ANCHO, ALTO, 0, 0, 0);
    XSetWMNormalHints(d, w, &h);
    XMapWindow(d, w);
    return w;
}

// como printf, pero escribe en el título de la ventana
static void pon_titulo(Display *d, Window w, const char *formato, ...) {
    char titulo[64];
    va_list args;
    va_start(args, formato);
    vsnprintf(titulo, sizeof(titulo), formato, args);
    va_end(args);
    XStoreName(d, w, titulo);
}

// llamar una vez por fotograma: cada segundo pone "<nombre> N fps" en el título
static void medir_fps(Display *d, Window w, fps_t *f, const char *nombre) {
    f->fotogramas++;
    long ahora = ahora_ms();
    if (ahora - f->inicio >= 1000) {
        pon_titulo(d, w, "%s %d fps", nombre, f->fotogramas);
        f->fotogramas = 0;
        f->inicio = ahora;
    }
}
