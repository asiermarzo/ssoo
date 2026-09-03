/* extra_02 — FCFS frente a SJF sobre la misma lista de rafagas (llegada 0)
 *
 *   ./extra_02
 */
#include <stdio.h>

static void simular(const char *nombre, const int rafaga[], int n) {
    int reloj = 0;
    double espera = 0, retorno = 0;
    for (int i = 0; i < n; i++) {
        espera  += reloj;               /* lo que P i espera antes de empezar */
        reloj   += rafaga[i];
        retorno += reloj;               /* instante de fin de P i */
    }
    printf("%s:  media espera = %.2f   media retorno = %.2f\n",
           nombre, espera / n, retorno / n);
}

int main(void) {
    int base[] = { 6, 8, 7, 3 };
    int n = (int) (sizeof base / sizeof base[0]);

    int fcfs[8], sjf[8];
    for (int i = 0; i < n; i++)
        fcfs[i] = sjf[i] = base[i];

    for (int i = 0; i < n - 1; i++)      /* ordena sjf de menor a mayor rafaga */
        for (int j = i + 1; j < n; j++)
            if (sjf[j] < sjf[i]) {
                int t = sjf[i];
                sjf[i] = sjf[j];
                sjf[j] = t;
            }

    printf("rafagas (llegada 0): 6 8 7 3\n");
    simular("FCFS", fcfs, n);
    simular("SJF ", sjf, n);
    return 0;
}
