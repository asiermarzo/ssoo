/* extra_03 — volcado hexadecimal de un fichero, al estilo hexdump -C
 *
 *   ./extra_03 fichero
 */
#include <stdio.h>
#include <ctype.h>
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

    unsigned char linea[16];
    long offset = 0;
    for (ssize_t n; (n = read(fd, linea, sizeof linea)) > 0; offset += n) {
        printf("%08lx  ", offset);
        for (int i = 0; i < 16; i++) {
            if (i < n)
                printf("%02x ", linea[i]);
            else
                printf("   ");
            if (i == 7)
                putchar(' ');
        }
        printf(" |");
        for (int i = 0; i < n; i++)
            putchar(isprint(linea[i]) ? linea[i] : '.');
        printf("|\n");
    }
    close(fd);
    return 0;
}
