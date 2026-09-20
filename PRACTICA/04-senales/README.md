# P4 — Señales

## Descripción general

Concepto, origen de las señales, tabla completa y cómo se entregan: ver [`TEORIA/08`](../../TEORIA/08-ipc-senales/). 

## Comandos bash comunes

el comando kill se utiliza para mandar **cualquier señal* a un proceso cuando se conoce su pid. Si no se indica el número de señal, se manda SIGTERM. 

```bash
kill -<número_señal> <pid>     # ej. kill -9 1234: envía la señal 9 (SIGKILL) al proceso 1234
kill -s <nombre_señal> <pid>   # ej. kill -s SIGUSR1 1234: envía SIGUSR1 al proceso 1234
killall <nombre_proceso>       # envía la señal (SIGTERM por defecto) a todos los procesos con ese nombre
```

## Función C: `kill`

Para mandar señales a otros procesos desde un programa en C se utiliza la función kill.

```c
#include <sys/types.h>
#include <signal.h>
int kill(pid_t pid, int sig);
```

Envía la señal `sig` al proceso `pid`, o un grupo de procesos. **Si**:
- `pid == 0`: a todos los procesos del grupo del proceso actual.
- `pid == -1`: a todos los procesos salvo `init`.
- `pid < -1`: a todos los del grupo `-pid`.
- `sig == 0`: no envía nada, sólo comprueba errores.
- **Devuelve** `0` si correcto, `-1` y `errno` si hay error. No se puede señalar a `init` (pid 1).

## Captura de señales: `signal`

Para que un proceso atienda a las señales que se le envían utilizar `signal()´:

```c
#include <signal.h>
typedef void (*sighandler_t)(int); //sighandler_t es un puntero a una función
sighandler_t signal(int signum, sighandler_t handler);
```

Instala `handler` como manejador de la señal `signum`: a partir de ese momento, cuando le llega la señal `signum`, el proceso interrumpe lo que esté haciendo, ejecuta `handler` y, al terminar, continúa por donde iba.

- **Devuelve** el manejador anterior, o `SIG_ERR` si hay error.

Dentro de `handler`, **no** hay que hacer: llamar a `malloc`/`free` o a `printf` (riesgo de interbloqueo). Por qué: ver [`TEORIA/08`](../../TEORIA/08-ipc-senales/#qué-no-hacer-dentro-de-un-manejador).

## Espera de señales

```c
#include <unistd.h>
unsigned int alarm(unsigned int s);   /* programa que se generé un SIGALRM dentro de s segundos, la ejecución sigue.*/
unsigned int sleep(unsigned int s);   /* espera s segundos, o hasta recibir una señal */
int pause(void);                             /* bloquea hasta recibir una señal */
```

- `alarm` **no bloquea** la ejecución, simplemente le dice al SO que te mandé una señal SIGALRM después de `s` segundos, pero la ejecución sigue.  
- `sleep` y `pause` **sí bloquean**: `sleep` hasta que pasan los segundos indicados o llega cualquier señal, y `pause` hasta que llega cualquier señal (sin límite de tiempo).


## Captura avanzada: `sigaction`

`SIGCHLD` es una señal **ambigua**: el kernel la manda tanto si un hijo **termina** como si se **para** o se **reanuda**. Con `signal()` no hay forma de distinguir el motivo. Más adelante, en [P8 — planificador de procesos](../08-planificador-de-procesos/), necesitaremos diferenciarlos, y para eso hace falta la versión más completa de `signal()`: `sigaction()`.

```c
#include <signal.h>
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);

struct sigaction {
    void     (*sa_handler)(int);
    sigset_t   sa_mask;
    int        sa_flags;
};
```

Recibe `signum` igual que `signal`; `act`, una estructura con el manejador (`sa_handler`), señales adicionales que se bloquean mientras se ejecuta el manejador (`sa_mask`, aquí la dejamos vacía) y una flags (`sa_flags`). `oldact` recupera la configuración anterior (aquí no nos hace falta, pasamos `NULL`). El flag que nos interesa es `SA_NOCLDSTOP`: con `SIGCHLD`, hace que sólo se reciba la señal cuando el hijo **termina**, ignorando sus paradas y reanudaciones.

Mismo manejador instalado con `signal` (avisa también de paradas y reanudaciones):
```c
void manejador(int senal) {
    printf("un hijo terminó, se ha parado o se ha reanudado\n");
}

signal(SIGCHLD, manejador);
```

Y con sigaction que sólo funciona cuando el hijo termina.
```c
struct sigaction accion = {0};
accion.sa_handler = manejador; //puntero al manejador, igual que en signal.
accion.sa_flags = SA_NOCLDSTOP;      /* este flag, hace que ignora paradas y reanudaciones: sólo avisa si el hijo termina */
sigaction(SIGCHLD, &accion, NULL);
```

Lo mismo, inicializando los campos de `accion` directamente en vez de asignarlos uno a uno:
```c
struct sigaction accion = { .sa_handler = manejador, .sa_flags = SA_NOCLDSTOP };
sigaction(SIGCHLD, &accion, NULL);
```



## Ejercicios propuestos

1. Programa que se queda esperando sin hacer nada (en un bucle con `pause()`) hasta que se pulsa `Ctrl-C`. En vez de morir en seco, que es la acción por defecto de `SIGINT`, captúrala e imprime un mensaje de despedida antes de terminar.

   Ejemplo de ejecución:
   ```
   $ ./esperador
   ^C
   adiós
   ```

2. Crea un programa **contador.c** que en bucle infinito hace: imprime `cuenta`, duerme 1 segundo, `cuenta += 1`. Si recibe `SIGUSR1`, pone a 0 el valor del contador. Ejecútalo en segundo plano y prueba a mandarle las señales `SIGUSR1`, `SIGSTOP` y `SIGCONT`.

   Ejemplo de ejecución:
   ```
   $ gcc contador.c -o contador
   $ ./contador & #lo pone en ejecución en segundo plano y te dice su pid
   [1] 4021
   0
   1
   2
   $ kill -USR1 4021
   0
   1
   $ kill -STOP 4021
   [1]+  Stopped                 ./contador
   $ kill -CONT 4021
   2
   3
   $ kill 4021
   [1]+  Terminated              ./contador
   ```

3. Programa que cree dos hijos: uno hijo autoenvía `SIGSTOP` (queda parado), el otro hijo duerme 1 segundo y luego se autoenvía `SIGKILL` (muere). El padre instala un manejador de `SIGCHLD` que simplemente incrementa una variable global. Después de crear los hijos, el padre espera 2 segundos y luego imprime el valor de la variable global.

   Repite la prueba instalando el manejador con `sigaction` y `SA_NOCLDSTOP`, y compara el valor final de `llamadas_sigchld`. Sin el flag debería salir `2` (una por la parada, otra por la muerte); con `SA_NOCLDSTOP` debería salir `1` (sólo llega la muerte).

4. Programa que pida un dato por teclado, pero sólo le da 5 segundos al usuario para introducirlo.

   Instala el manejador de `SIGALRM` con `sigaction`, dejando `sa_flags` a `0`:

   ```c
   struct sigaction accion = { .sa_handler = manejador_alarm, .sa_flags = 0 };
   sigaction(SIGALRM, &accion, NULL);
   ```

   No sirve `signal()` para esto: en Linux instala el manejador con `SA_RESTART` activado, así que el `scanf` interrumpido se reanudaría solo en vez de devolver el control, y el timeout nunca llegaría a detectarse. Con `sa_flags = 0` (sin `SA_RESTART`), al llegar `SIGALRM` el `scanf` bloqueado se interrumpe y retorna sin haber leído nada.

   Llama a `alarm(5)` justo antes del `scanf`. El manejador sólo pone a `1` una variable global `timeout`. Justo después del `scanf`, comprueba `timeout`: si vale `0`, el valor leído es correcto y cancela la alarma pendiente con `alarm(0)`; si vale `1`, no se ha leído nada y ha pasado el plazo, así que imprime `tiempo agotado`.

   Ejemplo de ejecución (a tiempo):
   ```
   $ ./lectura_timeout
   Escribe algo (tienes 5s): hola
   Leído: hola
   ```

   Ejemplo de ejecución (sin teclear nada):
   ```
   $ ./lectura_timeout
   Escribe algo (tienes 5s): 
   tiempo agotado
   ```