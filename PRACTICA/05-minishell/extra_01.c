/* extra_01 — tokenizador de lineas de comandos
 *
 *   ./extra_01        (escribe lineas, Ctrl-D para terminar)
 */
#include <stdio.h>
#include <string.h>

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
    printf("escribe lineas (Ctrl-D para terminar)\n");
    while (fgets(linea, sizeof linea, stdin) != NULL) {
        char *tok[MAX_TOK];
        int n = trocea(linea, tok, MAX_TOK);
        printf("%d tokens:", n);
        for (int i = 0; i < n; i++)
            printf(" [%s]", tok[i]);
        printf("\n");
    }
    return 0;
}
