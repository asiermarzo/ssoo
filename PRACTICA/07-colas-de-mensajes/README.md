# P7 — Colas de mensajes


Esta práctica usa la misma familia que [P6](../06-memoria-compartida-y-semaforos/): claves (`ftok`) y comandos (`ipcs`, `ipcrm`, `lsipc`). Como la memoria compartida y los semáforos, la cola **persiste** aunque terminen los procesos: hay que liberarla.

## `msgget` — crear / obtener la cola

```c
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
int msgget(key_t key, int msgflg);
```

- `msgflg`: `0666` | `IPC_CREAT` , permisos y flag para crear la cola. Si ya existe la cola, se devuelve la existente. Si no se indica el flag y no existe la cola -> error.
- **Devuelve** el identificador de la cola, o si hay error `-1` y `errno`.

## Estructura del mensaje

El mensaje **debe** ser una estructura cuyo primer campo sea un `long`:

```c
typedef struct {
    long mtype;           /* tipo de mensaje: entero positivo */
    char nombre[32];
    char apellido[32];
    int  edad;
    char telefono[16];
} msg_t;
```

## `msgsnd` — encolar un mensaje

```c
int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
```

- `msqid`: id de la cola
- `msgp`: puntero a donde empieza el mensaje.
- `msgsz`: tamaño del mensaje sin contar el long (`sizeof(msg_t) - sizeof(long)`).
- `msgflg`: `0` (bloquea hasta poder enviar; típicamente si la cola está llena) o `IPC_NOWAIT` (retorna siempre, con error si no puede enviar).
- **Devuelve** `0` o `-1` y `errno`.

## `msgrcv` — desencolar un mensaje

```c
ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
```
- `msqid`: id de la cola.
- `msgp`: puntero donde se copia el mensaje recibido.
- `msgsz`: tamaño máximo a copiar en `msgp`, sin contar el `long` (`sizeof(msg_t) - sizeof(long)`).
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
} msg_t;

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

    for (msg_t m;;) {
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
} msg_t;

int main(int argc, char *argv[]) {
    if (argc != 6) { 
        fprintf(stderr, "Uso: %s <valor_clave> <nombre> <apellido> <edad> <telefono>\n", argv[0]); 
        return 1; 
    }

    key_t clave = ftok("/etc", atoi(argv[1]));
    int id_cola = msgget(clave, 0600 | IPC_CREAT);

    msg_t m = { .mtype = 1, .edad = atoi(argv[4]) };
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

1. Productor/consumidor con cola de mensajes. `productor` genera un mensaje cada `periodo` segundos, numerados del 1 al 10, con `mtype = 1` y los encola; al mandar el décimo, encola además un mensaje de tipo `mtype = 999` para indicar al `consumidor` que ha de terminar. `consumidor` retira siempre el mensaje más antiguo (pedir con `msgtyp = 0`), lo imprime y espera `periodo` segundos antes de desencolar el siguiente. Cuando recibe un mensaje con `mtype = 999`, libera la cola (`msgctl` con `IPC_RMID`) y termina.

   ```c
   typedef struct {
       long mtype;
       int  numero;
       int id; //para ej2 y 4
   } msg_t;
   ```

   ```
   productor  clave periodo
   consumidor clave periodo
   ```

2. Añade a `productor` un argumento más, un identificador (`./productor clave periodo id`), este id se manda en cada mensaje; en `consumidor`, imprime junto a cada mensaje el id que lo mandó. Lanza cinco `productor` contra el mismo `consumidor`.

   Prueba primero con los cinco `productor` a `periodo = 3` y el `consumidor` a `periodo = 1`: el `consumidor` es más rápido que los `productor` (1 mensaje por segundo vs  1 cada 3 segundos), pero entre los cinco `productor` la cola acumulará mensajes poco a poco. Compruébalo con `lsipc`: si `MESSAGES` se queda siempre en 0 o 1, el `consumidor` va al día; si crece, se está quedando atrás.

   Repite con periodo de producción 5 segundos y comprueba que el consumidor puede procesar todos los mensajes de los productores.
   
   Repite con 3 productores (y periodos de 1, 2 y 3 respectivamente). Mira el orden de los `id` que va imprimiendo `consumidor`: como todos los `productor` encolan con el mismo `mtype = 1` y `consumidor` siempre pide el más antiguo (`msgtyp = 0`), lo que ves es el orden de llegada a la cola — el `id` con `periodo` más corto aparecerá bastantes más veces que el resto.

3. Cola con dos prioridades. `generador` manda un mensaje de tipo `1` (normal) cada segundo y, de vez en cuando (al azar, p. ej. 1 de cada 5), manda en su lugar una de tipo `2` (urgente). `procesador` intenta primero `msgrcv(..., 2, IPC_NOWAIT)`, si no hay mensaje urgente, entonces coge uno cualquiera `msgrcv(..., 0, 0)` (bloqueante). Comprueba que las tareas urgentes adelantan a las normales que ya llevaban rato esperando en la cola.

4. Servidor con una única cola para varios clientes: cada `cliente` manda su petición con un tipo fijo (p. ej. `mtype = 1`) e incluye su propio PID en el mensaje (por ejemplo en el campo `id`); `servidor` las recibe con `msgrcv(..., 1, 0)` y responde por la misma cola usando como `mtype` el PID del cliente. Cada `cliente`, tras enviar, espera su respuesta con `msgrcv(..., getpid(), 0)`. Lanza varios `cliente` a la vez y comprueba que, aunque sus peticiones y respuestas se entremezclen en la misma cola, cada uno solo recoge la suya.
