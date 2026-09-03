/* extra_03 — efecto del parametro msgtyp de msgrcv
 *
 *   ./extra_03
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define CLAVE 7778

typedef struct {
    long mtype;
    int  valor;
} mensaje_t;

int main(void) {
    int cola = msgget(CLAVE, 0660 | IPC_CREAT);
    if (cola == -1) {
        perror("msgget");
        return 1;
    }
    size_t carga = sizeof(mensaje_t) - sizeof(long);

    int tipos[] = { 2, 1, 3, 1 };
    for (int i = 0; i < 4; i++) {
        mensaje_t m = { .mtype = tipos[i], .valor = i };
        msgsnd(cola, &m, carga, 0);
    }
    printf("encolados: (t2,v0) (t1,v1) (t3,v2) (t1,v3)\n\n");

    mensaje_t m;
    msgrcv(cola, &m, carga, 1, 0);
    printf("msgtyp = 1   -> tipo %ld valor %d  (primero de tipo 1)\n", m.mtype, m.valor);
    msgrcv(cola, &m, carga, 0, 0);
    printf("msgtyp = 0   -> tipo %ld valor %d  (el mas antiguo)\n", m.mtype, m.valor);
    msgrcv(cola, &m, carga, -2, 0);
    printf("msgtyp = -2  -> tipo %ld valor %d  (menor tipo <= 2)\n", m.mtype, m.valor);
    msgrcv(cola, &m, carga, 0, 0);
    printf("msgtyp = 0   -> tipo %ld valor %d\n", m.mtype, m.valor);

    msgctl(cola, IPC_RMID, NULL);
    return 0;
}
