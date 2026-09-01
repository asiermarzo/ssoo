# IPC: pipes y fifos

Práctica asociada: [`PRACTICA/03`](../../PRACTICA/03-pipes-y-fifos/).

## Contenidos

- Comunicación entre procesos por tuberías.
- Pipes (tuberías sin nombre): `pipe()`, comunicación entre procesos emparentados,
  extremos de lectura y escritura.
- FIFOs (tuberías con nombre): `mkfifo()`, comunicación entre procesos no emparentados.
- Redirección de una pipe a la E/S estándar (`dup2`).
- Lecturas y escrituras bloqueantes y no bloqueantes.
- Atención a varios canales: `select()`.

## Esquema de una pipe

```mermaid
flowchart LR
    W["proceso escritor<br/>write por el extremo de escritura"] ==> BUF[["búfer FIFO del núcleo"]]
    BUF ==> R["proceso lector<br/>read por el extremo de lectura"]
```

Detalle en la práctica: [`PRACTICA/03`](../../PRACTICA/03-pipes-y-fifos/).

## Galería visual complementaria

### T07.1 · Una pipe como tubo neumático

```mermaid
flowchart LR
    W["Proceso escritor<br/>introduce bytes"] ==>|"extremo de escritura"| P[["tubo del núcleo<br/>a · b · c · d"]]
    P ==>|"extremo de lectura"| R["Proceso lector<br/>recibe en el mismo orden"]
    X["sentido único"] -.-> P
```

*Una pipe es un canal unidireccional administrado por el núcleo: un proceso escribe una secuencia
de bytes y otro la lee.*

### T07.2 · Una pipeline de shell

```mermaid
flowchart LR
    A["cat registro.log"] -->|stdout| B["grep ERROR"]
    B -->|solo líneas coincidentes| C[sort]
    C -->|líneas ordenadas| D["uniq -c"]
    D --> R["recuento final"]
```

*La shell conecta la salida estándar de un programa con la entrada del siguiente, formando
herramientas complejas a partir de programas sencillos.*

### T07.3 · Pipe anónima frente a FIFO con nombre

```mermaid
flowchart TB
    subgraph A["Pipe anónima · conexión privada heredada"]
        PA[Proceso padre] --> P[[pipe]] --> PH[Proceso hijo]
    end
    subgraph F["FIFO · punto de encuentro con nombre"]
        X[Proceso sin parentesco] --> N[["/tmp/canal"]] --> Y[Otro proceso]
    end
```

*Las pipes se usan normalmente entre procesos emparentados. Una FIFO posee un nombre y permite
conectar procesos que no comparten parentesco.*

## Material

Las figuras complementarias de este tema están incluidas en la galería anterior y catalogadas en
[`TEORIA/IMAGENES.md`](../IMAGENES.md).
