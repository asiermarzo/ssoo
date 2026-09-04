/* division_cero.c — division entera por cero.
 * En x86 la CPU genera la excepcion #DE (divide error); el SO la traduce a
 * la senal SIGFPE y termina SOLO este proceso. La maquina sigue funcionando
 * con normalidad: es una interrupcion de la clase "programa" del Tema 1.
 *
 *   gcc -Wall -Wextra -O0 -o division_cero division_cero.c
 *   ./division_cero        -> "Floating point exception (core dumped)"  (SIGFPE)
 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int divisor = (argc > 1) ? atoi(argv[1]) : 0;
    int n = 10;

    printf("calculando %d / %d ...\n", n, divisor);
    printf("resultado: %d\n", n / divisor);             /* -> SIGFPE si divisor == 0 */
    return 0;
}
