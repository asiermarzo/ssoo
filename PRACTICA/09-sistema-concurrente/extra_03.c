/* extra_03 — monitor: capacidad (memoria compartida + SIGUSR1) y alertas por prioridad
 *
 *   ./extra_03
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define CLAVE 9003
#define EVENTOS 4

typedef struct {
    long tipo;
    int  pid;
    char texto[100];
} mensaje_t;

static volatile sig_atomic_t aviso = 0;

static void al_usr1(int sig) {
    (void) sig;
    aviso = 1;
}

static void al_chld(int sig) {
    (void) sig;                          /* solo sirve para despertar de sigsuspend */
}

int main(void) {
    int shmid = shmget(CLAVE, sizeof(int), IPC_CREAT | 0660);
    int cola = msgget(CLAVE, IPC_CREAT | 0660);
    if (shmid == -1 || cola == -1) {
        perror("IPC");
        return 1;
    }
    int *capacidad = shmat(shmid, NULL, 0);
    *capacidad = 0;
    size_t carga = sizeof(mensaje_t) - sizeof(long);

    /* hijo: consume la cola de alertas por prioridad (tipo 1 antes que tipo 2) */
    pid_t lector = fork();
    if (lector == 0) {
        for (int i = 0; i < EVENTOS; i++) {
            mensaje_t m;
            if (msgrcv(cola, &m, carga, -2, 0) == -1) {
                perror("msgrcv");
                break;
            }
            printf("  [alerta tipo %ld] %s\n", m.tipo, m.texto);
        }
        _exit(0);
    }

    struct sigaction sa = {0};
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = al_usr1;
    sigaction(SIGUSR1, &sa, NULL);
    sa.sa_handler = al_chld;
    sigaction(SIGCHLD, &sa, NULL);

    sigset_t mask, previa;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGCHLD);
    sigprocmask(SIG_BLOCK, &mask, &previa);

    /* proceso que simula la planta: genera capacidad y alertas */
    pid_t planta = fork();
    if (planta == 0) {
        int *cap = shmat(shmid, NULL, 0);
        pid_t padre = getppid();
        for (int i = 1; i <= EVENTOS; i++) {
            sleep(1);
            *cap += 10 * i;
            kill(padre, SIGUSR1);
            mensaje_t m = { .tipo = (i % 2) ? 1 : 2, .pid = (int) getpid() };
            snprintf(m.texto, sizeof m.texto, "evento %d", i);
            msgsnd(cola, &m, carga, 0);
        }
        shmdt(cap);
        _exit(0);
    }

    for (int viva = 1; viva; ) {
        while (!aviso && viva) {
            sigsuspend(&previa);
            if (waitpid(planta, NULL, WNOHANG) == planta)
                viva = 0;               /* la planta ha terminado */
        }
        if (aviso) {
            aviso = 0;
            printf("monitor: presentes %d litros en el surtidor\n", *capacidad);
        }
    }
    sigprocmask(SIG_SETMASK, &previa, NULL);

    waitpid(lector, NULL, 0);
    shmdt(capacidad);
    shmctl(shmid, IPC_RMID, NULL);
    msgctl(cola, IPC_RMID, NULL);
    return 0;
}
