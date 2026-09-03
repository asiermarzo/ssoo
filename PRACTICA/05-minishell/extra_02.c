/* extra_02 — bucle de shell minimo: fork + execvp + waitpid, builtin exit
 *
 *   ./extra_02
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_TOK 64

static int trocea(char *linea, char *tok[], int max) {
    int n = 0;
    for (char *t = strtok(linea, " \t\n"); t != NULL && n < max - 1;
         t = strtok(NULL, " \t\n"))
        tok[n++] = t;
    tok[n] = NULL;
    return n;
}

int main(void) {
    char linea[1024];
    for (;;) {
        printf("minish> ");
        fflush(stdout);
        if (fgets(linea, sizeof linea, stdin) == NULL) {
            printf("\n");
            break;
        }

        char *tok[MAX_TOK];
        if (trocea(linea, tok, MAX_TOK) == 0)
            continue;
        if (strcmp(tok[0], "exit") == 0)
            break;

        pid_t hijo = fork();
        if (hijo == -1) {
            perror("fork");
            continue;
        }
        if (hijo == 0) {
            execvp(tok[0], tok);
            perror(tok[0]);
            _exit(127);
        }

        int estado;
        waitpid(hijo, &estado, 0);
        if (WIFEXITED(estado) && WEXITSTATUS(estado) != 0)
            printf("[codigo %d]\n", WEXITSTATUS(estado));
    }
    return 0;
}
