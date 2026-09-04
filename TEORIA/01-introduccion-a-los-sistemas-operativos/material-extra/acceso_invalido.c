/* acceso_invalido.c — escribe muy lejos del final de un array de la pila.
 * El indice cae fuera de toda pagina valida del proceso: la MMU lanza un
 * fallo de pagina y el SO termina SOLO a este proceso (SIGSEGV), sin tocar
 * al resto del sistema. Sin proteccion de memoria, un acceso asi podria
 * corromper cualquier cosa y dejar la maquina en un estado de error.
 *
 *   gcc -Wall -Wextra -O0 -o acceso_invalido acceso_invalido.c
 *   ./acceso_invalido        -> "Segmentation fault (core dumped)"  (SIGSEGV)
 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int v[4] = {0};
    int i = (argc > 1) ? atoi(argv[1]) : 1000000;

    printf("escribiendo en v[%d] (el array solo tiene 4 elementos)...\n", i);
    v[i] = 42;                              /* -> SIGSEGV */
    printf("esto ya no se imprime: %d\n", v[i]);
    return 0;
}
