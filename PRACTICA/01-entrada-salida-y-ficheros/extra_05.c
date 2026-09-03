/* extra_05 — tac: imprime las lineas de un fichero en orden inverso
 *
 *   ./extra_05 fichero
 */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX (1 << 20)   /* 1 MiB */

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

    static char datos[MAX];
    long total = 0;
    for (ssize_t n; (n = read(fd, datos + total, MAX - total)) > 0; )
        total += n;
    close(fd);

    if (total == MAX) {
        fprintf(stderr, "Fichero demasiado grande (max %d bytes)\n", MAX);
        return 1;
    }

    long fin = total;
    for (long i = total - 1; i >= 0; i--) {
        if (datos[i] == '\n' && i != total - 1) {
            if (write(STDOUT_FILENO, datos + i + 1, fin - (i + 1)) == -1)
                perror("write");
            fin = i + 1;
        }
    }
    if (write(STDOUT_FILENO, datos, fin) == -1)
        perror("write");
    return 0;
}
