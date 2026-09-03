/* extra_04 — provoca de forma controlada un zombie y un huerfano
 *
 *   ./extra_04            (en otra terminal: watch -n1 'ps -o pid,ppid,state,cmd')
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    /* --- Zombie: el hijo termina y el padre tarda en recogerlo --- */
    pid_t z = fork();
    if (z == -1) { perror("fork"); return 1; }
    if (z == 0) {
        printf("[zombie]  hijo %ld termina de inmediato\n", (long) getpid());
        _exit(0);
    }
    printf("[zombie]  el padre no hace wait durante 5 s -> el hijo queda en estado Z\n");
    sleep(5);
    waitpid(z, NULL, 0);
    printf("[zombie]  el padre ya ha recogido al hijo\n\n");

    /* --- Huerfano: el padre muere antes que el hijo --- */
    pid_t h = fork();
    if (h == -1) { perror("fork"); return 1; }
    if (h == 0) {
        printf("[huerfano] hijo %ld, ppid inicial = %ld\n",
               (long) getpid(), (long) getppid());
        sleep(3);
        printf("[huerfano] hijo %ld, ppid tras morir el padre = %ld (adoptado)\n",
               (long) getpid(), (long) getppid());
        _exit(0);
    }
    printf("[huerfano] el padre termina ya; el hijo seguira 3 s mas\n");
    return 0;
}
