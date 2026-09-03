/* extra_01 — hacen falta tres Ctrl-C para terminar
 *
 *   ./extra_01
 */
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

static volatile sig_atomic_t pulsaciones = 0;

static void al_sigint(int sig) {
    (void) sig;
    pulsaciones++;
}

int main(void) {
    struct sigaction sa = {0};
    sa.sa_handler = al_sigint;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    printf("pulsa Ctrl-C tres veces para terminar (pid %ld)\n", (long) getpid());
    while (pulsaciones < 3) {
        pause();
        printf("Ctrl-C %d/3\n", (int) pulsaciones);
    }
    printf("adios\n");
    return 0;
}
