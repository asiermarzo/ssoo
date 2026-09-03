# P8 — Ejercicios extra

Cinco ejercicios pequeños sobre planificación. Los tres primeros son **simulaciones** (sin procesos reales) para entender las políticas; los dos últimos usan señales para planificar procesos de verdad, como en la práctica. Compilar todos con `make`.

## 1. Round Robin (`extra_01.c`)

Simulador de Round Robin: dada una lista de ráfagas de CPU (llegada 0) y un quantum `Q` (por argumento), imprime el cronograma de ejecución y los tiempos de retorno y de espera, medios e individuales.

## 2. FCFS frente a SJF (`extra_02.c`)

Programa que, para la misma lista de ráfagas, calcule el tiempo de espera y de retorno medios con las políticas FCFS (orden de llegada) y SJF (ráfaga más corta primero) y compare los resultados.

## 3. Colas multinivel (`extra_03.c`)

Simulador de un sistema de tres niveles no apropiativos como el de la práctica: nivel 1 Round Robin (quantum 4), nivel 2 prioridades (menor valor = más prioridad), nivel 3 FCFS. Imprime el orden en que se sirven los procesos.

## 4. Turnos reales con `SIGSTOP` / `SIGCONT` (`extra_04.c`)

Programa que cree dos procesos hijo que ejecutan un bucle de trabajo y les reparta el procesador por turnos de 2 segundos, deteniéndolos y reanudándolos con `SIGSTOP` / `SIGCONT` (Round Robin real de dos procesos).

## 5. Mini-planificador con quantum por `alarm` (`extra_05.c`)

Programa que reciba varios comandos como argumentos, los lance detenidos y los ejecute por turnos de un quantum controlado con `alarm` + `SIGALRM`: al vencer el quantum detiene el proceso en curso (`SIGSTOP`) y pasa al siguiente, hasta que todos terminan.

```
./extra_05 "sleep 3" "sleep 4" "sleep 2"
```
