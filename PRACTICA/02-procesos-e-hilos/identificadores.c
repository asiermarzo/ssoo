#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main(void) {
    printf("PID: %d\n",  getpid());
    printf("PPID: %d\n", getppid());
    printf("UID: %d\n",  getuid());
    return 0;
}
