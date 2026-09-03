# P2 — Ejercicios extra

Cinco ejercicios adicionales sobre `fork`, `exec`, `wait` e hilos POSIX. Cada solución es un `.c` autocontenido. Compilar todos con `make` (el ejercicio 5 se enlaza con `-pthread`).

## 1. Cadena de N procesos (`extra_01.c`)

Programa que reciba `N` por argumento y cree una cadena de `N` procesos: cada proceso es padre del siguiente (`P1 → P2 → … → PN`). Cada proceso imprime su nivel, su `pid` y su `ppid`, y espera (`waitpid`) a que termine su hijo antes de terminar él.

## 2. Cronometrar un comando (`extra_02.c`)

Programa `crono comando [args…]` que lance un comando en un proceso hijo con `execvp`, espere su terminación y muestre el tiempo real transcurrido (`clock_gettime(CLOCK_MONOTONIC)`) y el código de salida, distinguiendo con las macros `WIFEXITED` / `WIFSIGNALED` si terminó normalmente o por una señal.

## 3. Árbol binario de procesos (`extra_03.c`)

Programa que reciba una profundidad `D` (1..5) y cree un árbol binario de procesos de esa profundidad: cada proceso crea dos hijos hasta alcanzar `D`. Cada proceso imprime su nivel y espera a sus dos hijos. El total de procesos es `2^(D+1) − 1`.

## 4. Zombies y huérfanos (`extra_04.c`)

Programa que provoque de forma controlada los dos casos:

- Un **zombie**: el hijo termina de inmediato y el padre tarda 5 s en hacer `wait` (observable con `ps` como estado `Z`).
- Un **huérfano**: el padre termina antes que el hijo; el hijo comprueba con `getppid()` que ha sido adoptado (`ppid` pasa a 1, o al proceso `init`/subreaper).

## 5. Suma paralela con hilos (`extra_05.c`)

Programa que sume el array `1..1000000` repartiendo el trabajo entre `K` hilos (`K` por argumento, por defecto 4). Cada hilo acumula su suma parcial en su propia estructura (sin variables compartidas ni mutex) y el hilo principal combina los resultados tras `pthread_join`.
