# IPC: colas de mensajes

Práctica asociada: [`PRACTICA/07`](../../PRACTICA/07-colas-de-mensajes/).

## Contenidos

- Comunicación entre procesos por paso de mensajes.
- Colas de mensajes provistas por el sistema operativo. Persistencia y prioridad de mensajes.
- API POSIX: `mq_open()`, `mq_send()`, `mq_receive()`, `mq_close()`, `mq_unlink()`, atributos de la cola.
- Envío y recepción bloqueantes y no bloqueantes.
- Inspección: `lsipc`, `/dev/mqueue`.
- Comparación con pipes/fifos y con memoria compartida.

## Esquema de una cola de mensajes

```mermaid
flowchart LR
    P["productor(es)<br/>send / msgsnd"] ==> Q[["cola FIFO, con tipo/prioridad<br/>m1 · m2 · m3"]]
    Q ==> C["consumidor(es)<br/>receive / msgrcv"]
```

Detalle en la práctica: [`PRACTICA/07`](../../PRACTICA/07-colas-de-mensajes/).

## Galería visual complementaria

### T09.1 · Una cola como oficina postal

```mermaid
flowchart LR
    E1[Remitente A] -->|"mensaje · prioridad 2"| Q[["clasificador del núcleo<br/>m2 · m1 · m3"]]
    E2[Remitente B] -->|"mensaje · prioridad 8"| Q
    E3[Remitente C] -->|"mensaje · prioridad 4"| Q
    Q -->|"extrae un mensaje completo"| R[Receptor]
```

*Una cola conserva mensajes completos hasta que un receptor los recoge. Puede distinguirlos por tipo o prioridad.*

### T09.2 · Productores y consumidores desacoplados

```mermaid
sequenceDiagram
    participant P as Productores
    participant Q as Cola persistente
    participant C as Consumidores
    P->>Q: envían a ritmos distintos
    Note over Q: los mensajes esperan almacenados
    Q-->>C: entrega cuando un consumidor está listo
    Note over P,C: no necesitan ejecutarse simultáneamente
```

*El emisor y el receptor no necesitan ejecutarse simultáneamente: la cola actúa como almacenamiento intermedio.*

### T09.3 · Una cola POSIX visible en Linux

```mermaid
flowchart TB
    C1["mq_open('/pedidos')"] --> M["/dev/mqueue/pedidos"]
    C2["mq_send()<br/>prioridad · longitud"] --> M
    M --> A["mq_maxmsg = 10<br/>mq_msgsize = 256<br/>mensajes actuales = 3"]
    M --> R["mq_receive()"]
    I["ls /dev/mqueue"] -. inspecciona .-> M
```

*Las colas POSIX son objetos administrados por el sistema operativo, con límites de tamaño, persistencia y operaciones bloqueantes o no bloqueantes.*

## Material

Las figuras complementarias de este tema están incluidas en la galería anterior y catalogadas en [`TEORIA/IMAGENES.md`](../IMAGENES.md).
