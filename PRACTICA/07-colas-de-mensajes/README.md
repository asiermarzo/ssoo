# P7 — Colas de mensajes


Esta práctica usa la misma familia que [P6](../06-memoria-compartida-y-semaforos/): claves (`ftok`) y comandos (`ipcs`, `ipcrm`, `lsipc`). Como la memoria compartida y los semáforos, la cola **persiste** aunque terminen los procesos: hay que liberarla.

## `msgget` — crear / obtener la cola

```c
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
int msgget(key_t key, int msgflg);
```

- `msgflg`: `0600 | IPC_CREAT`, permisos y flag para crear la cola. Si ya existe la cola, se devuelve la existente. Si no se indica el flag y no existe la cola -> error.
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

- `msqid`: id de la cola.
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
- `msgtyp`: **tipo** del mensaje a retirar: `> 0` un tipo concreto; `0` cualquier tipo (el más antiguo de la cola); `< 0` el de tipo más bajo entre los que tienen `mtype <= |msgtyp|` (a igual tipo, el más antiguo).
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
        fprintf(stderr, "Uso: %s <proj_id>\n", argv[0]); 
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
        fprintf(stderr, "Uso: %s <proj_id> <nombre> <apellido> <edad> <telefono>\n", argv[0]); 
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

Si el `lector` se para (`SIGSTOP`) deja de hacer `msgrcv`, pero la cola sigue aceptando mensajes: con `ipcs -q` se ve cómo crece en bytes y en número de mensajes hasta que el lector se reanuda (`SIGCONT`) y la vacía de golpe. No hace falta un script, se puede ir paso a paso en la terminal:

```
$ killall lector #termina los lector que sigan en marcha
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
$ ipcs -q #muestra que los mensajes siguen en la cola
------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages
0x00000016 98304      asier      600        264          3

$ kill -CONT 4021 #mandar continuar al lector
Persona Numero1, 20 años, tel. 600000000
Persona Numero2, 20 años, tel. 600000000
Persona Numero3, 20 años, tel. 600000000

$ ipcs -q #la cola está vacía
------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages
0x00000016 98304      asier      600        0            0
```


## Ejercicios propuestos

1. **Un emisor y un receptor**. El proceso `./emisor proj_id periodo` genera un mensaje `mtype = 1` cada `periodo` segundos, el campo `numero` de `msg_t` se va incrementando en cada mensaje y lo encola en la cola identificada por `proj_id` y `/etc`; al mandar el décimo mensaje, encola además un mensaje de tipo `mtype = 999` para indicar al `receptor` que ha de terminar.

   El proceso `./receptor proj_id periodo` se encarga de crear la cola y repetidamente: retira el mensaje más antiguo (pedir con `msgtyp = 0`), lo imprime en consola y espera `periodo`. Cuando recibe un mensaje con `mtype = 999`, libera la cola y termina.

   ```mermaid
   flowchart LR
       E["./emisor 22 2"] -- msgsnd --> Q[(cola)]
       Q -- msgrcv --> R["./receptor 22 1"]
   ```

   ```c
   typedef struct {
       long mtype;
       int  numero;
   } msg_t;
   ```

   ```bash
   ./receptor 22 1 #en una terminal
   ./emisor   22 2 #en otra terminal

   pkill -x 'emisor|receptor'      # mata a los dos si no terminan solos (desde otra terminal)
   ```


2. **Varios productores, un consumidor**. Crea `productor.c` y `consumidor.c`. `productor` recibe estos argumentos (`./productor proj_id periodo`) y manda su PID en cada mensaje (en el campo `numero`). `consumidor` recibe (`./consumidor proj_id periodo`), retira un mensaje cada `periodo` segundos e imprime el PID que lleva. Lanza 3 `productor` y 1 `consumidor`.

   ```mermaid
   flowchart LR
       P1["./productor 22 2"] -- msgsnd --> Q[(cola)]
       P2["./productor 22 2"] -- msgsnd --> Q
       P3["./productor 22 2"] -- msgsnd --> Q
       Q -- msgrcv --> C["./consumidor 22 1"]
   ```

   Prueba primero con los 3 `productor` a `periodo = 2` y el `consumidor` a `periodo = 1`. La cola acumulará mensajes poco a poco: los 3 `productor` generan 1,5 mensajes/s y `consumidor` solo retira 1/s. Compruébalo con el comando `ipcs -q`, verás que `messages` va creciendo.

   Repite con los `productor` configurados con `periodo = 4` y comprueba que la cola no acumula mensajes.
   
   Repite con 3 `productor` (y periodos de 1, 2 y 3 respectivamente). Mira el orden de los PID que va imprimiendo `consumidor`: como todos los `productor` encolan con el mismo `mtype = 1` y `consumidor` siempre pide el más antiguo (`msgtyp = 0`), verás los mensajes en orden de encolado: el PID del `productor` con `periodo` más corto aparecerá más veces que el resto.


3. **Varios productores, dos consumidores**. Lanza otro `consumidor` (para tener 2). Comprueba con `ipcs -q` que ahora `messages` no crece.

   ```mermaid
   flowchart LR
       P1["./productor 22 2"] -- msgsnd --> Q[(cola)]
       P2["./productor 22 2"] -- msgsnd --> Q
       P3["./productor 22 2"] -- msgsnd --> Q
       Q -- msgrcv --> C1["./consumidor 22 1"]
       Q -- msgrcv --> C2["./consumidor 22 1"]
   ```

4. **Cola con dos prioridades**. `./generador proj_id periodo mtype` encola sin parar un mensaje del tipo `mtype` cada `periodo` segundos. `procesador` desencola cada 2 segundos e imprime el `mtype` de cada mensaje: primero los urgentes (`mtype = 1`) con `msgrcv(..., 1, IPC_NOWAIT)`; si no hay ninguno, coge uno cualquiera con `msgrcv(..., 0, 0)` (bloqueante). Los normales van con `mtype = 2`. Comprueba que los urgentes adelantan a los normales que ya llevaban rato esperando en la cola.

   ```bash
   ./procesador 22 &
   ./generador 22 1 2 &      # normales, uno por segundo
   ./generador 22 5 1 &      # urgentes, uno cada 5 s
   ```

   **Con una sola llamada**. `procesador` puede sustituir las dos llamadas por `msgrcv(..., -2, 0)`: retira el mensaje de tipo más bajo con `mtype <= 2`, es decir, un urgente si lo hay y, si no, el normal más antiguo; si la cola está vacía, se bloquea. Modifica `procesador` para que use esta llamada y comprueba que funciona de forma similar.

<!-- 
5. **Servidor con una única cola para varios clientes**: cada proceso `cliente` manda su mensaje con `mtype = 1` e incluye su propio PID en el campo `numero`.
El proceso `servidor` recibe con `msgrcv(..., 1, 0)` y responde a cada mensaje en la misma cola usando como `mtype` el PID del cliente. Cada `cliente`, tras enviar, espera su respuesta con `msgrcv(..., getpid(), 0)`. Lanza varios `cliente` a la vez y comprueba que, aunque sus peticiones y respuestas van en la misma cola, cada cliente solo recoge sus respuestas. Puedes comprobarlo haciendo que cada cliente imprima su pid y el pid que hay en `numero` (deberían siempre coincidir).
-->