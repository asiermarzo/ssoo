/* extra_03 — condicion de carrera sobre un contador en memoria compartida
 *
 *   ./extra_03          sin semaforo -> resultado incorrecto
 *   ./extra_03 sem      con semaforo -> resultado exacto
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

union semun {
    int              val;
    struct semid_ds *buf;
    unsigned short  *array;
};

int main(int argc, char *argv[]) {
    int usar_sem = (argc == 2 && strcmp(argv[1], "sem") == 0);
    const int hijos = 4, incrementos = 100000;

    int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
    if (shmid == -1) {
        perror("shmget");
        return 1;
    }
    volatile int *contador = shmat(shmid, NULL, 0);
    *contador = 0;

    int sem = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
    union semun arg;
    arg.val = 1;
    semctl(sem, 0, SETVAL, arg);

    for (int h = 0; h < hijos; h++) {
        if (fork() == 0) {
            for (int i = 0; i < incrementos; i++) {
                if (usar_sem) {
                    struct sembuf w = { 0, -1, 0 };
                    semop(sem, &w, 1);
                }
                (*contador)++;
                if (usar_sem) {
                    struct sembuf s = { 0, 1, 0 };
                    semop(sem, &s, 1);
                }
            }
            shmdt((void *) contador);
            _exit(0);
        }
    }
    for (int h = 0; h < hijos; h++)
        wait(NULL);

    printf("con%s semaforo: contador = %d (esperado %d)\n",
           usar_sem ? "" : " NO", *contador, hijos * incrementos);

    shmdt((void *) contador);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(sem, 0, IPC_RMID);
    return 0;
}
