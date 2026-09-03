/* extra_02 — lee/escribe una posicion de la memoria compartida con exclusion mutua
 *
 *   ./extra_02 1234 escribir 3 42
 *   ./extra_02 1234 leer 3
 *
 * Requiere que la memoria exista (crearla antes con extra_01).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

union semun {
    int              val;
    struct semid_ds *buf;
    unsigned short  *array;
};

static int sem_obtener(key_t clave) {
    int id = semget(clave, 1, IPC_CREAT | IPC_EXCL | 0660);
    if (id != -1) {
        union semun arg;
        arg.val = 1;
        semctl(id, 0, SETVAL, arg);
        return id;
    }
    if (errno == EEXIST)
        return semget(clave, 1, 0660);
    return -1;
}

static void sem_op(int id, int delta) {
    struct sembuf s = { 0, (short) delta, 0 };
    if (semop(id, &s, 1) == -1)
        perror("semop");
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Uso:\n"
                        "  %s <clave> escribir <pos> <valor>\n"
                        "  %s <clave> leer <pos>\n", argv[0], argv[0]);
        return 1;
    }
    key_t clave = (key_t) atoi(argv[1]);
    const char *orden = argv[2];
    int pos = atoi(argv[3]);

    int shmid = shmget(clave, 0, 0);
    if (shmid == -1) {
        perror("shmget (crea antes la memoria con extra_01)");
        return 1;
    }

    struct shmid_ds info;
    shmctl(shmid, IPC_STAT, &info);
    int n = (int) (info.shm_segsz / sizeof(int));
    if (pos < 0 || pos >= n) {
        fprintf(stderr, "posicion fuera de rango (0..%d)\n", n - 1);
        return 1;
    }

    int *mem = shmat(shmid, NULL, 0);
    if (mem == (void *) -1) {
        perror("shmat");
        return 1;
    }

    int sem = sem_obtener(clave);
    if (sem == -1) {
        perror("semget");
        shmdt(mem);
        return 1;
    }

    if (strcmp(orden, "escribir") == 0 && argc == 5) {
        int valor = atoi(argv[4]);
        sem_op(sem, -1);
        mem[pos] = valor;
        sem_op(sem, +1);
        printf("mem[%d] = %d\n", pos, valor);
    } else if (strcmp(orden, "leer") == 0) {
        sem_op(sem, -1);
        int valor = mem[pos];
        sem_op(sem, +1);
        printf("mem[%d] = %d\n", pos, valor);
    } else {
        fprintf(stderr, "orden no valida\n");
    }

    shmdt(mem);
    return 0;
}
