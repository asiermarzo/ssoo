# P3 — Ejercicios extra

Cinco ejercicios adicionales sobre tuberías sin nombre (`pipe`), tuberías con nombre (`mkfifo`), `dup2` y `select`. Cada solución es un `.c` autocontenido. Compilar todos con `make`.

## 1. Media por tubería (`extra_01.c`)

Programa que cree un hijo que genere `N` enteros aleatorios y los envíe al padre por una tubería. El padre los lee y calcula la media. `N` se pasa por argumento (por defecto 10).

## 2. Operador `|` (`extra_02.c`)

Programa que reproduzca `cmd1 | cmd2`: recibe dos comandos como cadenas, los trocea, crea una tubería y dos hijos, conecta con `dup2` la salida del primero a la entrada del segundo y ejecuta ambos con `execvp`.

```
./extra_02 "ls -l" "wc -l"
./extra_02 "ps ax" "grep bash"
```

## 3. FIFO emisor / receptor (`extra_03.c`)

Un único programa que actúa como receptor o como emisor según el primer argumento, comunicándose por una tubería con nombre (`/tmp/extra_fifo_p3`). El receptor crea la FIFO, la escucha en bucle y la borra al recibir `Ctrl-C`.

```
./extra_03 receptor            # terminal 1
./extra_03 emisor "hola"       # terminal 2
```

## 4. Multiplexado con `select` (`extra_04.c`)

Programa que cree dos hijos que escriben en sendas tuberías a ritmos distintos (uno cada segundo, otro cada dos segundos). El padre usa `select` para atender la tubería que tenga datos, sin bloquearse en una mientras la otra está lista, hasta que ambas se cierran.

## 5. Pipeline de tres procesos (`extra_05.c`)

Programa que construya el cauce `generador → filtro → impresor` con dos tuberías: el generador emite `1..N`, el filtro deja pasar solo los pares y el impresor los muestra por pantalla.
