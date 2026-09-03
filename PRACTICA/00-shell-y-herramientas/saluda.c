/* saluda.c — saluda al nombre recibido como argumento de linea de comandos.
 *
 *   gcc -Wall -Wextra -o saluda saluda.c
 *   ./saluda Ana        ->  Hola, Ana
 *   ./saluda            ->  mensaje de uso y codigo de salida 1
 */
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <nombre>\n", argv[0]);
        return 1;
    }
    printf("Hola, %s\n", argv[1]);
    return 0;
}
