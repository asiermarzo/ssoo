/* extra_02 — reproduce "cmd1 | cmd2"
 *
 *   ./extra_02 "ls -l" "wc -l"
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 32

static void trocear(char *cadena, char *destino[]) {
    int i = 0;
    for (char *t = strtok(cadena, " \t"); t != NULL && i < MAX_ARGS - 1;
         t = strtok(NULL, " \t"))
        destino[i++] = t;
    destino[i] = NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s \"cmd1 args\" \"cmd2 args\"\n", argv[0]);
        return 1;
    }

    char izq[256], der[256];
    snprintf(izq, sizeof izq, "%s", argv[1]);
    snprintf(der, sizeof der, "%s", argv[2]);

    char *args_izq[MAX_ARGS], *args_der[MAX_ARGS];
    trocear(izq, args_izq);
    trocear(der, args_der);

    int tuberia[2];
    if (pipe(tuberia) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t p1 = fork();
    if (p1 == 0) {
        dup2(tuberia[1], STDOUT_FILENO);
        close(tuberia[0]);
        close(tuberia[1]);
        execvp(args_izq[0], args_izq);
        perror("execvp cmd1");
        _exit(127);
    }

    pid_t p2 = fork();
    if (p2 == 0) {
        dup2(tuberia[0], STDIN_FILENO);
        close(tuberia[0]);
        close(tuberia[1]);
        execvp(args_der[0], args_der);
        perror("execvp cmd2");
        _exit(127);
    }

    close(tuberia[0]);
    close(tuberia[1]);
    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);
    return 0;
}
