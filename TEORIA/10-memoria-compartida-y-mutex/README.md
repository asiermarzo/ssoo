# Memoria compartida y mutex

Práctica asociada: [`PRACTICA/06`](../../PRACTICA/06-memoria-compartida-y-semaforos/).

## Contenidos

- Memoria compartida: acceso de dos o más procesos a una misma zona de memoria.
  Ventajas (rapidez) y riesgo (condiciones de carrera).
- API POSIX: `shm_open()`, `ftruncate()`, `mmap()`, `munmap()`, `shm_unlink()`.
- Necesidad de sincronizar el acceso: exclusión mutua.
- Mutex y semáforos como mecanismo de protección de la región compartida.
- Mutex POSIX (`pthread_mutex_*`) y semáforos POSIX (`sem_open`/`sem_wait`/`sem_post`),
  con y sin nombre, compartidos entre procesos.

## Memoria compartida protegida por un mutex/semáforo

```mermaid
flowchart TD
    SEG[["zona de memoria compartida"]]
    subgraph A["proceso A"]
        A1["lock(mutex)"] --> A2["leer/escribir SEG"] --> A3["unlock(mutex)"]
    end
    subgraph B["proceso B"]
        B1["lock(mutex)"] --> B2["leer/escribir SEG"] --> B3["unlock(mutex)"]
    end
    A2 --- SEG
    B2 --- SEG
    A1 -. mientras A tiene el mutex, B espera en B1 .- B1
```

Detalle en la práctica: [`PRACTICA/06`](../../PRACTICA/06-memoria-compartida-y-semaforos/).

## Galería visual complementaria

### T10.1 · Dos procesos ante una misma pizarra

```mermaid
flowchart LR
    A["Proceso A<br/>espacio privado"] --> LA[lock]
    B["Proceso B<br/>espacio privado"] --> LB[lock]
    LA --> K{{"testigo único<br/>mutex"}}
    LB -. espera .-> K
    K --> S[["pizarra común<br/>memoria compartida"]]
    S --> U[unlock]
    U --> K
```

*La memoria compartida evita copiar datos entre procesos, pero obliga a sincronizar los accesos
para impedir escrituras simultáneas.*

### T10.2 · Sin mutex y con mutex

```mermaid
flowchart TB
    subgraph MAL["Sin mutex · carrera"]
        A1[Proceso A lee 10] --> A2[escribe 11]
        B1[Proceso B lee 10] --> B2[escribe 11]
        A2 --> R1["resultado 11<br/>se perdió una actualización"]
        B2 --> R1
    end
    subgraph BIEN["Con mutex · acceso ordenado"]
        C1["A: lock · 10→11 · unlock"] --> C2["B: lock · 11→12 · unlock"] --> R2[resultado 12]
    end
```

*Compartir memoria aporta velocidad. El mutex aporta el orden necesario para que esa velocidad no
produzca resultados incoherentes.*

### T10.3 · Una sala común mapeada en dos procesos

```mermaid
flowchart TB
    subgraph PA["Espacio virtual del proceso A"]
        APR[Memoria privada A]
        AS["dirección 0x7000<br/>segmento compartido"]
    end
    subgraph PB["Espacio virtual del proceso B"]
        BPR[Memoria privada B]
        BS["dirección 0x9000<br/>segmento compartido"]
    end
    AS --> F[["mismos marcos físicos<br/>en RAM"]]
    BS --> F
```

*Cada proceso conserva su memoria privada, pero el núcleo puede mapear el mismo segmento físico
dentro de varios espacios de direcciones.*

## Material

Las figuras complementarias de este tema están incluidas en la galería anterior y catalogadas en
[`TEORIA/IMAGENES.md`](../IMAGENES.md).
