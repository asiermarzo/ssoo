/* extra_04 — redireccion de entrada y salida: < fichero, > fichero, >> fichero
 *
 *   ./extra_04
 *   minish> wc -l < entrada.txt > cuenta.txt
 *   minish> echo hola >> registro.txt
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

        char *entrada = NULL, *salida = NULL;
        int anexar = 0, error = 0;
        for (int i = 0; i < n; ) {
            int es_ent = strcmp(tok[i], "<") == 0;
            int es_sal = strcmp(tok[i], ">") == 0 || strcmp(tok[i], ">>") == 0;
            if (!es_ent && !es_sal) {
                i++;
                continue;
            }
            if (i + 1 >= n) {
                fprintf(stderr, "falta el fichero tras '%s'\n", tok[i]);
                error = 1;
                break;
            }
            if (es_ent)
                entrada = tok[i + 1];
            else {
                salida = tok[i + 1];
                anexar = tok[i][1] == '>';
            }
            for (int j = i; j + 2 <= n; j++)
                tok[j] = tok[j + 2];
            n -= 2;
            tok[n] = NULL;
        }
        if (error || n == 0)
            continue;

        pid_t hijo = fork();
        if (hijo == -1) {
            perror("fork");
            continue;
        }
        if (hijo == 0) {
            if (entrada != NULL) {
                int fd = open(entrada, O_RDONLY);
                if (fd == -1) {
                    perror(entrada);
                    _exit(1);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            if (salida != NULL) {
                int flags = O_WRONLY | O_CREAT | (anexar ? O_APPEND : O_TRUNC);
                int fd = open(salida, flags, 0644);
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
