/* extra_03 — ejecutor con redireccion de salida: cmd ... > fichero
 *
 *   ./extra_03
 *   minish> ls -l > listado.txt
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
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
        int n = trocea(linea, tok, MAX_TOK);
        if (n == 0)
            continue;
        if (strcmp(tok[0], "exit") == 0)
            break;

        char *salida = NULL;
        for (int i = 0; i < n; i++)
            if (strcmp(tok[i], ">") == 0) {
                if (i + 1 < n)
                    salida = tok[i + 1];
                tok[i] = NULL;
                n = i;
                break;
            }

        pid_t hijo = fork();
        if (hijo == -1) {
            perror("fork");
            continue;
        }
        if (hijo == 0) {
            if (salida != NULL) {
                int fd = open(salida, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd == -1) {
                    perror(salida);
                    _exit(1);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            execvp(tok[0], tok);
            perror(tok[0]);
            _exit(127);
        }
        waitpid(hijo, NULL, 0);
    }
    return 0;
}
