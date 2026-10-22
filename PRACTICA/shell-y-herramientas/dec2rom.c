#include <stdio.h>

int dec2rom(int dec, char *rom, int max_str_len) {
    const int valores[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    const char *simbolos[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    const int num_simbolos = sizeof(valores) / sizeof(valores[0]);
    int len = 0;

    for (int i = 0; i < num_simbolos; i++)
        while (dec >= valores[i]) {
            for (const char *c = simbolos[i]; *c != '\0'; c++) {
                if (len + 1 >= max_str_len)
                    return -1;
                rom[len++] = *c;
            }
            dec -= valores[i];
        }

    rom[len] = '\0';
    return len;
}

int main(void) {
    char rom[64];
    for (int n; scanf("%d", &n) == 1; ) {
        dec2rom(n, rom, sizeof(rom) );
        printf("%s\n", rom);
    }

    return 0;
}
