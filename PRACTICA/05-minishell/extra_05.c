/* extra_05 — una tuberia (cmd1 | cmd2) y manejo de SIGINT
 *
 *   ./extra_05
 *   minish> ps ax | grep bash
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
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
    signal(SIGINT, SIG_IGN);        /* la shell no muere con Ctrl-C */

    char linea[1024];
    for (;;) {
        printf("minish> ");
        fflush(stdout);
        if (fgets(linea, sizeof linea, stdin) == NULL) {
            printf("\n");
            break;
        }

        char *tok[MAX_TOK];
        int n = trocea(linea, tok, MAX_TOK);
        if (n == 0)
            continue;
        if (strcmp(tok[0], "exit") == 0)
            break;

        int barra = -1;
        for (int i = 0; i < n; i++)
            if (strcmp(tok[i], "|") == 0) {
                barra = i;
                break;
            }

        if (barra == -1) {
            pid_t h = fork();
            if (h == 0) {
                signal(SIGINT, SIG_DFL);
                execvp(tok[0], tok);
                perror(tok[0]);
                _exit(127);
            }
            waitpid(h, NULL, 0);
            continue;
        }

        tok[barra] = NULL;
        char **izq = tok;
        char **der = &tok[barra + 1];

        int p[2];
        if (pipe(p) == -1) {
            perror("pipe");
            continue;
        }

        pid_t p1 = fork();
        if (p1 == 0) {
            signal(SIGINT, SIG_DFL);
            dup2(p[1], STDOUT_FILENO);
            close(p[0]);
            close(p[1]);
            execvp(izq[0], izq);
            perror(izq[0]);
            _exit(127);
        }

        pid_t p2 = fork();
        if (p2 == 0) {
            signal(SIGINT, SIG_DFL);
            dup2(p[0], STDIN_FILENO);
            close(p[0]);
            close(p[1]);
            execvp(der[0], der);
            perror(der[0]);
            _exit(127);
        }

        close(p[0]);
        close(p[1]);
        waitpid(p1, NULL, 0);
        waitpid(p2, NULL, 0);
    }
    return 0;
}
