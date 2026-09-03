/* extra_05 — mini-planificador: ejecuta varios comandos por turnos de un quantum
 *
 *   ./extra_05 "sleep 3" "sleep 4" "sleep 2"
 *
 * El quantum se controla con alarm + SIGALRM; al vencer, SIGSTOP al proceso
 * en curso y turno para el siguiente. Round Robin no apropiativo por quantum.
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXP 32

static volatile sig_atomic_t vencido = 0;

static void al_alarma(int sig) {
    (void) sig;
    vencido = 1;
}

static void al_chld(int sig) {
    (void) sig;                          /* solo sirve para despertar de pause() */
}

int main(int argc, char *argv[]) {
    if (argc < 2 || argc - 1 > MAXP) {
        fprintf(stderr, "Uso: %s \"cmd1\" [\"cmd2\" ...]   (max %d)\n", argv[0], MAXP);
        return 1;
    }
    int n = argc - 1;
    const int quantum = 2;

    struct sigaction sa = {0};
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = al_alarma;
    sigaction(SIGALRM, &sa, NULL);
    sa.sa_handler = al_chld;
    sigaction(SIGCHLD, &sa, NULL);

    pid_t pid[MAXP];
    int vivo[MAXP];
    for (int i = 0; i < n; i++) {
        pid[i] = fork();
        if (pid[i] == -1) {
            perror("fork");
            return 1;
        }
        if (pid[i] == 0) {
            char *args[] = { "/bin/sh", "-c", argv[i + 1], NULL };
            execv("/bin/sh", args);
            perror("execv");
            _exit(127);
        }
        kill(pid[i], SIGSTOP);
        vivo[i] = 1;
    }

    for (int restantes = n; restantes > 0; ) {
        for (int i = 0; i < n; i++) {
            if (!vivo[i])
                continue;

            printf("-> quantum para P%d (%s)\n", i, argv[i + 1]);
            vencido = 0;
            kill(pid[i], SIGCONT);
            alarm(quantum);

            for (int estado = 0; !vencido; ) {
                pid_t r = waitpid(pid[i], &estado, WNOHANG);
                if (r == pid[i] && (WIFEXITED(estado) || WIFSIGNALED(estado))) {
                    printf("   P%d ha terminado\n", i);
                    vivo[i] = 0;
                    restantes--;
                    break;
                }
                pause();
            }
            alarm(0);

            if (vivo[i]) {
                int estado = 0;
                kill(pid[i], SIGSTOP);
                if (waitpid(pid[i], &estado, WUNTRACED) == pid[i] &&
                    (WIFEXITED(estado) || WIFSIGNALED(estado))) {
                    printf("   P%d ha terminado\n", i);   /* justo al vencer el quantum */
                    vivo[i] = 0;
                    restantes--;
                }
            }
        }
    }

    printf("todos los procesos han terminado\n");
    return 0;
}
