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

<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 860 400" font-family="sans-serif" font-size="12" role="img" aria-label="Planificador de colas multinivel no apropiativas">
  <defs>
    <marker id="arrowP8" markerWidth="8" markerHeight="8" refX="4" refY="4" orient="auto">
      <path d="M0,0 L8,4 L0,8 z" fill="#333"/>
    </marker>
  </defs>
  <rect x="20" y="150" width="140" height="60" fill="#f2f2f2" stroke="#333"/>
  <text x="90" y="176" text-anchor="middle">solicitudes</text>
  <text x="90" y="192" text-anchor="middle" font-size="10">(teclado / configfile)</text>
  <path d="M160,180 L210,180" stroke="#333" fill="none" marker-end="url(#arrowP8)"/>
  <rect x="210" y="150" width="130" height="60" fill="#f2f2f2" stroke="#333"/>
  <text x="275" y="176" text-anchor="middle">procsched</text>
  <text x="275" y="192" text-anchor="middle" font-size="10">lee y encola por nivel</text>
  <path d="M340,165 L390,55" stroke="#333" fill="none" marker-end="url(#arrowP8)"/>
  <path d="M340,180 L390,180" stroke="#333" fill="none" marker-end="url(#arrowP8)"/>
  <path d="M340,195 L390,305" stroke="#333" fill="none" marker-end="url(#arrowP8)"/>
  <rect x="390" y="20" width="220" height="60" fill="#e3f5e1" stroke="#333"/>
  <text x="500" y="45" text-anchor="middle">Nivel 1 · Round Robin</text>
  <text x="500" y="61" text-anchor="middle" font-size="10">turnos de 4 s</text>
  <rect x="390" y="150" width="220" height="60" fill="#fdf3d0" stroke="#333"/>
  <text x="500" y="175" text-anchor="middle">Nivel 2 · Prioridades</text>
  <text x="500" y="191" text-anchor="middle" font-size="10">no apropiativas</text>
  <rect x="390" y="280" width="220" height="60" fill="#e0eefc" stroke="#333"/>
  <text x="500" y="305" text-anchor="middle">Nivel 3 · FCFS</text>
  <text x="500" y="321" text-anchor="middle" font-size="10">orden de llegada</text>
  <path d="M610,50 L650,175" stroke="#333" fill="none" marker-end="url(#arrowP8)"/>
  <path d="M610,180 L650,185" stroke="#333" fill="none" marker-end="url(#arrowP8)"/>
  <path d="M610,310 L650,195" stroke="#333" fill="none" marker-end="url(#arrowP8)"/>
  <text x="615" y="110" font-size="10" fill="#555">maxima prioridad</text>
  <text x="612" y="255" font-size="10" fill="#555">minima prioridad</text>
  <rect x="650" y="140" width="170" height="70" fill="#f6e3ee" stroke="#333"/>
  <text x="735" y="165" text-anchor="middle">planificador</text>
  <text x="735" y="181" text-anchor="middle" font-size="9">SIGSTOP / SIGCONT</text>
  <text x="735" y="195" text-anchor="middle" font-size="9">SIGALRM</text>
  <path d="M735,210 L735,260" stroke="#333" fill="none" marker-end="url(#arrowP8)"/>
  <rect x="655" y="262" width="160" height="45" fill="#eee" stroke="#333"/>
  <text x="735" y="288" text-anchor="middle">proceso en ejecucion</text>
  <path d="M825,290 C860,290 860,150 825,150" stroke="#333" fill="none" marker-end="url(#arrowP8)"/>
  <text x="828" y="222" font-size="9" fill="#555">fin de turno / bloqueo / fin</text>
</svg>

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
