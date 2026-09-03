# P4 — Ejercicios extra

Cinco ejercicios adicionales sobre señales: `sigaction`, `sigprocmask`, `sigpending`, `sigsuspend`, `alarm` y `SA_SIGINFO`. Cada solución es un `.c` autocontenido. Compilar todos con `make`.

## 1. Triple `Ctrl-C` (`extra_01.c`)

Programa que instale un manejador para `SIGINT` y solo termine cuando se haya pulsado `Ctrl-C` tres veces. Usar `volatile sig_atomic_t` para el contador y `pause` para esperar entre señales.

## 2. Cronómetro con `SIGALRM` (`extra_02.c`)

Programa que cuente segundos usando `alarm(1)` reprogramada dentro del manejador de `SIGALRM`. Imprime el tiempo transcurrido cada segundo y muestra el total al pulsar `Ctrl-C`.

## 3. Ping-pong de señales (`extra_03.c`)

Programa que cree un hijo e intercambie `N` rondas de señales: el padre envía `SIGUSR1` al hijo y este responde con `SIGUSR2`. Usa `sigprocmask` + `sigsuspend` para no perder ninguna señal por una condición de carrera.

## 4. Sección crítica y señales pendientes (`extra_04.c`)

Programa que bloquee `SIGINT` con `sigprocmask` durante una "sección crítica" de 10 segundos. Al terminar, comprueba con `sigpending` si el `SIGINT` llegó mientras estaba bloqueado y lo desbloquea (momento en que se entrega).

## 5. Quién envía la señal (`extra_05.c`)

Programa que instale un manejador para `SIGUSR1` con `SA_SIGINFO` y muestre el `pid` del proceso que la ha enviado (`siginfo_t::si_pid`). Termina tras recibir tres señales.

```
./extra_05 &
kill -USR1 <pid>
```
