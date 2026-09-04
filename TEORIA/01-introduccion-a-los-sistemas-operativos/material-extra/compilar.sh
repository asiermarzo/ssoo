#!/usr/bin/env bash
# Compila los ejemplos en C de este material extra.
set -e
cd "$(dirname "$0")"

gcc -Wall -Wextra -O0 -o acceso_invalido acceso_invalido.c
gcc -Wall -Wextra -O0 -o division_cero  division_cero.c
gcc -Wall -Wextra -O0  -o direcciones   direcciones.c
gcc -Wall -Wextra -O2  -o primos_cpu    primos_cpu.c
gcc -Wall -Wextra      -o hola          hola.c

echo "Listo. Binarios: acceso_invalido, division_cero, direcciones, primos_cpu, hola"
