# IPC: pipes y fifos

Práctica asociada: [`PRACTICA/03`](../../PRACTICA/03-pipes-y-fifos/).

## Contenidos

- Comunicación entre procesos por tuberías.
- Pipes (tuberías sin nombre): `pipe()`, comunicación entre procesos emparentados, extremos de lectura y escritura.
- FIFOs (tuberías con nombre): `mkfifo()`, comunicación entre procesos no emparentados.
- Redirección de una pipe a la E/S estándar (`dup2`).
- Lecturas y escrituras bloqueantes y no bloqueantes.
- Atención a varios canales: `select()`.

## Esquema de una pipe

```mermaid
flowchart LR
    W["proceso escritor<br/>write por el extremo de escritura"] ==> BUF[["búfer FIFO del núcleo"]]
    BUF ==> R["proceso lector<br/>read por el extremo de lectura"]

    classDef proceso fill:#cfe2f3,stroke:#2b6f99,color:#000;
    classDef nucleo fill:#d9d9d9,stroke:#333,color:#000;
    class W,R proceso;
    class BUF nucleo;
```

Una pipe funciona como un tubo neumático de sentido único administrado por el núcleo: un proceso introduce una secuencia de bytes por el extremo de escritura y otro la recibe, en el mismo orden, por el extremo de lectura.

<img src="img/pipe-tubo-neumatico.svg" width="560" alt="Una pipe como un tubo neumático de sentido único: el escritor introduce bytes por un extremo y el lector los recibe en el mismo orden por el otro">

*Una pipe es un canal unidireccional administrado por el núcleo: un proceso escribe una secuencia de bytes y otro la lee.*

## Pipelines de shell

La shell conecta la salida estándar de un programa con la entrada del siguiente (mediante `dup2` sobre los extremos de una pipe), formando herramientas complejas a partir de programas sencillos.

```mermaid
flowchart LR
    A["cat registro.log"] -->|stdout| B["grep ERROR"]
    B -->|solo líneas coincidentes| C[sort]
    C -->|líneas ordenadas| D["uniq -c"]
    D --> R(["recuento final"])

    classDef proceso fill:#cfe2f3,stroke:#2b6f99,color:#000;
    classDef resultado fill:#d9ead3,stroke:#333,color:#000;
    class A,B,C,D proceso;
    class R resultado;
```

*La shell conecta la salida estándar de un programa con la entrada del siguiente, formando herramientas complejas a partir de programas sencillos.*

## Pipe anónima frente a FIFO con nombre

Las pipes anónimas conectan procesos emparentados que heredan los descriptores. Una FIFO posee un nombre en el sistema de ficheros y permite conectar procesos que no comparten parentesco.

```mermaid
flowchart TB
    subgraph A["Pipe anónima · conexión privada heredada"]
        PA[Proceso padre] --> P[[pipe]] --> PH[Proceso hijo]
    end
    subgraph F["FIFO · punto de encuentro con nombre"]
        X[Proceso sin parentesco] --> N[["/tmp/canal"]] --> Y[Otro proceso]
    end

    classDef proceso fill:#cfe2f3,stroke:#2b6f99,color:#000;
    classDef canal fill:#fce5a8,stroke:#333,color:#000;
    class PA,PH,X,Y proceso;
    class P,N canal;
```

*Las pipes se usan normalmente entre procesos emparentados. Una FIFO posee un nombre y permite conectar procesos que no comparten parentesco.*

Detalle en la práctica: [`PRACTICA/03`](../../PRACTICA/03-pipes-y-fifos/). Figuras catalogadas en [`TEORIA/IMAGENES.md`](../IMAGENES.md).
