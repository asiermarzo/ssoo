/* extra_04 — Round Robin real de dos procesos con SIGSTOP / SIGCONT
 *
 *   ./extra_04 6        (6 turnos de 2 s)
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

static void trabajador(int id) {
    for (long i = 0; ; i++) {
        printf("  P%d: %ld\n", id, i);
        fflush(stdout);
        usleep(300000);
    }
}

int main(int argc, char *argv[]) {
    int turnos = (argc == 2) ? atoi(argv[1]) : 6;
    const int quantum = 2;

    pid_t hijo[2];
    for (int i = 0; i < 2; i++) {
        hijo[i] = fork();
        if (hijo[i] == -1) {
            perror("fork");
            return 1;
        }
        if (hijo[i] == 0) {
            trabajador(i);
            _exit(0);
        }
        kill(hijo[i], SIGSTOP);          /* los hijos arrancan detenidos */
    }

    for (int t = 0; t < turnos; t++) {
        int actual = t % 2;
        printf("turno %d -> P%d\n", t, actual);
        kill(hijo[actual], SIGCONT);
        sleep(quantum);
        kill(hijo[actual], SIGSTOP);
    }

    for (int i = 0; i < 2; i++) {
        kill(hijo[i], SIGKILL);
        waitpid(hijo[i], NULL, 0);
    }
    printf("planificacion terminada\n");
    return 0;
}
