/* extra_03 — emisor / receptor por tuberia con nombre (FIFO)
 *
 *   ./extra_03 receptor            (terminal 1)
 *   ./extra_03 emisor "hola"       (terminal 2)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>

#define RUTA_FIFO "/tmp/extra_fifo_p3"

static void limpiar(int sig) {
    (void) sig;
    unlink(RUTA_FIFO);
    _exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso:\n  %s receptor\n  %s emisor <mensaje>\n",
                argv[0], argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "receptor") == 0) {
        if (mkfifo(RUTA_FIFO, 0660) == -1 && errno != EEXIST) {
            perror("mkfifo");
            return 1;
        }
        signal(SIGINT, limpiar);
        printf("receptor: escuchando en %s (Ctrl-C para salir)\n", RUTA_FIFO);
        for (;;) {
            int fd = open(RUTA_FIFO, O_RDONLY);
            if (fd == -1) {
                perror("open");
                return 1;
            }
            char buffer[256];
            ssize_t n = read(fd, buffer, sizeof buffer - 1);
            if (n > 0) {
                buffer[n] = '\0';
                printf("recibido: %s\n", buffer);
            }
            close(fd);
        }
    }

    if (strcmp(argv[1], "emisor") == 0 && argc == 3) {
        int fd = open(RUTA_FIFO, O_WRONLY);
        if (fd == -1) {
            perror("open (arranca antes el receptor)");
            return 1;
        }
        if (write(fd, argv[2], strlen(argv[2])) == -1)
            perror("write");
        close(fd);
        return 0;
    }

    fprintf(stderr, "argumentos no validos\n");
    return 1;
}
