# IPC: señales

## Contenidos

- Concepto de señal: forma limitada de IPC.
- Origen de una señal: excepción hardware, evento del kernel u otro proceso (`kill`).
- Señales estándar, su acción por defecto y cuáles se pueden capturar o ignorar.
- Envío de señales desde bash: `kill`, `killall`, `pkill`.
- Envío de señales: `kill()`, incluido a uno mismo.
- Manejo de señales: `signal()`.
- Espera de señales: `pause()`, `sleep()`.
- Ejemplo: distinguir con `SIGCHLD` si un hijo ha muerto o solo se ha pausado.
- Qué no hacer dentro de un manejador: funciones no reentrantes, `malloc`/`free`, `exit()`.

## Concepto de señal

Una señal no transporta un flujo de datos: notifica de forma asíncrona que ha ocurrido un determinado evento. Asíncrono quiere decir que el proceso no debe estar esperando activamente a que llegue la señal, instala un manejador (cuando llegue está señal -> haz esto) pero sigue haciendo otras cosas. Cuando llega la señal, el proceso interrumpe lo que está haciendo, ejecuta una acción breve (el manejador) y continúa donde estaba.

Una señal puede originarse en tres sitios distintos:

- **Una excepción del hardware**: la CPU detecta una división por cero o un acceso a memoria inválido y el kernel la traduce en `SIGFPE` o `SIGSEGV` para el proceso que la causó.
- **Un evento que detecta el kernel**: un hijo termina (`SIGCHLD`), se agota un temporizador (`SIGALRM`), se escribe en una tubería sin lectores (`SIGPIPE`).
- **Otro proceso, o el propio proceso** (`kill(getpid(), ...)` para enviársela a uno mismo), mediante `kill()` — o el usuario, tecleando `Ctrl+C` o ejecutando `kill` desde la shell.

En los tres casos, el proceso destino la recibe de la misma forma: no distingue si el aviso viene del hardware, del kernel o de otro proceso.

## Tabla de señales más comunes

| Señal | Valor | Acción por defecto | ¿Capturable? |
|-------|-------|--------------------|--------------|
| `SIGINT`  | 2  | Term | Sí |
| `SIGFPE`  | 8  | Core | Sí |
| `SIGKILL` | 9  | Term | **No** |
| `SIGSEGV` | 11 | Core | Sí |
| `SIGPIPE` | 13 | Term | Sí |
| `SIGALRM` | 14 | Term | Sí |
| `SIGTERM` | 15 | Term | Sí |
| `SIGUSR1` | 10 | Term | Sí |
| `SIGUSR2` | 12 | Term | Sí |
| `SIGCHLD` | 17 | Ign  | Sí |
| `SIGCONT` | 18 | Cont | Sí |
| `SIGSTOP` | 19 | Stop | **No** |

Acciones por defecto: **Term** (terminar), **Ign** (ignorar), **Core** (terminar + volcado de memoria), **Stop** (pausar), **Cont** (continuar si estaba pausado). Más información: `man 7 signal`.

`SIGKILL` y `SIGSTOP` son las únicas señales que **no** se pueden capturar, bloquear ni ignorar: el kernel necesita un mecanismo que funcione siempre, incluso si el proceso tiene un manejador con errores, está en bucle infinito o simplemente decide ignorar todo lo demás. El resto de señales, aunque tengan una acción por defecto drástica (como `Core` o `Term`), admiten instalar un manejador propio — capturar `SIGSEGV` (acceso a memoria inválido: puntero nulo, desreferencia de puntero corrupto, desbordamiento de pila...) o `SIGFPE` (error aritmético: división entera entre cero, overflow de división) es técnicamente posible, pero no hay garantía de poder continuar la ejecución con normalidad tras el fallo que las originó.

## Entrega de una señal

Cuando llega una señal, primero se comprueba si está **bloqueada** por la máscara de señales del proceso. Si lo está, queda **pendiente** hasta que se desbloquea; si no está bloqueada, se aplica: la **acción por defecto** de la tabla anterior, se **descarta** si está puesta a `SIG_IGN`, o se ejecuta el **manejador** instalado y el proceso se reanuda donde estaba al terminar. Bloquear una señal no implica descartarla, solo retrasa su entrega.


Las señales **no se acumulan**: si una señal ya está pendiente y llega otra señal igual antes de desbloquearse, solo queda registrada una vez. Por eso un manejador de `SIGCHLD` no puede asumir "una señal, un hijo": si terminan varios hijos casi a la vez, puede que solo llegue un único `SIGCHLD` para todos ellos, y el manejador debe recogerlos todos en un bucle en lugar de asumir que hay exactamente uno esperando.

## De `Ctrl+C` a `SIGINT`

Al pulsar `Ctrl+C`, la terminal solicita al kernel que envíe `SIGINT` al grupo de procesos en primer plano. Por eso basta con instalar un manejador de `SIGINT` para que un programa ignore `Ctrl+C` y decida cuándo y cómo cerrarse en su lugar.

## Enviar señales desde bash

```bash
kill -l                       # lista los nombres de señal disponibles
kill -TERM 1234                # o kill -15 1234: pide un cierre ordenado
kill -9 1234                    # SIGKILL por número: terminación forzosa
kill -STOP 1234                  # lo pausa, como Ctrl+Z pero sin terminal
kill -CONT 1234                   # lo reanuda
killall firefox                    # a todas las instancias por nombre
pkill -f "python.*servidor"          # por patrón de línea de comandos
```

`kill` sin más opciones envía `SIGTERM`. El nombre puede darse con o sin el prefijo `SIG` (`kill -TERM` o `kill -SIGTERM`); el número es la alternativa cuando el nombre no está disponible en el script.

## Capturar señales en C: `signal()`

```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void manejador(int senal) {
    printf("recibida la señal %d, sigo vivo\n", senal);
}

int main(void) {
    signal(SIGUSR1, manejador); //también vale signal(SIGUSR1, &manejador);
    for (;;) {
        pause(); //duerme hasta que llegue una señal
    }
}
```

El manejador se instala **una sola vez**; no hace falta reinstalarlo tras cada señal recibida. Se prueba desde otra terminal con `kill -USR1 <pid>`.

## Captura avanzada: `sigaction()`

`signal()` basta para casos simples, pero su comportamiento exacto varía entre sistemas Unix (qué otras señales se bloquean mientras corre el manejador, si hay que reinstalarlo, etc.). `sigaction()` sustituye esa ambigüedad por un control explícito:

```c
struct sigaction {
    void     (*sa_handler)(int);
    sigset_t   sa_mask;      /* señales que se bloquean mientras se ejecuta el manejador */
    int        sa_flags;
};
```

- `sa_mask`: además de la propia señal, qué otras quedan bloqueadas durante el manejador.
- `sa_flags` (OR de las que interesen): `SA_RESTART` (reinicia automáticamente la llamada al sistema interrumpida por la señal), `SA_NOCLDSTOP` (con `SIGCHLD`, no generar la señal cuando un hijo se pausa o se reanuda, solo cuando termina), `SA_NODEFER` (no bloquear la propia señal mientras se atiende).

`SA_NOCLDSTOP` es la pieza clave para `SIGCHLD`: por defecto (sin ese flag) el kernel manda `SIGCHLD` tanto si un hijo **termina** como si se **pausa** (`SIGSTOP`) o se **reanuda** (`SIGCONT`); activando `SA_NOCLDSTOP`, solo se avisa de la terminación.

## Ejemplo: distinguir con `SIGCHLD` si un hijo ha muerto o se ha pausado

Dos hijos, cada uno con un desenlace distinto: uno se mata a sí mismo (`kill(getpid(), SIGKILL)`) y el otro se pausa a sí mismo (`kill(getpid(), SIGSTOP)`). El padre instala un único manejador de `SIGCHLD` con `sigaction` y usa `waitpid` para averiguar, en cada aviso, cuál de las dos cosas ha pasado.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void manejador_sigchld(int senal) {
    int estado;
    pid_t pid;
    while ((pid = waitpid(-1, &estado, WNOHANG | WUNTRACED)) > 0) {
        if (WIFEXITED(estado)) {
            printf("[padre] hijo %d terminó con código %d\n", pid, WEXITSTATUS(estado));
        } else if (WIFSIGNALED(estado)) {
            printf("[padre] hijo %d murió por la señal %d\n", pid, WTERMSIG(estado));
        } else if (WIFSTOPPED(estado)) {
            printf("[padre] hijo %d se ha pausado por la señal %d\n", pid, WSTOPSIG(estado));
        }
    }
}

int main(void) {
    struct sigaction accion = {0};
    accion.sa_handler = manejador_sigchld;
    sigemptyset(&accion.sa_mask);
    accion.sa_flags = SA_RESTART;      /* sin SA_NOCLDSTOP: también avisa de pausas */
    sigaction(SIGCHLD, &accion, NULL);

    pid_t hijo_muere = fork();
    if (hijo_muere == 0) {
        sleep(1);
        kill(getpid(), SIGKILL);       /* el hijo se mata a sí mismo */
    }

    pid_t hijo_pausa = fork();
    if (hijo_pausa == 0) {
        sleep(2);
        kill(getpid(), SIGSTOP);       /* el hijo se pausa a sí mismo */
        pause();
        exit(0);
    }

    sleep(3);
    printf("[padre] termino; el hijo %d sigue pausado, lo mato\n", hijo_pausa);
    kill(hijo_pausa, SIGKILL);
    sleep(1);
    return 0;
}
```

**Un error habitual, que parece un fallo del sistema y no lo es**: si el manejador llama a `waitpid(-1, &estado, WNOHANG)` sin `WUNTRACED`, el `SIGCHLD` de la pausa **llega igualmente** (mientras no se active `SA_NOCLDSTOP`), pero `waitpid` no devuelve nada para el hijo pausado — porque por defecto `waitpid` solo informa de terminaciones. Sin `WUNTRACED` parece que el programa no distingue muerte de pausa; en realidad falta pedirlo explícitamente con ese flag.

## Espera de señales

```c
#include <unistd.h>

unsigned int sleep(unsigned int segundos);   /* espera o hasta recibir una señal */
int pause(void);                             /* bloquea hasta recibir una señal */
```

`pause()` es la forma correcta de esperar una señal sin **espera activa** (sin hacer un bucle comprobando una variable una y otra vez). En variables compartidas con el manejador se usa `volatile sig_atomic_t`, el único tipo cuyo acceso está garantizado como atómico frente a una señal.

## Qué no hacer dentro de un manejador

Una señal puede interrumpir al proceso en **cualquier punto**, incluida mitad de una llamada a `malloc` o de un `printf`. El manejador se ejecuta con lo que hubiera a medio hacer en ese instante, así que solo debe llamar a funciones **reentrantes / async-signal-safe**: funciones que no dejan estado a medias ni comparten estructuras internas (buffers, *locks*) con el resto del programa. La lista exacta está en `man 7 signal-safety`; las más relevantes para estas prácticas:

- **`malloc` / `free` / `realloc` no son async-signal-safe**: usan estructuras internas (el *heap*) protegidas con un *lock*. Si la señal interrumpe al proceso justo cuando ya tenía ese *lock* tomado (por ejemplo dentro de otro `malloc`), el manejador que vuelve a llamar a `malloc` se queda bloqueado esperando un *lock* que nunca se libera: **interbloqueo**.
- **`printf` y el resto de E/S con buffer de `stdio` tampoco lo son**, porque por debajo usan buffers internos y a veces `malloc`. Los ejemplos de esta teoría usan `printf` dentro del manejador por sencillez pedagógica; en código real se preferiría `write(2)`, que sí es async-signal-safe, sobre el descriptor directamente.
- **`exit()` tampoco lo es**: ejecuta las funciones registradas con `atexit`/`on_exit` y vacía los búferes de `stdio`, lo que puede reentrar en código no seguro. Dentro de un manejador se usa `_exit()`, que termina el proceso sin ese cierre ordenado.
- **Cuidado con `errno`**: si el manejador llama a alguna función que pueda modificarlo, debe guardar su valor al entrar y restaurarlo al salir, porque si no puede corromper el `errno` que estaba comprobando el código interrumpido.
- **El manejador debe ser corto**: mientras se ejecuta, la señal que lo disparó (y las que se hayan añadido a `sa_mask`) están bloqueadas; un manejador largo retrasa su propia recepción y la de esas señales.
- Para comunicar información al resto del programa, basta una variable `volatile sig_atomic_t` (vista arriba); no hace falta ninguna de las funciones anteriores.
