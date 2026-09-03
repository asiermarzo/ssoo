/* extra_05 — suma paralela de 1..1000000 con K hilos
 *
 *   ./extra_05 8
 *
 * Compilar con -pthread. Cada hilo acumula su suma parcial en su propia
 * estructura; el hilo principal las combina tras pthread_join.
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 1000000
#define MAX_HILOS 64

typedef struct {
    const long *datos;
    long        desde, hasta;
    long        parcial;
} rango_t;

static void *sumar(void *arg) {
    rango_t *r = arg;
    r->parcial = 0;
    for (long i = r->desde; i < r->hasta; i++)
        r->parcial += r->datos[i];
    return NULL;
}

int main(int argc, char *argv[]) {
    int k = (argc == 2) ? atoi(argv[1]) : 4;
    if (k < 1 || k > MAX_HILOS) {
        fprintf(stderr, "numero de hilos entre 1 y %d\n", MAX_HILOS);
        return 1;
    }

    static long datos[N];
    for (long i = 0; i < N; i++)
        datos[i] = i + 1;

    pthread_t hilo[MAX_HILOS];
    rango_t rango[MAX_HILOS];
    for (int t = 0; t < k; t++) {
        rango[t].datos = datos;
        rango[t].desde = (long) t * N / k;
        rango[t].hasta = (long) (t + 1) * N / k;
        if (pthread_create(&hilo[t], NULL, sumar, &rango[t]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    long total = 0;
    for (int t = 0; t < k; t++) {
        pthread_join(hilo[t], NULL);
        total += rango[t].parcial;
    }

    printf("suma 1..%d con %d hilos = %ld (esperado %lld)\n",
           N, k, total, (long long) N * (N + 1) / 2);
    return 0;
}
