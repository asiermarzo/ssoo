# P6 — Memoria compartida y semáforos

## Descripción general

La memoria compartida permite que dos o más procesos accedan a una misma zona de memoria directamente. Los semáforos se usan para sincronizar este acceso. Exclusión mutua y condiciones de carrera: ver [`TEORIA/10`](../../TEORIA/10-memoria-compartida-y-mutex/).

Esta práctica usa la interfaz **System V IPC** (`ftok`, claves e `ipcs`): los semáforos y la memoria compartida **sobreviven a la muerte de los procesos que los crean**, así que hay que liberarlos.

## Comandos comunes

`ipcs` (lista los recursos IPC: memoria compartida, semáforos, colas de mensajes), `ipcrm` (elimina un recurso IPC por id), `lsipc` (resumen de los recursos IPC en uso frente a los límites del sistema).

## Claves: `ftok`

Memoria compartida, semáforos y colas de mensajes utilizan `key_t` (clave) para identificarse. Se pueden crear a partir de un path y un id de proyecto (`int`); normalmente utilizamos `/etc` como path y `22` como `proj_id`.

```c
#include <sys/types.h>
#include <sys/ipc.h>
key_t ftok(const char *path, int proj_id);
```

Convierte una ruta de fichero existente y un entero en una `key_t` (un `int`). Todos los procesos que comparten el recurso deben usar el **mismo fichero y el mismo entero**. Alternativa: `IPC_PRIVATE` como clave; el creador debe comunicar el id resultante a los que quieran usarlo.

**La misma clave se puede usar a la vez para un segmento de memoria compartida, un conjunto de semáforos y una cola sin que se pisen**. Lo habitual es usar una única clave para todos los recursos de un mismo programa.

## Memoria compartida

### Crear — `shmget`

```c
#include <sys/ipc.h>
#include <sys/shm.h>
int shmget(key_t key, size_t size, int shmflg);
```

- `key`: clave (de `ftok` o `IPC_PRIVATE`).
- `size`: tamaño del segmento en bytes.
- `shmflg`: permisos en octal (`0600`) y opciones: `IPC_CREAT` (crea si no existe), `IPC_EXCL` (con `IPC_CREAT`, falla si ya existe).
- **Devuelve** el id del segmento, o `-1` y `errno` si hay error.

Crear memoria compartida para 100 enteros:

```c
key_t clave = ftok("/etc", 22);
int shmid = shmget(clave, 100 * sizeof(int), 0600 | IPC_CREAT);
```

### Adjuntar — `shmat`

```c
#include <sys/shm.h>
void *shmat(int shmid, const void *shmaddr, int shmflg);
```

Asocia la memoria compartida al proceso y nos da un puntero a la memoria.

- `shmid`: identificador devuelto por `shmget`.
- `shmaddr`: normalmente `NULL` (para que elija dirección el SO).
- `shmflg`: normalmente `0`; `SHM_RDONLY` para adjuntarlo en solo lectura.
- **Devuelve** la dirección de comienzo del segmento, o `(void *) -1` y `errno` si hay error.

```c
int *enteros = (int *) shmat(shmid, NULL, 0);
enteros[5] = 10; /* ponemos el sexto entero (índice 5) a 10 */
```

Al terminar, el proceso desvincula automáticamente todo lo que tuviera adjuntado. Para desvincular manualmente se utiliza `int shmdt(const void *shmaddr)`, pasándole la dirección que devolvió `shmat`.

### Borrar — `shmctl`

```c
#include <sys/shm.h>
int shmctl(int shmid, int cmd, struct shmid_ds *buf);
```

Borra la memoria compartida (se hará efectivo cuando el último proceso que la usa se desvincule).

- `shmid`: identificador de la memoria compartida (el que devuelve `shmget`).
- `cmd`: `IPC_RMID` para borrarla; también admite `IPC_STAT` (lee la estructura de control) e `IPC_SET` (actualiza uid/gid/mode).
- `buf`: estructura de control que se lee (`IPC_STAT`) o se escribe (`IPC_SET`); con `IPC_RMID`, `NULL`.
- **Devuelve** `0` o `-1` y `errno`.

```c
shmctl(shmid, IPC_RMID, NULL); /* borra la memoria compartida */
```

## Semáforos

Concepto de wait/signal, semáforos y las garantías que debe cumplir un semáforo: ver [`TEORIA/10`](../../TEORIA/10-memoria-compartida-y-mutex/#semáforos-wait-y-signal).

### Crear — `semget`

```c
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
int semget(key_t key, int nsems, int semflg);
```

- Crea un array de `nsems` semáforos (su valor está sin inicializar).
- `nsems`: número de semáforos del array; en esta práctica basta con `1`.
- `semflg`: permisos en octal (`0600`) e `IPC_CREAT`.
- **Devuelve** el identificador del array de semáforos, o `-1` y `errno` si hay error.

### Inicializar — `semctl`

```c
#include <sys/sem.h>
int semctl(int semid, int semnum, int cmd, ...);
```

- `semid`: identificador del array de semáforos (el que devuelve `semget()`).
- `semnum`: índice del semáforo dentro del array (empieza en `0`).
- `cmd`: operación a realizar; `SETVAL` fija el valor de un semáforo (`IPC_RMID` borra el array, ver abajo).
- `...`: argumento extra que depende de `cmd`; con `SETVAL`, el valor inicial.

Crear un semáforo y ponerlo a 1 ("verde - libre"):

```c
key_t clave = ftok("/etc", 22);
int id_sem = semget(clave, 1, 0600 | IPC_CREAT); /* crea 1 semáforo */
semctl(id_sem, 0, SETVAL, 1); /* fija el valor del primer semáforo (el único que hay) a 1 */
```

### Borrar — `semctl`

Elimina semáforos.

```c
semctl(id_sem, 0, IPC_RMID); /* el segundo parámetro se ignora: se borran todos los semáforos asociados a id_sem */
```

### Acquire — `semop`

Un **acquire** comprueba primero el valor del semáforo: si es 1 o más, le resta 1 y el proceso sigue; si vale 0, no resta nada y el proceso se queda bloqueado hasta que otro proceso haga un release (que sumará 1 al valor).

```c
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
int semop(int semid, struct sembuf *sops, size_t nsops);

struct sembuf {
    unsigned short sem_num;   /* índice del semáforo */
    short          sem_op;    /* -1 = acquire, +1 = release */
    short          sem_flg;   /* 0, IPC_NOWAIT, SEM_UNDO */
};
```

- `semid`: identificador del array de semáforos.
- `sops`: puntero a la operación (`struct sembuf`) o array de operaciones a realizar.
- `nsops`: número de operaciones que hay en `sops`.
- `sem_flg` (en `struct sembuf`): normalmente `0`; `IPC_NOWAIT` no bloquea (falla con `EAGAIN` si tuviera que esperar); `SEM_UNDO` hace que el kernel deshaga la operación si el proceso termina (útil si muere dentro de la sección crítica).
- **Devuelve** `0` o `-1` y `errno`.

```c
struct sembuf accion = { .sem_num = 0, .sem_op = -1 };
semop(id_sem, &accion, 1);   /* acquire */
```

Aquí se pasa una sola operación (`nsops = 1`).

<details> <summary> varias operaciones simultáneas </summary>

`sops` también puede ser un array de `struct sembuf` sobre distintos semáforos del mismo array: `semop` las ejecuta **todas de forma atómica** (o se hacen todas o el proceso se bloquea sin hacer ninguna).

```c
struct sembuf acciones[2] = {
    { .sem_num = 0, .sem_op = -1 },
    { .sem_num = 1, .sem_op = -1 },
};
semop(id_sem, acciones, 2);  /* acquire de los semáforos 0 y 1 a la vez */
```

</details>

### Release — `semop`

Un **release** libera el semáforo: suma 1 y, si había algún proceso bloqueado en un acquire, lo despierta.

```c
struct sembuf accion = { .sem_num = 0, .sem_op = 1 };
semop(id_sem, &accion, 1);   /* release: salir de la sección crítica */
```

## Ejercicios propuestos

Los siguientes tres ejercicios son **sin semáforos**: usan memoria compartida pero no sincronizan el acceso concurrente.

1. Programa `./memoria proj_id n` que cree una zona de memoria compartida para `n` enteros (`int compartidos[n]`) con la clave de `ftok("/etc", proj_id)`, y quede en ejecución mostrando cada 5 segundos la suma de esos valores. Al pulsar `Ctrl-C`, destruye la memoria compartida y termina.

2. Programa `./escribir proj_id pos valor` que acceda a la memoria compartida identificada por `proj_id` y haga `compartidos[pos] = valor`.

3. Programa `./leer proj_id pos` que acceda a la memoria compartida identificada por `proj_id` y muestre `compartidos[pos]`.

4. Programa `./banco proj_id` que cree una memoria compartida con 3 `long` (el saldo de 3 cuentas, inicializadas a 1000 cada una) y quede en ejecución mostrando cada 5 segundos el total del banco (la suma de las tres cuentas), que debería mantenerse constante. Al pulsar `Ctrl-C`, destruye la memoria compartida y termina.

   Programa `./transferencias proj_id` que 100000 veces seguidas (sin esperas) elige al azar dos cuentas (puede ser dos veces la misma) y una cantidad (de -100 a 100), resta esa cantidad de la primera cuenta y la suma a la otra. Lanza 10 copias a la vez:

   ```bash
   for i in $(seq 10); do ./transferencias 22 & done
   ```

   El total que muestra `banco` puede variar: cada `cuentas[a] -= x` es leer, restar y escribir; si dos procesos (en núcleos distintos) tocan la misma cuenta a la vez, una escritura pisa a la otra y se pierde (o se crea) dinero.

   Corrige el ejercicio con un semáforo: `banco` lo crea a 1 (junto con la memoria compartida) y lo borra al terminar; `transferencias` hace un `acquire` antes de realizar la transferencia y un `release` al terminar. `banco` también tendrá que hacer un `acquire` antes de calcular la suma, y un `release` al terminarla. Repite la prueba con transferencias concurrentes y comprueba que ahora el total no cambia (aunque tarda más: cada transferencia hace dos `semop`).

5. Programa `./seccion proj_id [valor_inicial]` que repite en bucle:

   1. Imprime `Entrando...` y hace un `acquire` en el semáforo.
   2. Cuando entra, imprime `Dentro. Para salir pulsa Intro` y espera a que se pulse Intro (`getchar()`).
   3. Hace un `release`.

   El proceso que recibe `valor_inicial` (es un parámetro opcional) crea el semáforo y lo inicializa a ese valor. Puedes destruirlo al recibir `Ctrl-C`. Comprueba si `semop` devuelve `-1` para saber si has podido entrar (por ejemplo, si el semáforo ya se ha borrado).

   ```bash
   ./seccion 22 1    # terminal 1 (lanzar primero): crea el semáforo a 1
   ./seccion 22      # terminal 2
   ./seccion 22      # terminal 3
   ```

   Con `valor_inicial=1` solo puede haber un proceso dentro a la vez. Repite con `valor_inicial=2` y comprueba que pueden estar 2 procesos.

