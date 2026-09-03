/* extra_04 — productor/consumidor con buffer circular en memoria compartida
 *
 *   ./extra_04 1234 productor 10
 *   ./extra_04 1234 consumidor 10
 *   ./extra_04 1234 limpiar
 *
 * Semaforos:  0 = mutex (1)   1 = huecos libres (B)   2 = huecos ocupados (0)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define B 8

union semun {
    int              val;
    struct semid_ds *buf;
    unsigned short  *array;
};

typedef struct {
    int datos[B];
    int cabeza, cola;
} buffer_t;

static int sem_obtener(key_t clave) {
    int id = semget(clave, 3, IPC_CREAT | IPC_EXCL | 0660);
    if (id != -1) {
        union semun a;
        a.val = 1; semctl(id, 0, SETVAL, a);
        a.val = B; semctl(id, 1, SETVAL, a);
        a.val = 0; semctl(id, 2, SETVAL, a);
        return id;
    }
    if (errno == EEXIST)
        return semget(clave, 3, 0660);
    return -1;
}

static void op(int id, int num, int delta) {
    struct sembuf s = { (unsigned short) num, (short) delta, 0 };
    if (semop(id, &s, 1) == -1)
        perror("semop");
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Uso:\n"
                        "  %s <clave> productor <n>\n"
                        "  %s <clave> consumidor <n>\n"
                        "  %s <clave> limpiar\n", argv[0], argv[0], argv[0]);
        return 1;
    }
    key_t clave = (key_t) atoi(argv[1]);

    int shmid = shmget(clave, sizeof(buffer_t), IPC_CREAT | 0660);
    if (shmid == -1) {
        perror("shmget");
        return 1;
    }

    if (strcmp(argv[2], "limpiar") == 0) {
        shmctl(shmid, IPC_RMID, NULL);
        int sem = semget(clave, 3, 0660);
        if (sem != -1)
            semctl(sem, 0, IPC_RMID);
        printf("recursos liberados\n");
        return 0;
    }

    if (argc != 4) {
        fprintf(stderr, "falta <n>\n");
        return 1;
    }
    int soy_productor = strcmp(argv[2], "productor") == 0;
    int n = atoi(argv[3]);

    buffer_t *buf = shmat(shmid, NULL, 0);
    if (buf == (void *) -1) {
        perror("shmat");
        return 1;
    }
    int sem = sem_obtener(clave);
    if (sem == -1) {
        perror("semget");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        if (soy_productor) {
            int valor = (int) (getpid() % 100) * 1000 + i;
            op(sem, 1, -1);                 /* toma un hueco libre */
            op(sem, 0, -1);                 /* mutex */
            buf->datos[buf->cola] = valor;
            buf->cola = (buf->cola + 1) % B;
            op(sem, 0, +1);
            op(sem, 2, +1);                 /* un hueco ocupado mas */
            printf("producido %d\n", valor);
        } else {
            op(sem, 2, -1);                 /* toma un hueco ocupado */
            op(sem, 0, -1);
            int valor = buf->datos[buf->cabeza];
            buf->cabeza = (buf->cabeza + 1) % B;
            op(sem, 0, +1);
            op(sem, 1, +1);                 /* un hueco libre mas */
            printf("consumido %d\n", valor);
        }
        sleep(1);
    }

    shmdt(buf);
    printf("(para liberar: %s %d limpiar)\n", argv[0], (int) clave);
    return 0;
}
