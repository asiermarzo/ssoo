/* extra_03 — arbol binario de procesos de profundidad D
 *
 *   ./extra_03 3
 *
 * Cada proceso crea dos hijos hasta alcanzar la profundidad D.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

static void crear_subarbol(int nivel, int profundidad) {
    if (nivel >= profundidad)
        return;
    for (int i = 0; i < 2; i++) {
        pid_t hijo = fork();
        if (hijo == -1) {
            perror("fork");
            exit(1);
        }
        if (hijo == 0) {
            printf("nivel %d  pid=%ld  ppid=%ld\n",
                   nivel + 1, (long) getpid(), (long) getppid());
            crear_subarbol(nivel + 1, profundidad);
            exit(0);
        }
    }
    for (int i = 0; i < 2; i++)
        wait(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <profundidad>\n", argv[0]);
        return 1;
    }
    int profundidad = atoi(argv[1]);
    if (profundidad < 1 || profundidad > 5) {
        fprintf(stderr, "profundidad entre 1 y 5\n");
        return 1;
    }

    printf("nivel 0 (raiz)  pid=%ld\n", (long) getpid());
    crear_subarbol(0, profundidad);
    return 0;
}
