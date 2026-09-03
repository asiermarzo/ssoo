/* extra_01 — conversor de permisos: octal <-> rwx
 *
 *   ./extra_01 640         ->  rw-r-----
 *   ./extra_01 rw-r-----   ->  640
 *
 * Cada terna de permisos (usuario, grupo, otros) es un digito octal de 3 bits.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void octal_a_texto(int octal, char texto[10]) {
    const char *bits = "rwx";
    for (int terna = 0; terna < 3; terna++) {
        int digito = (octal >> ((2 - terna) * 3)) & 7;
        for (int b = 0; b < 3; b++)
            texto[terna * 3 + b] = (digito & (4 >> b)) ? bits[b] : '-';
    }
    texto[9] = '\0';
}

static int texto_a_octal(const char *texto) {
    int octal = 0;
    for (int i = 0; i < 9; i++)
        if (texto[i] != '-')
            octal |= 1 << (8 - i);
    return octal;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <octal | rwxrwxrwx>\n", argv[0]);
        return 1;
    }

    if (strlen(argv[1]) == 9) {
        printf("%03o\n", texto_a_octal(argv[1]));
    } else {
        char texto[10];
        octal_a_texto((int) strtol(argv[1], NULL, 8), texto);
        printf("%s\n", texto);
    }
    return 0;
}
