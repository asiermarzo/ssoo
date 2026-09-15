#include <stdio.h>

int main(void) {
    const int valores[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    const char *simbolos[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    int n;

    while (scanf("%d", &n) == 1) {
        for (int i = 0; i < 13; i++)
            while (n >= valores[i]) {
                printf("%s", simbolos[i]);
                n -= valores[i];
            }
        printf("\n");
    }

    return 0;
}
