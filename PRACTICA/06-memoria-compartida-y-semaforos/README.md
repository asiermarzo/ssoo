# P6 — Memoria compartida y semáforos

## Descripción general

La memoria compartida permite que dos o más procesos accedan a una misma zona de memoria directamente. Los semáforos se usan para sincronizar ese acceso. Exclusión mutua y condiciones de carrera: ver [`TEORIA/10`](../../TEORIA/10-memoria-compartida-y-mutex/).

Esta práctica usa la interfaz **System V IPC**. Los semáforos y la memoria compartida **sobreviven a la muerte de los procesos que los crean**: hay que liberarlos siempre.

## Comandos comunes

`ipcs` (lista los recursos IPC: memoria compartida, semáforos, colas de mensajes), `ipcrm` (elimina un recurso IPC por id), `lsipc`.

## Claves: `ftok`

Memoria compartida, semáforos y colas de mansajes utilizan key_t (clave) para identificarse. Se pueden crear a partir de un path y un id de proyecto (entero), normalmente utilizamos `/etc` como path y 22 (u otro numero especificado por el usuario) como proj_id.

```c
#include <sys/types.h>
#include <sys/ipc.h>
key_t ftok(const char *path, int proj_id);
```

Convierte una ruta de fichero existente y un entero en una `key_t` (un `long`). Todos los procesos que comparten el recurso deben usar el **mismo fichero y el mismo entero**. Alternativa: `IPC_PRIVATE` como clave, el creador debe comunicar el id resultante a los que quieran usarlo.

Cada tipo de recurso (memoria compartida, semáforos, colas de mensajes, ver [P7](../07-colas-de-mensajes/)) tiene su propio espacio de claves, así que **la misma clave se puede usar a la vez para un segmento de memoria, un conjunto de semáforos y una cola sin que se pisen**. Lo habitual es usar una única clave para todos los recursos de un mismo programa. Lo que no se puede es tener dos recursos **del mismo tipo** con la misma clave: el segundo `get` devuelve el ya existente (o falla con `EEXIST` si se usa `IPC_CREAT | IPC_EXCL`).

## Memoria compartida

### Crear — `shmget`

```c
#include <sys/ipc.h>
#include <sys/shm.h>
int shmget(key_t key, size_t size, int shmflg);
```

- `key`: clave (de `ftok` o `IPC_PRIVATE`).
- `size`: tamaño del segmento en bytes.
- `shmflg`: permisos (`0640`, `SHM_R`, `SHM_W`) y opciones: `IPC_CREAT` (crea si no existe), `IPC_EXCL` (con `IPC_CREAT`, falla si ya existe).
- **Devuelve** el identificador del segmento, o `-1` y `errno` si hay error.

Crear memoria compartida para 100 enteros
```c
key_t clave = ftok("/etc", 33);
int shmid = shmget(clave, 100 * sizeof(int), IPC_CREAT | 0600);
```

### Adjuntar — `shmat`

```c
#include <sys/shm.h>
void *shmat(int shmid, const void *shmaddr, int shmflg);
```

Asocia la memoria compartida al proceso y nos da un puntero a la memoria.

- `shmid`: identificador devuelto por `shmget`.
- `shmaddr`: normalmente `NULL` (para que elija dirección el SO).
- `shmflg`: normalmente `0`; `SHM_RDONLY` para adjuntarlo solo lectura.

- **Devuelve** la dirección de comienzo del segmento, o `(void *) -1` y `errno` si hay error.

```c
int *enteros = (int *) shmat(shmid, NULL, 0);
enteros[5] = 10; /* ponemos la posición 6 a 10 */
```

Al terminar, el proceso desvincula automáticamente todo lo que tuviera adjuntado. Para desvincular manualmente se utiliza `shmdt()`

### Borrar — `shmctl`

```c
#include <sys/shm.h>
int shmctl(int shmid, int cmd, struct shmid_ds *buff);
```

Borra la memoria compartida (se hará efectivo cuando el último proceso que la usa se desvincula).

- `cmd`: `IPC_RMID` para borrarla; también admite `IPC_STAT` (lee la estructura de control) e `IPC_SET` (actualiza uid/gid/mode).
- **Devuelve** `0` o `-1` y `errno`.

```c
shmctl(shmid, IPC_RMID, NULL);
```

## Semáforos

Concepto de wait/signal, semáforos binarios/n-arios y las garantías que debe cumplir un semáforo: ver [`TEORIA/10`](../../TEORIA/10-memoria-compartida-y-mutex/#semáforos-wait-y-signal).

### Crear e inicializar — `semget` y `semctl`

```c
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
int semget(key_t key, int nsems, int semflg);
int semctl(int semid, int semnum, int cmd, ...);
```

- `semget` crea un array de `nsems` semáforos, sin inicializar su valor.
- `semctl` con `SETVAL` fija ese valor inicial; `semnum` es el índice del semáforo dentro del array.
- `nsems`: número de semáforos del array; en esta práctica basta con `1`.
- `semflg`: permisos en octal (`0640`) e `IPC_CREAT`.
- **Devuelven** el identificador del array (`semget`) o con error `-1` y `errno`.

Crear un semáforo con valor inicial 1 ("libre"):

```c
int id_sem = semget(clave, 1, 0640 | IPC_CREAT); /* crea 1 semáforo */
semctl(id_sem, 0, SETVAL, 1); /* fija el valor del primer semáforo (el único que hay) a 1 */
```

### Borrar — `semctl`

Elimina semáforos.

```c
semctl(id_sem, 0, IPC_RMID); //el segundo parámetro se ignora, se borran todos los semáforos asociados a id_sem
```

### Acquire — `semop`

Un **acquire** comprueba primero el valor del semáforo: si es 1 o más, le resta 1 y el proceso sigue; si ya vale 0, no resta nada y el proceso se queda bloqueado hasta que otro proceso haga un release (que sumará 1 al valor).

```c
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
int semop(int semid, struct sembuf *sops, unsigned nsops);

struct sembuf {
    unsigned short sem_num;   /* índice del semáforo */
    short          sem_op;    /* -1 = acquire, +1 = release */
    short          sem_flg;   /* 0, IPC_NOWAIT, SEM_UNDO */
};
```

`semop` (tanto en acquire como en release) **devuelve** `0`, con error `-1` y `errno`.

```c
struct sembuf accion = { .sem_num = 0, .sem_op = -1 };
semop(id_sem, &accion, 1);   /* acquire */
```

Aquí se pasa una sola operación (`nsops = 1`) 

<details> <summary> varias operaciones simulatáneas </summary>
pero `sops` puede ser un array de `struct sembuf` sobre distintos semáforos del mismo conjunto: `semop` las ejecuta **todas de forma atómica** (o se hacen todas o el proceso se bloquea sin hacer ninguna).

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

Los siguientes tres ejercicios son **sin semáforos**: usan memoria compartida pero no protegen el acceso concurrente.

1. Programa `memoria clave n` que cree una zona de memoria compartida para `n` enteros (`int compartidos[n]`) con la clave indicada, y quede en ejecución mostrando cada 5 segundos la suma de esos valores. Al pulsar `Ctrl-C`, destruye la memoria compartida y termina.

2. Programa `escribir clave pos valor` que acceda a la memoria compartida identificada por `clave` y haga `compartidos[pos] = valor`.

3. Programa `leer clave pos` que acceda a la memoria compartida `clave` y muestre `compartidos[pos]`.

4. Programa `banco clave` que cree una memoria compartida con 3 `long` (el saldo de 3 cuentas, inicializadas a 1000 cada una) y quede en ejecución mostrando cada segundo la suma de las tres cuentas: el total del banco, que debería mantenerse constante. Al pulsar `Ctrl-C`, destruye la memoria compartida y termina.

   Programa `transferencia clave` que elija al azar dos cuentas distintas y una cantidad, resta esa cantidad de una cuenta y la suma a la otra, **sin ninguna protección**. Lanza 100 copias a la vez:

   ```bash
   for i in $(seq 100); do ./transferencia clave & done
   ```

   y observa cómo el total que muestra `banco` deja de ser constante: al no ser atómica la transferencia (leer, restar, sumar, escribir), dos transferencias que tocan la misma cuenta a la vez pueden pisarse y perder dinero (o crearlo).

   Mejora el ejercicio con un semáforo: `banco` lo crea (junto con la memoria compartida) y lo borra al terminar; `transferencia` hace toda la operación —leer las dos cuentas, restar, sumar, escribir— dentro de un único `acquire`/`release`. Repite la prueba con 100 transferencias concurrentes y comprueba que ahora el total no cambia.

