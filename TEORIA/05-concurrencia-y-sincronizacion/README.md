# Concurrencia y sincronización

## Contenidos

- Conceptos básicos: recursos compartibles y no compartibles; ejecución concurrente.
- Condiciones de carrera. Problema de la sección crítica y requisitos de una solución
  (exclusión mutua, progreso, espera limitada).
- Soluciones software (Peterson) y hardware (deshabilitar interrupciones, `test-and-set`,
  `compare-and-swap`).
- Semáforos: contadores y binarios; operaciones `wait`/`signal`. Semáforos POSIX.
- Problemas clásicos de sincronización: productor/consumidor, lectores/escritores,
  filósofos comensales.
- Monitores.
- Interbloqueo (*deadlock*): condiciones de Coffman, prevención, evitación (algoritmo del
  banquero), detección y recuperación.

## Sección crítica con semáforo

```mermaid
flowchart TD
    NC["sección no crítica"] --> W["wait(mutex)  ·  P"]
    W --> DEC{"mutex > 0?"}
    DEC -->|sí| CS["SECCIÓN CRÍTICA"]
    DEC -->|no| BLK["proceso bloqueado"]
    BLK -. otro proceso: signal .-> CS
    CS --> SIG["signal(mutex)  ·  V"]
    SIG --> NC
```

## Interbloqueo — condiciones de Coffman

Deben cumplirse las **cuatro** a la vez; romper cualquiera evita el interbloqueo.

```mermaid
flowchart LR
    EM["1. Exclusión mutua"] --> DL(("INTERBLOQUEO"))
    HW["2. Retención y espera"] --> DL
    NP["3. Sin expropiación"] --> DL
    CE["4. Espera circular"] --> DL
```

Espera circular: P1 retiene A y pide B, mientras P2 retiene B y pide A.

```mermaid
flowchart LR
    P1(("P1")) -->|pide| B["recurso B"]
    B -->|asignado a| P2(("P2"))
    P2 -->|pide| A["recurso A"]
    A -->|asignado a| P1
```

## Material

_(pendiente de añadir)_
