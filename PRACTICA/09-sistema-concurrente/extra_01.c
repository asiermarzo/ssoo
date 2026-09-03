/* extra_01 — cauce surtidor -> caudalimetro -> sumidero con tuberias de fluido_t
 *
 *   ./extra_01 8
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

typedef struct {
    int contador, caudal;
} fluido_t;

int main(int argc, char *argv[]) {
    int n = (argc == 2) ? atoi(argv[1]) : 8;

    int p1[2], p2[2];
    if (pipe(p1) == -1 || pipe(p2) == -1) {
        perror("pipe");
        return 1;
    }

    if (fork() == 0) {                    /* surtidor */
        close(p1[0]); close(p2[0]); close(p2[1]);
        srand(getpid());
        for (int i = 1; i <= n; i++) {
            fluido_t f = { i, 5 + rand() % 20 };
            if (write(p1[1], &f, sizeof f) == -1)
                _exit(1);
            sleep(1);
        }
        close(p1[1]);
        _exit(0);
    }

    if (fork() == 0) {                    /* caudalimetro */
        close(p1[1]); close(p2[0]);
        fluido_t f;
        while (read(p1[0], &f, sizeof f) == (ssize_t) sizeof f) {
            printf("caudalimetro: descarga %d, caudal %d\n", f.contador, f.caudal);
            if (write(p2[1], &f, sizeof f) == -1)
                _exit(1);
        }
        close(p1[0]); close(p2[1]);
        _exit(0);
    }

    /* sumidero: proceso principal */
    close(p1[0]); close(p1[1]); close(p2[1]);
    long total = 0;
    fluido_t f;
    while (read(p2[0], &f, sizeof f) == (ssize_t) sizeof f) {
        total += f.caudal;
        printf("sumidero: acumulado %ld\n", total);
    }
    close(p2[0]);

    wait(NULL);
    wait(NULL);
    printf("total trasegado = %ld\n", total);
    return 0;
}
