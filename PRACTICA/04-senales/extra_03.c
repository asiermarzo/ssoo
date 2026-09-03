/* extra_03 — ping-pong de senales entre padre e hijo (N rondas)
 *
 *   ./extra_03 10
 *
 * sigprocmask + sigsuspend evitan perder la senal por una condicion de carrera.
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

static volatile sig_atomic_t recibida = 0;

static void manejador(int sig) {
    (void) sig;
    recibida = 1;
}

int main(int argc, char *argv[]) {
    int rondas = (argc == 2) ? atoi(argv[1]) : 10;

    struct sigaction sa = {0};
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = manejador;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    sigset_t mascara, previa;
    sigemptyset(&mascara);
    sigaddset(&mascara, SIGUSR1);
    sigaddset(&mascara, SIGUSR2);
    sigprocmask(SIG_BLOCK, &mascara, &previa);

    pid_t hijo = fork();
    if (hijo == -1) {
        perror("fork");
        return 1;
    }

    if (hijo == 0) {
        pid_t ppid = getppid();
        for (int i = 0; i < rondas; i++) {
            sigsuspend(&previa);              /* espera SIGUSR1 */
            printf("  hijo:  recibida %d, devuelvo SIGUSR2\n", i + 1);
            kill(ppid, SIGUSR2);
        }
        printf("  hijo:  fin\n");
        _exit(0);
    }

    for (int i = 0; i < rondas; i++) {
        printf("padre: envio SIGUSR1 %d\n", i + 1);
        kill(hijo, SIGUSR1);
        sigsuspend(&previa);                  /* espera SIGUSR2 */
    }
    printf("padre: fin\n");
    wait(NULL);
    return 0;
}
