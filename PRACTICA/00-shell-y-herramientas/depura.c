/* depura.c — deberia sumar los enteros 1..N, pero tiene un fallo:
 * con N pequeno da un resultado erroneo y con N grande termina abruptamente.
 * El objetivo es encontrarlo con gdb (y con valgrind).
 *
 *   gcc -g -Wall -Wextra -o depura depura.c
 *   ./depura 5        -> deberia imprimir 15
 *   ./depura 500      -> deberia imprimir 125250
 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <N>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int valores[100];

    for (int i = 1; i <= n; i++)
        valores[i] = i;

    long suma = 0;
    for (int i = 0; i < n; i++)
        suma += valores[i];

    printf("Suma 1..%d = %ld\n", n, suma);
    return 0;
}
