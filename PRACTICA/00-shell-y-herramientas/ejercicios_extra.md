# P0 — Ejercicios extra

Cinco ejercicios adicionales para practicar C, la línea de comandos y el ciclo de compilación. Cada solución es un `.c` autocontenido que compila con `gcc -Wall -Wextra`. Compilar todos con `make`.

## 1. Conversor de permisos (`extra_01.c`)

Escribir un programa que traduzca permisos de fichero entre notación octal y notación simbólica `rwx`, en los dos sentidos, detectando automáticamente el formato de entrada.

```
./extra_01 640         ->  rw-r-----
./extra_01 rw-r-----   ->  640
```

Usar operadores de bits: cada terna (usuario, grupo, otros) es un dígito octal de 3 bits.

## 2. Mini `wc` (`extra_02.c`)

Programa que lee de la entrada estándar y cuenta líneas, palabras y caracteres, imitando a `wc`. Una palabra es una secuencia maximal de caracteres no blancos (`isspace`).

```
cat README.md | ./extra_02
```

## 3. Criba de Eratóstenes (`extra_03.c`)

Programa que reciba un entero `N` por argumento (`2 ≤ N < 1000000`) e imprima todos los primos menores o iguales que `N` usando la criba de Eratóstenes sobre un array reservado en la pila. Al final indica cuántos primos ha encontrado.

## 4. Triángulo de Pascal (`extra_04.c`)

Programa que reciba un número de filas por argumento (`1..30`) e imprima el triángulo de Pascal correspondiente. Cada fila se calcula a partir de la anterior sumando in situ, sin almacenar todo el triángulo.

## 5. Estadísticas de argumentos (`extra_05.c`)

Programa que reciba una lista indeterminada de enteros como argumentos de línea de comandos y muestre cuántos son, su suma, su media, el mínimo y el máximo. Practica el uso de `argc` / `argv` y `atoi`.

```
./extra_05 4 8 15 16 23 42
```
