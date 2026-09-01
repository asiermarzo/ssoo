# IPC: señales

Práctica asociada: [`PRACTICA/04`](../../PRACTICA/04-senales/).

## Contenidos

- Concepto de señal: forma limitada de IPC en sistemas POSIX.
- Señales estándar y su significado (`SIGINT`, `SIGTERM`, `SIGKILL`, `SIGCHLD`,
  `SIGUSR1`/`SIGUSR2`, `SIGSTOP`/`SIGCONT`…).
- Envío de señales: `kill()`, `raise()`.
- Manejo de señales: `signal()` frente a `sigaction()`. Acción por defecto, ignorar,
  capturar.
- Máscara de señales, señales pendientes y bloqueadas.
- Espera de señales: `pause()`, `sleep()`, `sigsuspend()`.

## Entrega de una señal

```mermaid
flowchart TD
    SRC["origen: kill() / evento del núcleo / excepción"] --> PEND["señal pendiente en el proceso destino"]
    PEND --> M{"¿bloqueada por la máscara?"}
    M -->|sí| WAIT["queda pendiente hasta desbloquearse"]
    M -->|no| DISP{"disposición de la señal"}
    DISP -->|SIG_DFL| DEF["acción por defecto: Term / Core / Stop / Ign / Cont"]
    DISP -->|SIG_IGN| IGN["se descarta"]
    DISP -->|manejador| H["se ejecuta el manejador y se reanuda el proceso"]
```

Detalle y tabla de señales: [`PRACTICA/04`](../../PRACTICA/04-senales/).

## Galería visual complementaria

### T08.1 · Una señal como aviso asíncrono

```mermaid
sequenceDiagram
    participant P as Proceso concentrado en su trabajo
    participant K as Núcleo
    participant H as Manejador
    K-->>P: señal · aviso inesperado
    P->>P: guarda el punto de reanudación
    P->>H: ejecuta una acción breve
    H-->>P: retorna
    P->>P: continúa donde estaba
```

*Una señal no transporta un flujo de datos: notifica de forma asíncrona que ha ocurrido un
determinado evento.*

### T08.2 · De `Ctrl+C` a `SIGINT`

```mermaid
flowchart LR
    K["Teclado<br/>Ctrl+C"] --> T["driver de terminal"]
    T --> SO["núcleo genera SIGINT"]
    SO --> FG["grupo de procesos<br/>en primer plano"]
    FG --> D{"disposición"}
    D -->|por defecto| F[terminar]
    D -->|manejador| H[atender y continuar]
```

*Al pulsar `Ctrl+C`, el terminal solicita al núcleo que envíe `SIGINT` al grupo de procesos en
primer plano.*

### T08.3 · Señal bloqueada y pendiente

```mermaid
stateDiagram-v2
    [*] --> Generada
    Generada --> Pendiente: la máscara la bloquea
    Pendiente --> Pendiente: continúa bloqueada
    Pendiente --> Entregada: se retira de la máscara
    Generada --> Entregada: no está bloqueada
    Entregada --> [*]
```

*Bloquear una señal no implica necesariamente descartarla: puede permanecer pendiente hasta que
la máscara permita su entrega.*

### T08.4 · `SIGTERM` frente a `SIGKILL`

```mermaid
flowchart TB
    TERM["SIGTERM<br/>petición de cierre"] --> P{"¿hay manejador?"}
    P -->|sí| L["guardar estado<br/>cerrar ficheros<br/>liberar recursos"]
    P -->|no| X[terminar]
    L --> X
    KILL["SIGKILL<br/>terminación forzosa"] --> D["el núcleo termina el proceso<br/>no se captura ni se ignora"]
```

*`SIGTERM` permite que el proceso responda y libere recursos. `SIGKILL` no puede capturarse ni
ignorarse y provoca su terminación inmediata.*

## Material

Las figuras complementarias de este tema están incluidas en la galería anterior y catalogadas en
[`TEORIA/IMAGENES.md`](../IMAGENES.md).
