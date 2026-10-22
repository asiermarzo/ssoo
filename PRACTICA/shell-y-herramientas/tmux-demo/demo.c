/* demo.c con fallos, para depurar paso a paso
 */
#include <stdio.h>

int main(void) {
    int n = 12;
    int suma = 0;

    for (int i = 1; i <= n; i++) {
        suma += i;
        printf("  i=%2d  suma=%d\n", i, suma);
    }

    printf("total 1..%d = %d\n", n, suma);
    return 0;
}
