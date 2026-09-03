/* extra_04 — deposito con semaforo de recurso; alertas si falta suministro
 *
 *   ./extra_04
 *
 * El semaforo cuenta litros disponibles. El surtidor intenta descontar un
 * volumen fijo con IPC_NOWAIT; si no hay bastante, encola una alerta de tipo 1.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define CLAVE 9004

union semun {
    int              val;
    struct semid_ds *buf;
    unsigned short  *array;
};

typedef struct {
    long tipo;
    int  pid;
    char texto[100];
} mensaje_t;

int main(void) {
    int sem = semget(CLAVE, 1, IPC_CREAT | 0660);
    int cola = msgget(CLAVE, IPC_CREAT | 0660);
    if (sem == -1 || cola == -1) {
        perror("IPC");
        return 1;
    }
    union semun a;
    a.val = 0;                            /* deposito inicialmente vacio */
    semctl(sem, 0, SETVAL, a);
    size_t carga = sizeof(mensaje_t) - sizeof(long);

    if (fork() == 0) {                    /* llena_deposito */
        struct sembuf mas;
        mas.sem_num = 0;
        mas.sem_flg = 0;
        sleep(2);
        mas.sem_op = 30;
        semop(sem, &mas, 1);
        printf("llena_deposito: +30 L\n");
        sleep(3);
        mas.sem_op = 20;
        semop(sem, &mas, 1);
        printf("llena_deposito: +20 L\n");
        _exit(0);
    }

    if (fork() == 0) {                    /* surtidor */
        for (int i = 1; i <= 4; i++) {
            sleep(2);
            struct sembuf intento = { 0, -25, IPC_NOWAIT };
            if (semop(sem, &intento, 1) == -1) {
                mensaje_t m = { .tipo = 1, .pid = (int) getpid() };
                snprintf(m.texto, sizeof m.texto,
                         "Problema de suministro en el surtidor %d, caudal insuficiente",
                         (int) getpid());
                msgsnd(cola, &m, carga, 0);
                printf("surtidor: descarga %d SIN suministro -> alerta\n", i);
            } else {
                printf("surtidor: descarga %d OK (-25 L)\n", i);
            }
        }
        _exit(0);
    }

    for (int i = 0; i < 2; i++)
        wait(NULL);

    for (mensaje_t m; msgrcv(cola, &m, carga, 0, IPC_NOWAIT) != -1; )
        printf("ALERTA: %s\n", m.texto);

    semctl(sem, 0, IPC_RMID);
    msgctl(cola, IPC_RMID, NULL);
    return 0;
}
