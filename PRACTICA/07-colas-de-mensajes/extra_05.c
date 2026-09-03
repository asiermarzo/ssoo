/* extra_05 — barrera de sincronizacion mediante una cola de mensajes
 *
 *   ./extra_05 5
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define CLAVE 7780

typedef struct {
    long mtype;
    int  pid;
    int  valor;
} mensaje_t;

int main(int argc, char *argv[]) {
    int k = (argc == 2) ? atoi(argv[1]) : 5;
    if (k < 1) {
        fprintf(stderr, "k debe ser >= 1\n");
        return 1;
    }

    int cola = msgget(CLAVE, 0660 | IPC_CREAT);
    if (cola == -1) {
        perror("msgget");
        return 1;
    }
    size_t carga = sizeof(mensaje_t) - sizeof(long);

    for (int i = 0; i < k; i++) {
        if (fork() == 0) {
            srand(getpid());
            int trabajo = 1 + rand() % 5;
            sleep(trabajo);
            mensaje_t m = { .mtype = 1, .pid = (int) getpid(), .valor = trabajo };
            msgsnd(cola, &m, carga, 0);
            _exit(0);
        }
    }

    printf("esperando a %d trabajadores...\n", k);
    for (int i = 0; i < k; i++) {
        mensaje_t m;
        msgrcv(cola, &m, carga, 1, 0);
        printf("  trabajador %d listo (tardo %d s) [%d/%d]\n",
               m.pid, m.valor, i + 1, k);
    }
    printf("todos han llegado a la barrera\n");

    for (int i = 0; i < k; i++)
        wait(NULL);
    msgctl(cola, IPC_RMID, NULL);
    return 0;
}
