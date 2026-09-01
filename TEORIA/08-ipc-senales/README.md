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

## Material

_(pendiente de añadir)_
