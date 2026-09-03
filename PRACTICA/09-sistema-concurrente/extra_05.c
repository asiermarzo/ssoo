/* extra_05 — gestor: crea toda la infraestructura y la libera al recibir Ctrl-C
 *
 *   ./extra_05        (Ctrl-C para el apagado ordenado)
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define CLAVE 9005

union semun {
    int              val;
    struct semid_ds *buf;
    unsigned short  *array;
};

static volatile sig_atomic_t apagar = 0;

static void al_int(int sig) {
    (void) sig;
    apagar = 1;
}

int main(void) {
    int shmid = shmget(CLAVE, sizeof(int), IPC_CREAT | IPC_EXCL | 0660);
    if (shmid == -1) {
        perror("shmget (libera con 'ipcrm' si quedo de una ejecucion previa)");
        return 1;
    }
    int sem = semget(CLAVE, 2, IPC_CREAT | 0660);
    int cola = msgget(CLAVE, IPC_CREAT | 0660);
    union semun a;
    a.val = 1;
    semctl(sem, 0, SETVAL, a);
    semctl(sem, 1, SETVAL, a);

    int *estado = shmat(shmid, NULL, 0);
    *estado = 0;

    struct sigaction sa = {0};
    sa.sa_handler = al_int;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    pid_t hijo[3];
    for (int i = 0; i < 3; i++) {
        hijo[i] = fork();
        if (hijo[i] == 0) {
            signal(SIGINT, SIG_IGN);          /* solo el gestor atiende Ctrl-C */
            for (;;) {
                sleep(1);
                printf("  hijo %d activo (pid %ld)\n", i, (long) getpid());
                fflush(stdout);
            }
        }
    }

    printf("gestor en marcha (pid %ld). Ctrl-C para apagar ordenadamente.\n",
           (long) getpid());
    while (!apagar)
        pause();

    printf("\napagando...\n");
    for (int i = 0; i < 3; i++) {
        kill(hijo[i], SIGTERM);
        waitpid(hijo[i], NULL, 0);
        printf("  hijo %d terminado\n", i);
    }

    shmdt(estado);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(sem, 0, IPC_RMID);
    msgctl(cola, IPC_RMID, NULL);
    printf("todos los recursos IPC liberados (comprueba con 'ipcs')\n");
    return 0;
}
