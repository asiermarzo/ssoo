/* extra_02 — espera activa frente a semaforo: coste en CPU del proceso que espera
 *
 *   ./extra_02 activa
 *   ./extra_02
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/resource.h>

union semun {
    int              val;
    struct semid_ds *buf;
    unsigned short  *array;
};

int main(int argc, char *argv[]) {
    int activa = (argc == 2 && strcmp(argv[1], "activa") == 0);
    const int objetivo = 5;

    int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
    volatile int *contador = shmat(shmid, NULL, 0);
    *contador = 0;

    int sem = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
    union semun a;
    a.val = 0;
    semctl(sem, 0, SETVAL, a);

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    if (fork() == 0) {                    /* trabajador: avanza despacio */
        for (int i = 0; i < objetivo; i++) {
            sleep(1);
            (*contador)++;
            struct sembuf s = { 0, 1, 0 };
            semop(sem, &s, 1);
        }
        shmdt((void *) contador);
        _exit(0);
    }

    if (activa) {
        while (*contador < objetivo)
            ;                            /* ESPERA ACTIVA: quema CPU */
    } else {
        for (int i = 0; i < objetivo; i++) {
            struct sembuf w = { 0, -1, 0 };
            semop(sem, &w, 1);           /* bloqueo sin consumir CPU */
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &t1);
    wait(NULL);

    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    double cpu = ru.ru_utime.tv_sec + ru.ru_utime.tv_usec / 1e6 +
                 ru.ru_stime.tv_sec + ru.ru_stime.tv_usec / 1e6;
    double real = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1e9;
    printf("modo %s: contador=%d  tiempo real=%.2f s  CPU del que espera=%.3f s\n",
           activa ? "ACTIVA" : "semaforo", *contador, real, cpu);

    shmdt((void *) contador);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(sem, 0, IPC_RMID);
    return 0;
}
