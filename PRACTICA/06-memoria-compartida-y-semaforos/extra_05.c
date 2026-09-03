/* extra_05 — control de aforo con un semaforo N-ario
 *
 *   ./extra_05 3 8      (aforo 3, 8 clientes)
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

union semun {
    int              val;
    struct semid_ds *buf;
    unsigned short  *array;
};

static void op(int id, int delta) {
    struct sembuf s = { 0, (short) delta, 0 };
    if (semop(id, &s, 1) == -1)
        perror("semop");
}

int main(int argc, char *argv[]) {
    int aforo = (argc >= 2) ? atoi(argv[1]) : 3;
    int clientes = (argc >= 3) ? atoi(argv[2]) : 8;
    if (aforo < 1 || clientes < 1) {
        fprintf(stderr, "aforo y clientes deben ser > 0\n");
        return 1;
    }

    int sem = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
    if (sem == -1) {
        perror("semget");
        return 1;
    }
    union semun arg;
    arg.val = aforo;
    semctl(sem, 0, SETVAL, arg);

    printf("sala con aforo %d, %d clientes\n", aforo, clientes);
    for (int c = 0; c < clientes; c++) {
        if (fork() == 0) {
            srand(getpid());
            op(sem, -1);
            printf("  cliente %d ENTRA\n", c);
            sleep(1 + rand() % 3);
            printf("  cliente %d sale\n", c);
            op(sem, +1);
            _exit(0);
        }
    }
    for (int c = 0; c < clientes; c++)
        wait(NULL);

    semctl(sem, 0, IPC_RMID);
    printf("todos han pasado\n");
    return 0;
}
