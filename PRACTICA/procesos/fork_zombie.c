#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main(void) {
    pid_t pid = fork();
    if (pid == 0) {
        printf("Hijo (%d): termino\n", getpid());
        exit(0);
    }
    printf("Padre (%d): duermo 30 s sin hacer wait; el hijo %d queda zombie\n", getpid(), pid);
    sleep(30);
    wait(NULL);
    return 0;
}
