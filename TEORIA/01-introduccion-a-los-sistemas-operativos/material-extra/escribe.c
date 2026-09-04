/* escribe.c — ejemplo minimo para ver una llamada al sistema en ensamblador.
 * En Compiler Explorer (godbolt.org, x86-64 gcc, -O2) se ve como write()
 * acaba en la instruccion 'syscall': una interrupcion software que pasa la
 * CPU a modo nucleo para que el SO haga la E/S.
 *
 *   gcc escribe.c -Wall -Wextra -O2 -static -S -o escribe.s
 *   objdump -d ./escribe | less     # buscar 'syscall'
 */
#include <unistd.h>

int main(void) {
    if (write(1, "hola\n", 5) != 5)
        return 1;
    return 0;
}
