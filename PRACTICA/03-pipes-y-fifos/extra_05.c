/* extra_05 — cauce de tres procesos: generador -> filtro (pares) -> impresor
 *
 *   ./extra_05 20
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int n = (argc == 2) ? atoi(argv[1]) : 20;

    int p1[2], p2[2];
    if (pipe(p1) == -1 || pipe(p2) == -1) {
        perror("pipe");
        return 1;
    }

    if (fork() == 0) {                 /* generador: 1..n -> p1 */
        close(p1[0]); close(p2[0]); close(p2[1]);
        for (int i = 1; i <= n; i++)
            if (write(p1[1], &i, sizeof i) == -1)
                _exit(1);
        close(p1[1]);
        _exit(0);
    }

    if (fork() == 0) {                 /* filtro: pares de p1 -> p2 */
        close(p1[1]); close(p2[0]);
        int v;
        while (read(p1[0], &v, sizeof v) == (ssize_t) sizeof v)
            if (v % 2 == 0 && write(p2[1], &v, sizeof v) == -1)
                _exit(1);
        close(p1[0]); close(p2[1]);
        _exit(0);
    }

    /* impresor: proceso principal, lee p2 */
    close(p1[0]); close(p1[1]); close(p2[1]);
    int v;
    while (read(p2[0], &v, sizeof v) == (ssize_t) sizeof v)
        printf("%d ", v);
    printf("\n");
    close(p2[0]);

    wait(NULL);
    wait(NULL);
    return 0;
}
