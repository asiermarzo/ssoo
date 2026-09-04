/* primos.c — deberia imprimir los 5 primeros numeros primos y terminar.
 * Tiene un fallo: imprime 2 y 3 y despues se queda colgado para siempre,
 * sin imprimir nada mas y sin terminar nunca (no casca, no consume CPU util).
 * El objetivo es engancharse con gdb al proceso ya en marcha y ver por que.
 *
 *   gcc primos.c -g -Wall -Wextra -o primos
 *   ./primos      -> deberia imprimir 2 3 5 7 11 y terminar
 */
#include <stdio.h>

int es_primo(int n) {
    if (n < 2)
        return 0;
    for (int d = 2; d * d <= n; d++)
        if (n % d == 0)
            return 0;
    return 1;
}

int main(void) {
    int encontrados = 0;
    int candidato = 2;

    while (encontrados < 5) {
        if (es_primo(candidato)) {
            printf("%d\n", candidato);
            encontrados++;
            candidato++;
        }
    }

    return 0;
}
