# P7 — Colas de mensajes


Esta práctica usa la misma familia que [P6](../06-memoria-compartida-y-semaforos/): claves (`ftok`) y comandos (`ipcs`, `ipcrm`, `lsipc`). Como la memoria compartida y los semáforos, la cola **persiste** aunque terminen los procesos: hay que liberarla.

## `msgget` — crear / obtener la cola

```c
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
int msgget(key_t key, int msgflg);
```

- `msgflg`: permisos (`0666`) OR `IPC_CREAT` (crea la cola si no existe; si no se indica y no existe, error).
- **Devuelve** el identificador de la cola, o si hay error `-1` y `errno`.



## Estructura del mensaje

El mensaje **debe** ser una estructura cuyo primer campo sea un `long` con el tipo:

```c
typedef struct {
    long mtype;           /* tipo de mensaje: entero positivo */
    char nombre[32];
    char apellido[32];
    int  edad;
    char telefono[16];
} msgbuf_t;
```

## `msgsnd` — encolar un mensaje

```c
int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
```

- `msqid`: id de la cola
- `msgp`: punteroa donde empieza el mensaje.
- `msgsz`: tamaño del mensaje sin contar el long (`sizeof(msgbuf_t) - sizeof(long)`).
- `msgflg`: `0` (bloquea hasta poder enviar; típicamente si la cola está llena) o `IPC_NOWAIT` (retorna de inmediato con error si no puede enviar).
- **Devuelve** `0` o `-1` y `errno`.

## `msgrcv` — desencolar un mensaje

```c
ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
```
- `msqid`: id de la cola.
- `msgp`: puntero donde se copia el mensaje recibido.
- `msgsz`: tamaño máximo a copiar en `msgp`, sin contar el `long` (`sizeof(msgbuf_t) - sizeof(long)`).
- `msgtyp`: **prioridad** del mensaje a retirar: `> 0` un tipo concreto; `0` cualquier tipo (el más antiguo de la cola).
- `msgflg`: `0` (bloquea hasta que haya un mensaje del tipo pedido) o `IPC_NOWAIT` (error inmediato si no lo hay).
- **Devuelve** el nº de bytes recibidos en la carga útil, o `-1` y `errno`.

## `msgctl` — control / liberación

```c
int msgctl(int msqid, int cmd, struct msqid_ds *buf);
```
Para liberar la cola: `cmd = IPC_RMID`, `buf = NULL`.

- **Devuelve** `0` o `-1` y `errno`.

## Ejemplo de uso

Dos programas que comparten cola a través de la misma clave (el valor pasado por argumento hace de `proj_id` de `ftok`). `lector` crea la cola y se queda recibiendo mensajes hasta que se pulsa `Ctrl-C`, momento en el que la destruye y termina; `escritor` manda un único mensaje con los datos de una persona.

```c
// lector.c — crea la cola, recibe mensajes y los muestra; Ctrl-C destruye la cola
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct {
    long mtype;
    char nombre[32];
    char apellido[32];
    int  edad;
    char telefono[16];
} msgbuf_t;

int id_cola;

void manejador_sigint(int senal) {
    msgctl(id_cola, IPC_RMID, NULL);
    printf("\ncola destruida\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) { 
        fprintf(stderr, "Uso: %s <valor_clave>\n", argv[0]); 
        return 1; 
    }

    key_t clave = ftok("/etc", atoi(argv[1]));
    id_cola = msgget(clave, 0600 | IPC_CREAT);
    signal(SIGINT, manejador_sigint);

    for (msgbuf_t m;;) {
        msgrcv(id_cola, &m, sizeof(m) - sizeof(long), 0, 0);
        printf("%s %s, %d años, tel. %s\n", m.nombre, m.apellido, m.edad, m.telefono);
    }
}
```

```c
// escritor.c — manda un mensaje con los datos de una persona
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct {
    long mtype;
    char nombre[32];
    char apellido[32];
    int  edad;
    char telefono[16];
} msgbuf_t;

int main(int argc, char *argv[]) {
    if (argc != 6) { 
        fprintf(stderr, "Uso: %s <valor_clave> <nombre> <apellido> <edad> <telefono>\n", argv[0]); 
        return 1; 
    }

    key_t clave = ftok("/etc", atoi(argv[1]));
    int id_cola = msgget(clave, 0600 | IPC_CREAT);

    msgbuf_t m = { .mtype = 1, .edad = atoi(argv[4]) };
    strcpy(m.nombre, argv[2]);
    strcpy(m.apellido, argv[3]);
    strcpy(m.telefono, argv[5]);
    msgsnd(id_cola, &m, sizeof(m) - sizeof(long), 0);
    return 0;
}
```

```bash
./lector 22 &
./escritor 22 Ana Garcia 30 645111222
./escritor 22 Luis Perez 21 633333444
```

Si el `lector` se para (`SIGSTOP`) deja de hacer `msgrcv`, pero la cola sigue aceptando mensajes: con `lsipc` se ve cómo crece en bytes y en número de mensajes hasta que el lector se reanuda (`SIGCONT`) y los vacía de golpe. No hace falta un script, se puede ir paso a paso en la terminal:

```
$ ./lector 22 &
[1] 4021
$ pgrep lector
4021
$ kill -STOP 4021 #pausamos al lector
```

Con el lector parado, se mandan varios mensajes y no aparece nada por pantalla (nadie los está sacando de la cola):

```
$ ./escritor 22 Persona Numero1 20 600000000
$ ./escritor 22 Persona Numero2 20 600000000
$ ./escritor 22 Persona Numero3 20 600000000
$ lsipc #muestra que los mensajes siguen en la cola
Message Queues:
KEY        MSQID      OWNER      PERMS      USED-BYTES   MESSAGES
0x00000016 98304      asier      600        201          3

$ kill -CONT 4021 #mandar continuar al lector
Persona Numero1, 20 años, tel. 600000000
Persona Numero2, 20 años, tel. 600000000
Persona Numero3, 20 años, tel. 600000000
$ lsipc           #la cola está vacía
Message Queues:
KEY        MSQID      OWNER      PERMS      USED-BYTES   MESSAGES
0x00000016 98304      asier      600        0            0
```


## Ejercicios propuestos

1. Productor/consumidor con cola de mensajes: `productor` genera un mensaje cada `periodo` segundos (numerados del 1 al 10, `mtype = 1`) y los encola; al mandar el décimo, encola además un mensaje de tipo `mtype = 2` como marca de fin. `consumidor` retira siempre el más antiguo (`msgtyp = 0`), lo imprime y tarda `periodo` segundos en "procesarlo" antes de pedir el siguiente (así el argumento marca tanto el ritmo de producción como el de consumo); en cuanto recibe uno de tipo `2` sabe que no llegarán más, libera la cola (`msgctl` con `IPC_RMID`) y termina.

   `sleep()` solo admite segundos enteros: si quieres poder pasar un `periodo` con decimales (p. ej. `0.25`), duerme con `usleep(periodo_en_segundos * 1000000)` (microsegundos) en su lugar — mismo `#include <unistd.h>`.

   ```
   productor  clave periodo
   consumidor clave periodo
   ```

2. Añade a `productor` un argumento más, un identificador (`productor clave periodo id`), y que lo incluya en cada mensaje; en `consumidor`, imprime junto a cada mensaje qué `id` lo mandó. Lanza cinco `productor` contra el mismo `consumidor`.

   Prueba primero con los cinco `productor` a `periodo = 1` y el `consumidor` a `periodo = 0.25` (con `usleep`, como en el ejercicio anterior): el `consumidor` por sí solo es mucho más rápido que cualquier `productor` (4 mensajes por segundo frente a 1), pero entre los cinco `productor` encolan 5 mensajes por segundo, más de los que el `consumidor` llega a retirar — la cola debería acumular mensajes poco a poco. Compruébalo con `lsipc`: si `MESSAGES` se queda siempre en 0 o 1, el `consumidor` va al día; si crece sin parar, se está quedando atrás.

   Repite con periodos de producción distintos (p. ej. 1, 2, 3, 4 y 5 segundos) y mira el orden de los `id` que va imprimiendo `consumidor`: como todos encolan con el mismo `mtype = 1` y `consumidor` siempre pide el más antiguo (`msgtyp = 0`), lo que ves es el orden real de llegada a la cola, no uno fijado por qué productor lo mandó — el `id` con `periodo` más corto aparecerá bastantes más veces que el resto.

3. Cola con dos prioridades: `generador` manda una tarea de tipo `1` (normal) cada segundo y, de vez en cuando (al azar, p. ej. 1 de cada 5), manda en su lugar una de tipo `2` (urgente). `procesador` intenta primero `msgrcv(..., 2, IPC_NOWAIT)`: si hay una tarea urgente la procesa; si no la hay (`errno == ENOMSG`), procesa la más antigua de cualquier tipo con `msgrcv(..., 0, 0)` (bloqueante). Comprueba que las tareas urgentes adelantan a las normales que ya llevaban rato esperando en la cola.

4. Servidor con una única cola para varios clientes: cada `cliente` manda su petición con un tipo fijo (p. ej. `mtype = 100`) e incluye su propio PID en la carga útil; `servidor` las recibe con `msgrcv(..., 100, 0)` y responde por la misma cola usando como tipo el PID que acaba de leer. Cada `cliente`, tras enviar, espera su respuesta con `msgrcv(..., getpid(), 0)`. Lanza varios `cliente` a la vez y comprueba que, aunque sus peticiones y respuestas se entremezclen en la misma cola, cada uno solo recoge la suya.
