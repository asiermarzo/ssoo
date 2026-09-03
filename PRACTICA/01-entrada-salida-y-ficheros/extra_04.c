/* extra_04 — agenda de registros binarios de tamano fijo
 *
 *   ./extra_04 agenda.dat add Ana 21
 *   ./extra_04 agenda.dat get 0
 *   ./extra_04 agenda.dat list
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct {
    char nombre[32];
    int  edad;
} persona_t;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Uso:\n"
                        "  %s <fichero> add <nombre> <edad>\n"
                        "  %s <fichero> get <indice>\n"
                        "  %s <fichero> list\n", argv[0], argv[0], argv[0]);
        return 1;
    }

    const char *ruta = argv[1];
    const char *orden = argv[2];

    if (strcmp(orden, "add") == 0 && argc == 5) {
        int fd = open(ruta, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd == -1) { perror("open"); return 1; }
        persona_t p = {0};
        snprintf(p.nombre, sizeof p.nombre, "%s", argv[3]);
        p.edad = atoi(argv[4]);
        if (write(fd, &p, sizeof p) != (ssize_t) sizeof p) { perror("write"); close(fd); return 1; }
        close(fd);
        return 0;
    }

    if (strcmp(orden, "get") == 0 && argc == 4) {
        int fd = open(ruta, O_RDONLY);
        if (fd == -1) { perror("open"); return 1; }
        long indice = atol(argv[3]);
        if (lseek(fd, indice * (long) sizeof(persona_t), SEEK_SET) == -1) {
            perror("lseek");
            close(fd);
            return 1;
        }
        persona_t p;
        if (read(fd, &p, sizeof p) != (ssize_t) sizeof p) {
            fprintf(stderr, "No existe el registro %ld\n", indice);
            close(fd);
            return 1;
        }
        printf("%s (%d)\n", p.nombre, p.edad);
        close(fd);
        return 0;
    }

    if (strcmp(orden, "list") == 0 && argc == 3) {
        int fd = open(ruta, O_RDONLY);
        if (fd == -1) { perror("open"); return 1; }
        persona_t p;
        for (long i = 0; read(fd, &p, sizeof p) == (ssize_t) sizeof p; i++)
            printf("%ld: %s (%d)\n", i, p.nombre, p.edad);
        close(fd);
        return 0;
    }

    fprintf(stderr, "Orden no reconocida\n");
    return 1;
}
