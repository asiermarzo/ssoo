/* extra_05 — manejador con SA_SIGINFO: muestra el pid del emisor
 *
 *   ./extra_05 &
 *   kill -USR1 <pid>
 */
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

static volatile sig_atomic_t emisor_pid = 0;
static volatile sig_atomic_t contador = 0;

static void manejador(int sig, siginfo_t *info, void *ctx) {
    (void) sig;
    (void) ctx;
    emisor_pid = info->si_pid;
    contador++;
}

int main(void) {
    struct sigaction sa = {0};
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = manejador;
    sigaction(SIGUSR1, &sa, NULL);

    printf("pid %ld - enviame SIGUSR1:  kill -USR1 %ld\n",
           (long) getpid(), (long) getpid());

    while (contador < 3) {
        pause();
        printf("SIGUSR1 numero %d enviada por el proceso %ld\n",
               (int) contador, (long) emisor_pid);
    }
    printf("recibidas 3 senales, fin\n");
    return 0;
}
