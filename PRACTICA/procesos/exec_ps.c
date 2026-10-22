#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main(void) {
    char *args[] = { "ps", "-aux", NULL };
    execvp(args[0], args);
    return 0;   /* nunca se alcanza si exec tiene éxito */
}
