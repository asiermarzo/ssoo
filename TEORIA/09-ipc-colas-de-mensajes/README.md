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

<img src="img/cola-mensajes.svg" width="560" alt="Productores envían mensajes a una cola FIFO con prioridad; los consumidores extraen mensajes completos de la cola">

Una cola conserva mensajes completos hasta que un receptor los recoge, y puede ordenarlos por tipo o prioridad: el clasificador del núcleo coloca cada mensaje según su prioridad y entrega primero el más prioritario.

<img src="img/cola-mensajes-prioridad.svg" width="560" alt="Tres remitentes envían mensajes con distinta prioridad; el nucleo los coloca en una cola ordenada por prioridad, con el mensaje de mayor prioridad más cerca de la salida; el receptor extrae mensajes completos">

*Una cola conserva mensajes completos hasta que un receptor los recoge. Puede distinguirlos por tipo o prioridad.*

## Productores y consumidores desacoplados

El emisor y el receptor no necesitan ejecutarse simultáneamente: la cola actúa como almacenamiento intermedio persistente.

```mermaid
sequenceDiagram
    participant P as Productores
    participant Q as Cola persistente
    participant C as Consumidores
    P->>Q: envían a ritmos distintos
    rect rgb(253, 243, 208)
        Note over Q: los mensajes esperan almacenados
    end
    Q-->>C: entrega cuando un consumidor está listo
    Note over P,C: no necesitan ejecutarse simultáneamente
```

*El emisor y el receptor no necesitan ejecutarse simultáneamente: la cola actúa como almacenamiento intermedio.*

## Una cola POSIX en Linux

Las colas POSIX son objetos administrados por el sistema operativo, con límites de tamaño, persistencia y operaciones bloqueantes o no bloqueantes. En Linux son visibles bajo `/dev/mqueue`.

```mermaid
flowchart TB
    C1["mq_open('/pedidos')"] --> M[["/dev/mqueue/pedidos"]]
    C2["mq_send()<br/>prioridad · longitud"] --> M
    M --> A["mq_maxmsg = 10<br/>mq_msgsize = 256<br/>mensajes actuales = 3"]
    M --> R["mq_receive()"]
    I["ls /dev/mqueue"] -. inspecciona .-> M

    classDef productor fill:#cfe2f3,stroke:#2b6f99,color:#000;
    classDef consumidor fill:#d9ead3,stroke:#333,color:#000;
    classDef nucleo fill:#d9d9d9,stroke:#333,color:#000;
    classDef info fill:#fdf3d0,stroke:#333,color:#000;
    class C1,C2 productor;
    class R consumidor;
    class M nucleo;
    class A,I info;
```

*Las colas POSIX son objetos administrados por el sistema operativo, con límites de tamaño, persistencia y operaciones bloqueantes o no bloqueantes.*

Detalle en la práctica: [`PRACTICA/07`](../../PRACTICA/07-colas-de-mensajes/). Figuras catalogadas en [`TEORIA/IMAGENES.md`](../IMAGENES.md).
