/* lee.c — abre un fichero para lectura sin comprobar si fopen ha tenido
 * exito: si el fichero no existe, fopen devuelve NULL y el programa casca
 * al usar ese puntero. El objetivo es ver con strace por que falla.
 *
 *   gcc lee.c -Wall -o lee
 *   ./lee
 */
#include <stdio.h>

int main(void) {
    FILE *f = fopen("datos.txt", "r");
    int c = fgetc(f);
    printf("Primer caracter: %c\n", c);
    fclose(f);
    return 0;
}
