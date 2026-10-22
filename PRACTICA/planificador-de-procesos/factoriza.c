// factoriza.c — urgente: factoriza n (64 bits) por división por tentativa; cuesta del orden de √n divisiones
// con el producto de dos primos parecidos hay que llegar hasta √n:
//   factoriza 10000004400000259        100000007 · 100000037       ~0,2 s
//   factoriza 1000000016000000063      1000000007 · 1000000009     ~3 s
//   factoriza 18446743979220271189     4294967279 · 4294967291     ~15 s
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long ahora_ms(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1000 + t.tv_nsec / 1000000;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <n>\n", argv[0]);
        return 1;
    }
    unsigned long long n = strtoull(argv[1], NULL, 10);
    long inicio = ahora_ms();
    printf("%llu =", n);
    for (; n % 2 == 0; n /= 2) {
        printf(" 2");
    }
    for (unsigned long long d = 3; d <= n / d; d += 2) {
        for (; n % d == 0; n /= d) {
            printf(" %llu", d);
        }
    }
    if (n > 1) {
        printf(" %llu", n);
    }
    // si le han quitado la CPU, el tiempo real será mayor que el de CPU
    printf("   (cpu %ld ms, real %ld ms)\n", clock() * 1000 / CLOCKS_PER_SEC, ahora_ms() - inicio);
    return 0;
}
