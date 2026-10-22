/* division_cero.c — divide dos enteros; si el divisor es 0, el programa casca.
 *
 *   gcc division_cero.c -g -Wall -o division_cero
 *   ./division_cero 10 0      -> "Floating point exception (core dumped)" (SIGFPE)
 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <dividendo> <divisor>\n", argv[0]);
        return 1;
    }

    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    int cociente = a / b;

    printf("%d / %d = %d\n", a, b, cociente);
    return 0;
}
