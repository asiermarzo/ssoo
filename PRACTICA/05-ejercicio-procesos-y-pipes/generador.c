#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define N 1000

// Escribe por la salida estándar N números aleatorios entre 1 y 3999, uno por línea.
int main(void) {
    srand(time(NULL) ^ getpid());
    for (int i = 0; i < N; i++)
        printf("%d\n", rand() % 3999 + 1);

    return 0;
}
