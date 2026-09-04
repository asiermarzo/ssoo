/* primos_cpu.c — quema CPU buscando primos durante ~3 s, imprime el ultimo
 * primo encontrado y espera a que pulses ENTER para empezar otra ronda.
 *
 * Lanzar VARIAS copias a la vez, cada una en su terminal, con htop abierto
 * en otra: se ve como el planificador del SO reparte la CPU entre los
 * procesos que compiten por ella (y como al pulsar ENTER en uno, ese deja
 * de consumir y los demas aprovechan su hueco).
 *
 *   gcc primos_cpu.c -Wall -Wextra -O2 -o primos_cpu
 *   ./primos_cpu
 */
#include <stdio.h>
#include <time.h>

int es_primo(long n) {
    if (n < 2)
        return 0;
    for (long d = 2; d * d <= n; d++)
        if (n % d == 0)
            return 0;
    return 1;
}

int main(void) {
    for (int ronda = 1; ; ronda++) {
        time_t fin = time(NULL) + 3;
        long contador = 0;
        long ultimo = 0;

        for (long n = 2; time(NULL) < fin; n++) {
            if (es_primo(n)) {
                ultimo = n;
                contador++;
            }
        }

        printf("ronda %d: %ld primos, el ultimo es %ld — ENTER para otra ronda...\n",
               ronda, contador, ultimo);
        if (getchar() == EOF)
            return 0;
    }
}
