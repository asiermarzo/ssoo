# P1 — Ejercicios extra

Cinco ejercicios adicionales sobre entrada/salida con **descriptores de fichero** (`open`, `read`, `write`, `close`, `lseek`, `unlink`). Cada solución es un `.c` autocontenido que compila con `gcc -Wall -Wextra`. Compilar todos con `make`.

## 1. Copia de ficheros (`extra_01.c`)

Programa `micopia origen destino` que copie un fichero usando exclusivamente llamadas al sistema, con un búfer en la pila. Debe gestionar los errores con `perror`, tratar las escrituras parciales (bucle hasta escribir todo el bloque leído) e informar al final del número de bytes copiados.

## 2. Contar líneas y bytes (`extra_02.c`)

Programa que reciba el nombre de un fichero por argumento, lo lea por bloques con `read` y cuente cuántos bytes tiene y cuántos saltos de línea (`\n`) contiene.

## 3. Volcado hexadecimal (`extra_03.c`)

Programa que imprima el contenido de un fichero al estilo `xxd`/`hexdump -C`: por cada 16 bytes, el desplazamiento en hexadecimal, los 16 bytes en hexadecimal y su representación ASCII (sustituyendo por `.` los no imprimibles).

## 4. Agenda de registros binarios (`extra_04.c`)

Programa que mantenga una agenda de personas en un fichero binario de registros de tamaño fijo (`struct { char nombre[32]; int edad; }`). Debe soportar tres órdenes:

```
./extra_04 agenda.dat add Ana 21
./extra_04 agenda.dat get 0        # usa lseek para saltar al registro 0
./extra_04 agenda.dat list
```

## 5. `tac`: fichero al revés (`extra_05.c`)

Programa que imprima las líneas de un fichero en orden inverso (la última primero), como el comando `tac`. Cargar el fichero en un búfer y recorrerlo de atrás hacia delante localizando los saltos de línea.
