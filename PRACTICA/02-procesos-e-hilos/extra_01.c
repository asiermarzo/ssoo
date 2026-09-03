/* extra_01 — cadena de N procesos: P1 -> P2 -> ... -> PN
 *
 *   ./extra_01 5
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <n>\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);
    if (n < 1) {
        fprintf(stderr, "n debe ser >= 1\n");
        return 1;
    }

    for (int nivel = 1; nivel < n; nivel++) {
        pid_t hijo = fork();
        if (hijo == -1) {
            perror("fork");
            return 1;
        }
        if (hijo > 0) {
            printf("nivel %d  pid=%ld  ppid=%ld\n",
                   nivel, (long) getpid(), (long) getppid());
            waitpid(hijo, NULL, 0);
            return 0;
        }
        /* el hijo sigue el bucle y crea el siguiente eslabon */
    }

    printf("nivel %d  pid=%ld  ppid=%ld  (ultimo)\n",
           n, (long) getpid(), (long) getppid());
    return 0;
}
