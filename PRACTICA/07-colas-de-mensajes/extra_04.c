/* extra_04 — varios productores y un consumidor
 *
 *   ./extra_04 5 4      (5 productores, 4 mensajes cada uno)
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define CLAVE 7779
#define MAX_PIDS 64

typedef struct {
    long mtype;
    int  pid;
    int  valor;
} mensaje_t;

int main(int argc, char *argv[]) {
    int productores = (argc >= 2) ? atoi(argv[1]) : 5;
    int por_productor = (argc >= 3) ? atoi(argv[2]) : 4;
    if (productores < 1 || productores > MAX_PIDS || por_productor < 1) {
        fprintf(stderr, "1..%d productores, >=1 mensajes\n", MAX_PIDS);
        return 1;
    }

    int cola = msgget(CLAVE, 0660 | IPC_CREAT);
    if (cola == -1) {
        perror("msgget");
        return 1;
    }
    size_t carga = sizeof(mensaje_t) - sizeof(long);
    int total = productores * por_productor;

    for (int p = 0; p < productores; p++) {
        if (fork() == 0) {
            srand(getpid());
            for (int i = 0; i < por_productor; i++) {
                mensaje_t m = { .mtype = 1, .pid = (int) getpid(), .valor = i };
                msgsnd(cola, &m, carga, 0);
                usleep((rand() % 200) * 1000);
            }
            _exit(0);
        }
    }

    int pids[MAX_PIDS], cuenta[MAX_PIDS] = {0}, npids = 0;
    for (int r = 0; r < total; r++) {
        mensaje_t m;
        if (msgrcv(cola, &m, carga, 1, 0) == -1) {
            perror("msgrcv");
            break;
        }
        int k = -1;
        for (int j = 0; j < npids; j++)
            if (pids[j] == m.pid)
                k = j;
        if (k == -1) {
            k = npids++;
            pids[k] = m.pid;
        }
        cuenta[k]++;
    }

    for (int p = 0; p < productores; p++)
        wait(NULL);

    for (int j = 0; j < npids; j++)
        printf("pid %d -> %d mensajes\n", pids[j], cuenta[j]);

    msgctl(cola, IPC_RMID, NULL);
    return 0;
}
