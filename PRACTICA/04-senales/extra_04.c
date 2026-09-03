/* extra_04 — SIGINT bloqueado durante una seccion critica; sigpending lo detecta
 *
 *   ./extra_04      (pulsa Ctrl-C durante la cuenta atras)
 */
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

static volatile sig_atomic_t recibida = 0;

static void manejador(int sig) {
    (void) sig;
    recibida = 1;
}

int main(void) {
    struct sigaction sa = {0};
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = manejador;
    sigaction(SIGINT, &sa, NULL);

    sigset_t solo_int, previa;
    sigemptyset(&solo_int);
    sigaddset(&solo_int, SIGINT);

    printf("entrando en la seccion critica (10 s). Pulsa Ctrl-C ahora.\n");
    sigprocmask(SIG_BLOCK, &solo_int, &previa);

    for (int i = 10; i > 0; i--) {
        printf("  seccion critica: %d\n", i);
        sleep(1);
    }

    sigset_t pendientes;
    sigpending(&pendientes);
    if (sigismember(&pendientes, SIGINT))
        printf("SIGINT llego durante la seccion critica y quedo pendiente\n");
    else
        printf("no llego ningun SIGINT\n");

    printf("desbloqueando SIGINT...\n");
    sigprocmask(SIG_SETMASK, &previa, NULL);   /* aqui se entrega el pendiente */

    printf("recibida = %d\n", (int) recibida);
    return 0;
}
