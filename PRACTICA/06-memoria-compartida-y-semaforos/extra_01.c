/* extra_01 — crea una memoria compartida de enteros y la libera al recibir Ctrl-C
 *
 *   ./extra_01 1234 16
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

static volatile sig_atomic_t fin = 0;

static void al_sigint(int sig) {
    (void) sig;
    fin = 1;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <clave> <n_enteros>\n", argv[0]);
        return 1;
    }
    key_t clave = (key_t) atoi(argv[1]);
    int n = atoi(argv[2]);
    if (n < 1) {
        fprintf(stderr, "n_enteros debe ser >= 1\n");
        return 1;
    }

    int shmid = shmget(clave, n * sizeof(int), IPC_CREAT | IPC_EXCL | 0660);
    if (shmid == -1) {
        perror("shmget");
        return 1;
    }

    int *mem = shmat(shmid, NULL, 0);
    if (mem == (void *) -1) {
        perror("shmat");
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }

    for (int i = 0; i < n; i++)
        mem[i] = 0;

    struct sigaction sa = {0};
    sa.sa_handler = al_sigint;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    printf("memoria creada: clave=%d id=%d, %d enteros. Ctrl-C para liberar.\n",
           (int) clave, shmid, n);
    while (!fin)
        pause();

    shmdt(mem);
    shmctl(shmid, IPC_RMID, NULL);
    printf("\nmemoria liberada\n");
    return 0;
}
