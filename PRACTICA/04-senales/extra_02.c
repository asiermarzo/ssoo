/* extra_02 — cronometro con SIGALRM reprogramada
 *
 *   ./extra_02      (Ctrl-C para parar)
 */
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

static volatile sig_atomic_t segundos = 0;
static volatile sig_atomic_t terminar = 0;

static void al_alarma(int sig) {
    (void) sig;
    segundos++;
    alarm(1);
}

static void al_sigint(int sig) {
    (void) sig;
    terminar = 1;
}

int main(void) {
    struct sigaction sa = {0};
    sigemptyset(&sa.sa_mask);

    sa.sa_handler = al_alarma;
    sigaction(SIGALRM, &sa, NULL);
    sa.sa_handler = al_sigint;
    sigaction(SIGINT, &sa, NULL);

    printf("cronometro en marcha, Ctrl-C para parar\n");
    alarm(1);
    while (!terminar) {
        pause();
        if (!terminar)
            printf("%d s\n", (int) segundos);
    }
    printf("tiempo final: %d s\n", (int) segundos);
    return 0;
}
