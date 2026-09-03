/* extra_02 — servidor de eco por tipos de mensaje
 *
 *   ./extra_02 servidor            (terminal 1)
 *   ./extra_02 cliente "hola"      (terminal 2)
 *
 * El servidor responde usando como mtype el pid del cliente.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define CLAVE 7777

typedef struct {
    long mtype;
    int  pid;
    char texto[128];
} mensaje_t;

static volatile sig_atomic_t parar = 0;

static void al_int(int sig) {
    (void) sig;
    parar = 1;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s servidor | %s cliente <texto>\n", argv[0], argv[0]);
        return 1;
    }

    int cola = msgget(CLAVE, 0660 | IPC_CREAT);
    if (cola == -1) {
        perror("msgget");
        return 1;
    }
    size_t carga = sizeof(mensaje_t) - sizeof(long);

    if (strcmp(argv[1], "servidor") == 0) {
        signal(SIGINT, al_int);
        printf("servidor en marcha (Ctrl-C para parar)\n");
        for (;;) {
            mensaje_t m;
            if (msgrcv(cola, &m, carga, 1, 0) == -1) {
                if (errno == EINTR && parar)
                    break;
                perror("msgrcv");
                break;
            }
            for (char *p = m.texto; *p != '\0'; p++)
                *p = (char) toupper((unsigned char) *p);
            m.mtype = m.pid;
            if (msgsnd(cola, &m, carga, 0) == -1)
                perror("msgsnd");
        }
        msgctl(cola, IPC_RMID, NULL);
        printf("\ncola eliminada\n");
        return 0;
    }

    if (strcmp(argv[1], "cliente") == 0 && argc == 3) {
        mensaje_t m = { .mtype = 1, .pid = (int) getpid() };
        snprintf(m.texto, sizeof m.texto, "%s", argv[2]);
        if (msgsnd(cola, &m, carga, 0) == -1) {
            perror("msgsnd");
            return 1;
        }
        if (msgrcv(cola, &m, carga, getpid(), 0) == -1) {
            perror("msgrcv");
            return 1;
        }
        printf("respuesta: %s\n", m.texto);
        return 0;
    }

    fprintf(stderr, "argumentos no validos\n");
    return 1;
}
