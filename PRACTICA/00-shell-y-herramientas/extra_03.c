/* extra_03 — criba de Eratostenes: primos <= N
 *
 *   ./extra_03 100
 *
 * N por argumento, 2 <= N < 1000000. El array de marcas va en la pila.
 */
#include <stdio.h>
#include <stdlib.h>

#define LIMITE 1000000

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <N>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n < 2 || n >= LIMITE) {
        fprintf(stderr, "N debe estar entre 2 y %d\n", LIMITE - 1);
        return 1;
    }

    char compuesto[LIMITE] = {0};
    for (int i = 2; (long) i * i <= n; i++)
        if (!compuesto[i])
            for (int m = i * i; m <= n; m += i)
                compuesto[m] = 1;

    int cuenta = 0;
    for (int i = 2; i <= n; i++)
        if (!compuesto[i]) {
            printf("%d ", i);
            cuenta++;
        }

    printf("\n%d primos\n", cuenta);
    return 0;
}
