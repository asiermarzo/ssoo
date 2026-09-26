// cola.h — cola de procesos listos de procsched: un array que se puede recorrer.
// Incluir después de procsched.h (usa MAX_COMANDO).
#include <sys/types.h>

#define MAX_PROCESOS 64

typedef struct {
    pid_t pid;
    int   prioridad;               // 1, 2 o 3
    char  comando[MAX_COMANDO];
    long  ultimo_ms;               // instante en que salió de CPU (o en que se creó)
} proceso_t;

typedef struct {
    proceso_t procesos[MAX_PROCESOS];
    int       n;                   // procesos en la cola
} cola_t;

// añade p al final; devuelve 0, o -1 si la cola está llena
int cola_mete(cola_t *c, proceso_t p) {
    if (c->n == MAX_PROCESOS) {
        return -1;
    }
    c->procesos[c->n++] = p;
    return 0;
}

// saca el proceso de la posición i (0 = el primero) y desplaza los siguientes
proceso_t cola_saca(cola_t *c, int i) {
    proceso_t p = c->procesos[i];
    for (int j = i; j < c->n - 1; j++) {
        c->procesos[j] = c->procesos[j + 1];
    }
    c->n--;
    return p;
}
