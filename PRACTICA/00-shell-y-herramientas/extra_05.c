/* extra_05 — estadisticas de los enteros pasados como argumentos
 *
 *   ./extra_05 4 8 15 16 23 42
 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s n1 n2 n3 ...\n", argv[0]);
        return 1;
    }

    long suma = 0;
    int minimo = atoi(argv[1]), maximo = atoi(argv[1]);
    for (int i = 1; i < argc; i++) {
        int v = atoi(argv[i]);
        suma += v;
        if (v < minimo)
            minimo = v;
        if (v > maximo)
            maximo = v;
    }

    int cuenta = argc - 1;
    printf("cuenta = %d\n", cuenta);
    printf("suma   = %ld\n", suma);
    printf("media  = %.2f\n", (double) suma / cuenta);
    printf("minimo = %d\n", minimo);
    printf("maximo = %d\n", maximo);
    return 0;
}
