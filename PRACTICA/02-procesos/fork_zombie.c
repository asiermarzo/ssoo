#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main(void) {
    pid_t childpid = fork();
    if (childpid == 0) {
        printf("Hijo (%d): termino ya\n", getpid());
        exit(0);
    } else {
        printf("Padre (%d): duermo 60 s sin hacer wait; el hijo %d queda zombie\n", getpid(), childpid);
        sleep(60);
        wait(NULL);
    }
    return 0;
}
