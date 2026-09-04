/* direcciones.c — mapa de memoria de un proceso: codigo, pila y heap.
 *
 * Muestra la direccion de main() (codigo), de tres variables locales (pila)
 * y de tres bloques reservados con malloc() (heap). Se observa:
 *   - el codigo esta en direcciones bajas;
 *   - la pila esta en direcciones altas (0x7fff...);
 *   - el heap queda justo detras del codigo, muy lejos de la pila, y cada
 *     malloc() devuelve una direccion MAYOR que el anterior;
 *   pila y heap parten de extremos opuestos y crecen una hacia la otra.
 *
 *   gcc -Wall -Wextra -O0 -o direcciones direcciones.c
 *   ./direcciones
 */
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int a = 1;
    int b = 2;
    int c = 3;

    void *h1 = malloc(1024);
    void *h2 = malloc(1024);
    void *h3 = malloc(1024);

    printf("codigo   &main = %p\n\n", (void *) main);

    printf("pila     &a    = %p\n", (void *) &a);
    printf("pila     &b    = %p\n", (void *) &b);
    printf("pila     &c    = %p\n\n", (void *) &c);

    printf("heap     h1    = %p\n", h1);
    printf("heap     h2    = %p\n", h2);
    printf("heap     h3    = %p\n", h3);

    free(h1);
    free(h2);
    free(h3);
    return 0;
}
