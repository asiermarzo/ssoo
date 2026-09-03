/* extra_04 — triangulo de Pascal de N filas
 *
 *   ./extra_04 6
 *
 * N por argumento, 1 <= N <= 30. Cada fila se obtiene de la anterior sumando in situ.
 */
#include <stdio.h>
#include <stdlib.h>

#define MAX_FILAS 30

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <filas>\n", argv[0]);
        return 1;
    }

    int filas = atoi(argv[1]);
    if (filas < 1 || filas > MAX_FILAS) {
        fprintf(stderr, "El numero de filas debe estar entre 1 y %d\n", MAX_FILAS);
        return 1;
    }

    long fila[MAX_FILAS + 1] = {1};
    for (int i = 0; i < filas; i++) {
        for (int j = i; j > 0; j--)
            fila[j] += fila[j - 1];
        for (int j = 0; j <= i; j++)
            printf("%ld ", fila[j]);
        printf("\n");
    }
    return 0;
}
