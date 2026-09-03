/* extra_03 — simulador de colas multinivel no apropiativas (3 niveles)
 *
 *   ./extra_03
 *
 * Nivel 1: Round Robin (quantum 4)   Nivel 2: prioridades   Nivel 3: FCFS
 */
#include <stdio.h>

typedef struct {
    int id, nivel, prioridad, rafaga;
} proc_t;

int main(void) {
    proc_t p[] = {
        { 0, 1, 0, 4 },
        { 1, 2, 3, 6 },
        { 2, 2, 1, 5 },
        { 3, 3, 0, 3 },
        { 4, 1, 0, 2 },
    };
    int n = (int) (sizeof p / sizeof p[0]);
    const int quantum = 4;

    int restante[16];
    for (int i = 0; i < n; i++)
        restante[i] = p[i].rafaga;

    int reloj = 0;
    printf("orden de servicio:\n");

    /* --- Nivel 1: Round Robin --- */
    for (int pendiente = 1; pendiente; ) {
        pendiente = 0;
        for (int i = 0; i < n; i++) {
            if (p[i].nivel != 1 || restante[i] <= 0)
                continue;
            int trozo = restante[i] < quantum ? restante[i] : quantum;
            printf("  [N1 RR]          t=%2d..%2d  P%d\n", reloj, reloj + trozo, p[i].id);
            reloj += trozo;
            restante[i] -= trozo;
            if (restante[i] > 0)
                pendiente = 1;
        }
    }

    /* --- Nivel 2: prioridades no apropiativas (menor valor = mas prioridad) --- */
    for (;;) {
        int mejor = -1;
        for (int i = 0; i < n; i++)
            if (p[i].nivel == 2 && restante[i] > 0 &&
                (mejor == -1 || p[i].prioridad < p[mejor].prioridad))
                mejor = i;
        if (mejor == -1)
            break;
        printf("  [N2 PRIO prio=%d]  t=%2d..%2d  P%d\n",
               p[mejor].prioridad, reloj, reloj + restante[mejor], p[mejor].id);
        reloj += restante[mejor];
        restante[mejor] = 0;
    }

    /* --- Nivel 3: FCFS --- */
    for (int i = 0; i < n; i++) {
        if (p[i].nivel != 3 || restante[i] <= 0)
            continue;
        printf("  [N3 FCFS]         t=%2d..%2d  P%d\n",
               reloj, reloj + restante[i], p[i].id);
        reloj += restante[i];
        restante[i] = 0;
    }

    printf("tiempo total = %d\n", reloj);
    return 0;
}
