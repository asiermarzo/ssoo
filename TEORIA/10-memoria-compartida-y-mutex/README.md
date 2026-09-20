# Memoria compartida y mutex

Práctica asociada: [`PRACTICA/06`](../../PRACTICA/06-memoria-compartida-y-semaforos/).

## Contenidos

- Memoria compartida: acceso de dos o más procesos a una misma zona de memoria. Ventajas (rapidez) y riesgo (condiciones de carrera).
- API POSIX: `shm_open()`, `ftruncate()`, `mmap()`, `munmap()`, `shm_unlink()`.
- Necesidad de sincronizar el acceso: exclusión mutua.
- Mutex y semáforos como mecanismo de protección de la región compartida.
- Mutex POSIX (`pthread_mutex_*`) y semáforos POSIX (`sem_open`/`sem_wait`/`sem_post`), con y sin nombre, compartidos entre procesos.
- Semáforos binarios (0/1, exclusión mutua) y n-arios (0..N, hasta N procesos concurrentes).
- Operaciones atómicas `wait`/`P` y `signal`/`V`, y las tres condiciones que debe garantizar un semáforo.

## Memoria compartida y exclusión mutua

La memoria compartida evita copiar datos entre procesos, pero obliga a sincronizar los accesos para impedir escrituras simultáneas. Un testigo único (el mutex) decide quién puede tocar la pizarra común en cada momento.

```mermaid
flowchart LR
    A["Proceso A<br/>espacio privado"] --> LA[lock]
    B["Proceso B<br/>espacio privado"] --> LB[lock]
    LA --> K{{"testigo único<br/>mutex"}}
    LB -. espera .-> K
    K --> S[["pizarra común<br/>memoria compartida"]]
    S --> U[unlock]
    U --> K

    classDef proceso fill:#cfe2f3,stroke:#2b6f99,color:#000;
    classDef avanza fill:#d9ead3,stroke:#333,color:#000;
    classDef espera fill:#fce5a8,stroke:#333,color:#000;
    classDef mutex fill:#fbe0e0,stroke:#333,color:#000;
    classDef recurso fill:#d9d9d9,stroke:#333,color:#000;
    class A,B proceso;
    class LA avanza;
    class LB espera;
    class K mutex;
    class S,U recurso;
```

*La memoria compartida evita copiar datos entre procesos, pero obliga a sincronizar los accesos para impedir escrituras simultáneas.*

## Memoria compartida protegida por un mutex/semáforo

<img src="img/mutex-linea-tiempo.svg" width="520" alt="Linea de tiempo vertical: el proceso A bloquea el mutex, trabaja y libera; el proceso B permanece bloqueado durante exactamente ese intervalo y solo entonces empieza su propia sección critica">

Sin mutex, dos procesos pueden leer el mismo valor y escribir encima el uno del otro, perdiendo una actualización. Con mutex, los accesos se ordenan y el resultado es coherente.

```mermaid
flowchart TB
    subgraph MAL["Sin mutex · carrera"]
        A1[Proceso A lee 10] --> A2[escribe 11]
        B1[Proceso B lee 10] --> B2[escribe 11]
        A2 --> R1["resultado 11<br/>se perdió una actualización"]
        B2 --> R1
    end
    subgraph BIEN["Con mutex · acceso ordenado"]
        C1["A: lock · 10→11 · unlock"] --> C2["B: lock · 11→12 · unlock"] --> R2(["resultado 12"])
    end

    classDef procA fill:#cfe2f3,stroke:#2b6f99,color:#000;
    classDef procB fill:#6ba3d6,stroke:#2b6f99,color:#0d2a3f;
    classDef error fill:#fbe0e0,stroke:#333,color:#000;
    classDef ok fill:#d9ead3,stroke:#333,color:#000;
    class A1,A2,C1 procA;
    class B1,B2,C2 procB;
    class R1 error;
    class R2 ok;
    style MAL fill:none,stroke-dasharray: 5 5;
    style BIEN fill:none,stroke-dasharray: 5 5;
```

*Compartir memoria aporta velocidad. El mutex aporta el orden necesario para que esa velocidad no produzca resultados incoherentes.*

## Semáforos: wait y signal

Un semáforo es la generalización del mutex: un contador con dos operaciones atómicas, **wait/P** (decrementa; si el contador quedaría negativo, bloquea al proceso) y **signal/V** (incrementa y despierta a un proceso bloqueado, si lo hay). Debe garantizar tres cosas: exclusión mutua (solo un proceso en la sección crítica), que un proceso fuera de su sección crítica no bloquee a otros, y que el que está dentro no bloquee para siempre al resto.

- **Binarios** (0/1): a 1 permiten el acceso a la sección crítica, a 0 lo bloquean. Es el caso particular que equivale a un mutex.
- **N-arios** (0..N): permiten que hasta N procesos trabajen a la vez en una tarea no crítica (p. ej. varios lectores).

```mermaid
flowchart TD
    W["wait / P  ·  decrementa"] --> Q{"¿semáforo ≥ 0?"}
    Q -->|sí| CS["SECCIÓN CRÍTICA<br/>(acceso exclusivo al recurso compartido)"]
    Q -->|no| BL["proceso bloqueado en la cola del semáforo"]
    BL -. otro proceso hace signal .-> CS
    CS --> S["signal / V  ·  incrementa<br/>(despierta a un proceso bloqueado)"]

    classDef accion fill:#cfe2f3,stroke:#2b6f99,color:#222;
    classDef decision fill:#fce5a8,stroke:#a06a1a,color:#222;
    classDef critica fill:#d9ead3,stroke:#3a7a3a,color:#222;
    classDef bloqueado fill:#fbe0e0,stroke:#a04040,color:#222;

    class W,S accion;
    class Q decision;
    class CS critica;
    class BL bloqueado;
```

*Un mutex es el caso particular de un semáforo binario usado para exclusión mutua.*

## El mismo segmento en dos espacios de direcciones

Cada proceso conserva su memoria privada, pero el kernel puede mapear (`mmap`) el mismo segmento físico dentro de varios espacios de direcciones, posiblemente en direcciones virtuales distintas.

<img src="img/memoria-compartida-mmap.svg" width="560" alt="Los procesos A y B mapean, en direcciones virtuales distintas, el mismo segmento físico de memoria compartida">

*Cada proceso conserva su memoria privada, pero el kernel puede mapear el mismo segmento físico dentro de varios espacios de direcciones.*

Detalle en la práctica: [`PRACTICA/06`](../../PRACTICA/06-memoria-compartida-y-semaforos/).
