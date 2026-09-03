/* extra_01 — el hijo genera N aleatorios por una tuberia, el padre promedia
 *
 *   ./extra_01 10
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int n = (argc == 2) ? atoi(argv[1]) : 10;
    if (n < 1) {
        fprintf(stderr, "n debe ser >= 1\n");
        return 1;
    }

    int tuberia[2];
    if (pipe(tuberia) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t hijo = fork();
    if (hijo == -1) {
        perror("fork");
        return 1;
    }

    if (hijo == 0) {                       /* hijo: genera y envia */
        close(tuberia[0]);
        srand(getpid());
        for (int i = 0; i < n; i++) {
            int valor = rand() % 100;
            if (write(tuberia[1], &valor, sizeof valor) != (ssize_t) sizeof valor)
                _exit(1);
        }
        close(tuberia[1]);
        _exit(0);
    }

    close(tuberia[1]);                     /* padre: lee y promedia */
    long suma = 0;
    int recibidos = 0, valor;
    while (read(tuberia[0], &valor, sizeof valor) == (ssize_t) sizeof valor) {
        suma += valor;
        recibidos++;
    }
    close(tuberia[0]);
    wait(NULL);

    if (recibidos > 0)
        printf("%d valores, media = %.2f\n", recibidos, (double) suma / recibidos);
    return 0;
}
