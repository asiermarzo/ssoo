#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main(void) {
    int status = 0;
    pid_t childpid = fork();
    if (childpid == 0) { //hijo
        printf("Hijo (%d): espero 2s y termino(con 3) \n", getpid());
        sleep(2);
        exit(3);
    }
    //padre
    waitpid(childpid, &status, 0);
    printf("Padre (%d): hijo devolvió STATUS=%d\n", getpid(), status);
    return 0;
}
