/* extra_02 — mini wc: cuenta lineas, palabras y caracteres de la entrada estandar
 *
 *   cat README.md | ./extra_02
 *
 * Una palabra es una secuencia maximal de caracteres no blancos.
 */
#include <stdio.h>
#include <ctype.h>

int main(void) {
    long lineas = 0, palabras = 0, caracteres = 0;
    int dentro_palabra = 0;

    for (int c; (c = getchar()) != EOF; ) {
        caracteres++;
        if (c == '\n')
            lineas++;
        if (isspace(c)) {
            dentro_palabra = 0;
        } else if (!dentro_palabra) {
            dentro_palabra = 1;
            palabras++;
        }
    }

    printf("%8ld %8ld %8ld\n", lineas, palabras, caracteres);
    return 0;
}
