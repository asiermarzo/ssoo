/* puntero_nulo.c — escribe a traves de un puntero NULL.
 *
 *   gcc puntero_nulo.c -g -Wall -o puntero_nulo
 *   ./puntero_nulo      -> "Segmentation fault (core dumped)" (SIGSEGV)
 */
#include <stdio.h>

int main(void) {
    int *p = NULL;

    printf("escribiendo en *p...\n");
    *p = 42;
    printf("esto ya no se imprime: %d\n", *p);
    return 0;
}
