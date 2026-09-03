# P9 — Ejercicios extra

Cinco ejercicios pequeños que ejercitan por separado las piezas del sistema concurrente de la práctica (tuberías con `fluido_t`, semáforos de recurso y de exclusión mutua, memoria compartida, cola de alertas por prioridad, apagado ordenado). Compilar todos con `make`.

Estructuras usadas, como en la práctica:

```c
typedef struct { long tipo; int pid; char texto[100]; } mensaje_t;
typedef struct { int contador, caudal; } fluido_t;
```

## 1. Cauce hidráulico con tuberías (`extra_01.c`)

Cauce `surtidor → caudalímetro → sumidero` con dos tuberías por las que circulan estructuras `fluido_t`. El surtidor genera `N` descargas de caudal aleatorio, el caudalímetro las mide y reenvía, y el sumidero acumula el caudal total.

## 2. Espera activa frente a semáforo (`extra_02.c`)

Programa que espera a que un proceso trabajador incremente un contador compartido hasta un objetivo. Con el argumento `activa` usa espera activa (bucle que quema CPU); sin él usa un semáforo (se bloquea sin consumir CPU). Al final compara el tiempo de CPU consumido por el proceso que espera. Ilustra por qué la práctica prohíbe las esperas activas.

```
./extra_02 activa
./extra_02
```

## 3. Monitor de capacidad y alertas (`extra_03.c`)

Réplica del `monitor` de la práctica: el proceso padre espera `SIGUSR1` (con `sigsuspend`) e imprime la capacidad guardada en memoria compartida; un proceso hijo lee la cola de alertas atendiendo a la prioridad (tipo 1 antes que tipo 2). Un tercer proceso simula la planta generando capacidad y alertas.

## 4. Depósito con semáforo de recurso (`extra_04.c`)

`llena_deposito` aporta litros a un semáforo contador; `surtidor` intenta descontar un volumen fijo con `IPC_NOWAIT` y, si no hay suficiente, encola una alerta de tipo 1 en la cola de mensajes. Al terminar se muestran las alertas acumuladas.

## 5. Apagado ordenado (`extra_05.c`)

Un `gestor` crea una memoria compartida, un grupo semafórico, una cola de mensajes y tres procesos hijo, y queda a la espera. Al recibir `Ctrl-C` termina ordenadamente: envía `SIGTERM` a los hijos, los espera y libera todos los recursos IPC (comprobable con `ipcs`).
