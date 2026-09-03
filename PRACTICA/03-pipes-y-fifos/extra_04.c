/* extra_04 — el padre multiplexa dos tuberias con select
 *
 *   ./extra_04
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/wait.h>

static void hijo_escritor(int fd, int periodo, const char *nombre) {
    for (int i = 1; i <= 5; i++) {
        sleep(periodo);
        char linea[64];
        int len = snprintf(linea, sizeof linea, "%s #%d\n", nombre, i);
        if (write(fd, linea, len) == -1)
            _exit(1);
    }
    close(fd);
    _exit(0);
}

int main(void) {
    int a[2], b[2];
    if (pipe(a) == -1 || pipe(b) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t h1 = fork();
    if (h1 == 0) {
        close(a[0]); close(b[0]); close(b[1]);
        hijo_escritor(a[1], 1, "rapido");
    }

    pid_t h2 = fork();
    if (h2 == 0) {
        close(b[0]); close(a[0]); close(a[1]);
        hijo_escritor(b[1], 2, "lento");
    }

    close(a[1]);
    close(b[1]);

    int canal[2] = { a[0], b[0] };
    int max = (a[0] > b[0]) ? a[0] : b[0];
    for (int abiertos = 2; abiertos > 0; ) {
        fd_set lectura;
        FD_ZERO(&lectura);
        for (int c = 0; c < 2; c++)
            if (canal[c] != -1)
                FD_SET(canal[c], &lectura);

        if (select(max + 1, &lectura, NULL, NULL, NULL) == -1) {
            perror("select");
            break;
        }

        for (int c = 0; c < 2; c++) {
            if (canal[c] == -1 || !FD_ISSET(canal[c], &lectura))
                continue;
            char buffer[128];
            ssize_t n = read(canal[c], buffer, sizeof buffer);
            if (n > 0) {
                if (write(STDOUT_FILENO, buffer, n) == -1)
                    perror("write");
            } else {
                close(canal[c]);
                canal[c] = -1;
                abiertos--;
            }
        }
    }

    waitpid(h1, NULL, 0);
    waitpid(h2, NULL, 0);
    return 0;
}
