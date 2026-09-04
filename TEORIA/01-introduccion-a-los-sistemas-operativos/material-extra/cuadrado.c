/* cuadrado.c — ejemplo minimo para Compiler Explorer (godbolt.org).
 * Pegar en https://godbolt.org con el compilador "x86-64 gcc" y opciones -O0
 * para ver la llamada a subrutina: call / ret, paso de argumento en un
 * registro y valor de retorno en eax.
 *
 *   gcc cuadrado.c -Wall -Wextra -O0 -S -o cuadrado.s   # generar el ensamblador
 */
int cuadrado(int x) {
    return x * x;
}

int main(void) {
    return cuadrado(7);
}
