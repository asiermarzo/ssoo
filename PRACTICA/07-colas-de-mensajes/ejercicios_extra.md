# P7 — Ejercicios extra

Cinco ejercicios adicionales sobre colas de mensajes **System V IPC** (`msgget`, `msgsnd`, `msgrcv`, `msgctl`). Cada solución es un `.c` autocontenido. Compilar todos con `make`. Recuerda liberar las colas (`ipcs -q`, `ipcrm -q`).

Todos los mensajes son una estructura cuyo primer campo es un `long mtype`. El tamaño que se pasa a `msgsnd`/`msgrcv` es `sizeof(mensaje_t) - sizeof(long)`.

## 1. Productor / consumidor (`extra_01.c`)

Programa que, según el rol del argumento, produzca o consuma 10 mensajes a través de una cola. El periodo (segundos por mensaje) se pasa como argumento. El consumidor elimina la cola al terminar.

```
./extra_01 <clave> productor 1
./extra_01 <clave> consumidor 2
```

## 2. Servidor de eco por tipos (`extra_02.c`)

Un servidor lee mensajes de tipo 1, pasa el texto a mayúsculas y responde usando como tipo de mensaje el `pid` del cliente; cada cliente envía su texto y espera la respuesta dirigida a su `pid`. El servidor libera la cola al recibir `Ctrl-C`.

```
./extra_02 servidor            # terminal 1
./extra_02 cliente "hola"      # terminal 2
```

## 3. Recepción selectiva (`extra_03.c`)

Programa que encole cuatro mensajes de tipos 2, 1, 3 y 1, y luego los extraiga demostrando el efecto del parámetro `msgtyp` de `msgrcv`: un tipo concreto (`> 0`), cualquiera (`0`) y el menor tipo menor o igual a un valor (`< 0`).

## 4. Varios productores, un consumidor (`extra_04.c`)

Programa que lance `P` productores (cada uno envía `M` mensajes con su `pid`) y un consumidor (el proceso principal) que recibe todos los mensajes y, al terminar, informa de cuántos ha recibido de cada `pid`.

## 5. Barrera de sincronización con una cola (`extra_05.c`)

Programa que lance `K` trabajadores; cada uno realiza una tarea de duración aleatoria y, al acabar, encola un mensaje. El proceso principal hace `K` `msgrcv` (uno por trabajador) a modo de barrera: no continúa hasta que todos han terminado.
