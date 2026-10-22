#include <stdio.h>

int valor(char simbolo) {
    switch (simbolo) {
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
        default: return 0;
    }
}

int rom2dec(char *str) {
    int total = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        int actual = valor(str[i]);
        int siguiente = valor(str[i + 1]);
        total += actual < siguiente ? -actual : actual;
    }

    return total;
}

int main(void) {
    char romano[64];

    while (scanf("%63s", romano) == 1){
        int dec = rom2dec(romano);
        printf("%d\n", dec);
    }

    return 0;
}
