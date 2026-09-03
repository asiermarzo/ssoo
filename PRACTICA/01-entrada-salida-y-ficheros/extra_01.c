/* extra_01 — micopia: copia un fichero con open/read/write
 *
 *   ./extra_01 origen destino
 */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <origen> <destino>\n", argv[0]);
        return 1;
    }

    int fd_in = open(argv[1], O_RDONLY);
    if (fd_in == -1) {
        perror("open origen");
        return 1;
    }

    int fd_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out == -1) {
        perror("open destino");
        close(fd_in);
        return 1;
    }

    char buffer[4096];
    long total = 0;
    for (ssize_t n; (n = read(fd_in, buffer, sizeof buffer)) > 0; ) {
        for (ssize_t escritos = 0; escritos < n; ) {
            ssize_t w = write(fd_out, buffer + escritos, n - escritos);
            if (w == -1) {
                perror("write");
                close(fd_in);
                close(fd_out);
                return 1;
            }
            escritos += w;
        }
        total += n;
    }

    close(fd_in);
    close(fd_out);
    printf("%ld bytes copiados\n", total);
    return 0;
}
