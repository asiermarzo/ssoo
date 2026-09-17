#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int main(void) {
    for (int i = 0; i < 5; i++) {
        int pid = fork();
        if (pid == 0) {   /* hijo */
            printf("Hijo %d, padre %d\n", i, getppid());
            exit(0);
        }
    }
    return 0;
}
