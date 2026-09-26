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

// recorre la cola del primero al último; p es un proceso_t * a cada elemento.
// No encolar ni desencolar dentro del bucle: elegir primero, desencolar después.
#define COLA_FOR_EACH(c, p) \
    for (proceso_t *p = (c)->procesos; p < (c)->procesos + (c)->n; p++)

// añade p al final; devuelve 0, o -1 si la cola está llena
static int cola_encolar(cola_t *c, proceso_t p) {
    if (c->n == MAX_PROCESOS) {
        return -1;
    }
    c->procesos[c->n++] = p;
    return 0;
}

// saca el proceso al que apunta p (obtenido con COLA_FOR_EACH), desplaza los siguientes y lo devuelve
static proceso_t cola_desencolar(cola_t *c, proceso_t *p) {
    proceso_t sacado = *p;
    for (int j = p - c->procesos; j < c->n - 1; j++) {
        c->procesos[j] = c->procesos[j + 1];
    }
    c->n--;
    return sacado;
}
