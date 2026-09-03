/* extra_01 — productor / consumidor a traves de una cola de mensajes
 *
 *   ./extra_01 1234 productor 1
 *   ./extra_01 1234 consumidor 2
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct {
    long mtype;
    int  pid;
    int  valor;
} mensaje_t;

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Uso:\n"
                        "  %s <clave> productor <periodo>\n"
                        "  %s <clave> consumidor <periodo>\n", argv[0], argv[0]);
        return 1;
    }
    key_t clave = (key_t) atoi(argv[1]);
    int periodo = atoi(argv[3]);
    const int total = 10;

    int cola = msgget(clave, 0660 | IPC_CREAT);
    if (cola == -1) {
        perror("msgget");
        return 1;
    }
    size_t carga = sizeof(mensaje_t) - sizeof(long);

    if (strcmp(argv[2], "productor") == 0) {
        for (int i = 1; i <= total; i++) {
            sleep(periodo);
            mensaje_t m = { .mtype = 1, .pid = (int) getpid(), .valor = i };
            if (msgsnd(cola, &m, carga, 0) == -1) {
                perror("msgsnd");
                return 1;
            }
            printf("producido %d\n", i);
        }
    } else if (strcmp(argv[2], "consumidor") == 0) {
        for (int i = 1; i <= total; i++) {
            mensaje_t m;
            if (msgrcv(cola, &m, carga, 1, 0) == -1) {
                perror("msgrcv");
                return 1;
            }
            sleep(periodo);
            printf("consumido %d (de pid %d)\n", m.valor, m.pid);
        }
        msgctl(cola, IPC_RMID, NULL);
        printf("cola eliminada\n");
    } else {
        fprintf(stderr, "rol no valido\n");
        return 1;
    }
    return 0;
}
