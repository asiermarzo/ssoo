#!/usr/bin/env bash
# Compila los ejemplos en C de este material extra.
set -e
cd "$(dirname "$0")"

gcc acceso_invalido.c -Wall -Wextra -O0 -o acceso_invalido
gcc division_cero.c   -Wall -Wextra -O0 -o division_cero
gcc direcciones.c     -Wall -Wextra -O0 -o direcciones
gcc primos_cpu.c      -Wall -Wextra -O2 -o primos_cpu
gcc hola.c            -Wall -Wextra     -o hola

echo "Listo. Binarios: acceso_invalido, division_cero, direcciones, primos_cpu, hola"
