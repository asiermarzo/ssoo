/* extra_01 — simulador de Round Robin (llegada 0 para todos los procesos)
 *
 *   ./extra_01 3        (quantum = 3)
 */
#include <stdio.h>
#include <stdlib.h>

#define MAXP 16

int main(int argc, char *argv[]) {
    int q = (argc == 2) ? atoi(argv[1]) : 3;
    if (q < 1) {
        fprintf(stderr, "quantum >= 1\n");
        return 1;
    }

    int rafaga[] = { 8, 4, 9, 5 };
    int nproc = (int) (sizeof rafaga / sizeof rafaga[0]);

    int restante[MAXP], fin[MAXP];
    for (int i = 0; i < nproc; i++) {
        restante[i] = rafaga[i];
        fin[i] = 0;
    }

    int reloj = 0, terminados = 0;
    printf("cronograma (quantum = %d):\n", q);
    while (terminados < nproc) {
        for (int i = 0; i < nproc; i++) {
            if (restante[i] <= 0)
                continue;
            int trozo = restante[i] < q ? restante[i] : q;
            printf("  t=%2d..%2d  P%d\n", reloj, reloj + trozo, i);
            reloj += trozo;
            restante[i] -= trozo;
            if (restante[i] == 0) {
                fin[i] = reloj;
                terminados++;
            }
        }
    }

    double espera = 0, retorno = 0;
    for (int i = 0; i < nproc; i++) {
        int tr = fin[i];                 /* llegada 0 => retorno = instante de fin */
        retorno += tr;
        espera  += tr - rafaga[i];
        printf("P%d: retorno=%d espera=%d\n", i, tr, tr - rafaga[i]);
    }
    printf("media retorno = %.2f, media espera = %.2f\n",
           retorno / nproc, espera / nproc);
    return 0;
}
