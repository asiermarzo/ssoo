/* extra_02 — crono: cronometra la ejecucion de un comando
 *
 *   ./extra_02 sleep 1
 *   ./extra_02 ls -l
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <comando> [args...]\n", argv[0]);
        return 1;
    }

    struct timespec t0;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    pid_t hijo = fork();
    if (hijo == -1) {
        perror("fork");
        return 1;
    }
    if (hijo == 0) {
        execvp(argv[1], &argv[1]);
        perror("execvp");
        _exit(127);
    }

    int estado;
    waitpid(hijo, &estado, 0);

    struct timespec t1;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    double seg = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1e9;

    if (WIFEXITED(estado))
        printf("codigo de salida: %d\n", WEXITSTATUS(estado));
    else if (WIFSIGNALED(estado))
        printf("terminado por la senal %d\n", WTERMSIG(estado));
    printf("tiempo real: %.3f s\n", seg);
    return 0;
}
