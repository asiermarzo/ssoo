# P8 — Planificador de procesos

Construcción de un planificador de procesos mediante colas de múltiples niveles. **Entrega evaluable** (curso 2025/26: 28/11/2025).

## Descripción general

Construir un planificador (*scheduler*) de procesos **a alto nivel, basado en señales**, que implementa un sistema de colas de **tres niveles NO APROPIATIVOS**:

- **Nivel 1** (mayor prioridad): política **Round Robin** con turnos de 4 segundos.
- **Nivel 2**: política de **prioridades** no apropiativas (menor valor = mayor prioridad).
- **Nivel 3** (menor prioridad): política **FCFS** (*first come first served*).

Los procesos de nivel 1 se atienden con la mayor celeridad; los de nivel 3, los últimos. El planificador se apoya en señales para detener y reanudar los procesos gobernados (`SIGSTOP` / `SIGCONT`) y en un temporizador (`SIGALRM` / `alarm`) para los turnos.

## Arquitectura

`procsched` se construye con la ayuda de una **cola de mensajes**:

1. Un proceso recibe las solicitudes de ejecución (por teclado o por fichero de configuración; con teclado, EOF con `Ctrl-D`).
2. Crea la cola de mensajes (y la elimina ordenadamente al final), construye la estructura de datos de cada proceso y la encola en la cola que corresponda a su nivel.
3. Crea un segundo proceso encargado de la planificación.
4. Libera todos los recursos (procesos y colas) cuando el usuario indica la finalización (`Ctrl-C`).

```mermaid
flowchart TD
    IN["solicitudes<br/>(teclado / configfile)<br/>nivel · prioridad · programa · args"] --> PS["procsched<br/>lee y encola"]
    PS --> Q1[["Nivel 1 · Round Robin (turnos de 4 s)"]]
    PS --> Q2[["Nivel 2 · Prioridades no apropiativas"]]
    PS --> Q3[["Nivel 3 · FCFS"]]
    Q1 -->|"máxima prioridad"| SCH["planificador<br/>SIGSTOP / SIGCONT / SIGALRM"]
    Q2 --> SCH
    Q3 -->|"mínima prioridad"| SCH
    SCH --> RUN["proceso en ejecución"]
    RUN -->|"fin de turno / bloqueo / fin"| SCH
```

## Manual

```
PROCSCHED(1)

NOMBRE
      procsched — scheduler de procesos a nivel de usuario

SINOPSIS
      procsched [configfile]

DESCRIPCIÓN
      procsched crea las colas de mensajes y el planificador, lanza los procesos
      configurados y va encolando las peticiones conforme las recibe. El
      planificador ejecuta los procesos encolados atendiendo a la prioridad de
      cada nivel (máxima prioridad = nivel más bajo).

      configfile es opcional; si no se indica, la configuración se lee de la
      entrada estándar con el mismo formato.

      Formato del fichero de configuración: una línea por programa a ejecutar:

          nivel prioridad nombreprograma argumento1 argumento2 ...

          nivel:     1 (Round Robin, turnos de 4 s), 2 (prioridades no
                     apropiativas) o 3 (FCFS). Los tres niveles son NO apropiativos.
          prioridad: sólo se tiene en cuenta para los procesos de nivel 2; en el
                     resto se ignora.
          nombreprograma argumento1 ...: programa y sus argumentos (número
                     indeterminado, distinto por programa).

      procsched no finaliza hasta que el usuario pulsa Ctrl-C, momento en el que
      termina ordenadamente liberando todos los recursos y muestra por pantalla:
      el número de procesos concluidos con éxito (diferenciando cuántos por
      prioridades, cuántos por FCFS y cuántos por Round Robin), el número de
      cambios de contexto producidos y el número total de procesos finalizados.

VALOR DEVUELTO
      A su finalización, procsched indica el número de procesos concluidos con
      éxito (por política), el número de cambios de contexto y el número total de
      procesos finalizados.
```

## Llamadas al sistema útiles

`kill(1)`, `kill(2)`, `sigaction(2)`, `signal(2)`, `signal(7)`, `sleep(2)`/`sleep(3)`, `raise(3)`, `nanosleep(2)`, `pause(2)`, `alarm(2)`, `execvp(3)`, `fork(2)`, `wait(2)`. Ver [P2](../02-procesos-e-hilos/), [P4](../04-senales/) y [P7](../07-colas-de-mensajes/) para sus descripciones.

Se sugiere emplear la biblioteca `fragmenta.o` usada en [P5](../05-minishell/).

## Entrega

El comprimido debe incluir el `Makefile` provisto y todos los `.c` y `.h` necesarios para crear `procsched`. `make` sin argumentos debe crear el ejecutable `procsched`. Para la corrección se borran los ejecutables, se hace `touch` a los fuentes y se recompila con el `Makefile`.

## Consulta

- Teoría: [`TEORIA/04`](../../TEORIA/04-planificacion-de-procesos/), [`TEORIA/08`](../../TEORIA/08-ipc-senales/), [`TEORIA/09`](../../TEORIA/09-ipc-colas-de-mensajes/).
