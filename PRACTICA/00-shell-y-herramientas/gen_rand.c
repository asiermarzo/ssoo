#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int cuantos = argc > 1 ? atoi(argv[1]) : 16;
    int max = argc > 2 ? atoi(argv[2]) : 3999;
    int min = argc > 3 ? atoi(argv[3]) : 1;

    srand(time(NULL));
    for (int i = 0; i < cuantos; i++)
        printf("%d\n", min + rand() % (max - min + 1));

    return 0;
}
