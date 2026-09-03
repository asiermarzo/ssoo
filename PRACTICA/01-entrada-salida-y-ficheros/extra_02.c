/* extra_02 — cuenta lineas y bytes de un fichero leyendo por bloques
 *
 *   ./extra_02 fichero
 */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <fichero>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    char buffer[4096];
    long bytes = 0, lineas = 0;
    for (ssize_t n; (n = read(fd, buffer, sizeof buffer)) > 0; ) {
        bytes += n;
        for (ssize_t i = 0; i < n; i++)
            if (buffer[i] == '\n')
                lineas++;
    }
    close(fd);

    printf("%ld lineas, %ld bytes\n", lineas, bytes);
    return 0;
}
