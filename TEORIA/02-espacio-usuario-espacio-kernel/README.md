# Espacio usuario y espacio kernel. Llamadas al sistema

## Contenidos

- Modo usuario y modo núcleo (kernel). Bit de modo y protección.
- El SO como colección de procedimientos con interfaz de entrada/salida definida.
- Llamadas al sistema: paso de parámetros en lugares bien definidos, cambio de modo, ejecución del servicio y retorno al programa de usuario.
- Estructuras de los sistemas operativos:
  - Sistemas monolíticos (programa principal, procedimientos de servicio, utilidades). Núcleos tipo Unix (Linux, BSD, Solaris), tipo DOS, etc.
  - Microkernels: primitivas mínimas (espacios de direcciones, IPC, planificación básica); el resto de servicios como procesos servidores en espacio de usuario. Ventajas (complejidad, aislamiento de fallos, portabilidad, drivers) e inconvenientes (sincronización entre módulos).
  - Sistemas por capas, máquinas virtuales, exokernels.

## Modo usuario y modo kernel

Los programas ordinarios se ejecutan con privilegios limitados. Para acceder a recursos protegidos (memoria, CPU, dispositivos, sistema de ficheros) deben solicitar un servicio al núcleo a través de un único punto de control: la llamada al sistema.

```mermaid
flowchart TB
    subgraph U["Zona pública · modo usuario"]
        A1[Aplicación]
        A2[Biblioteca]
        A3[Utilidad]
    end
    G{{"Control de acceso<br/>syscall · trap"}}
    subgraph K["Zona restringida · modo kernel"]
        MEM[Memoria]
        CPU[CPU y planificación]
        DEV[Dispositivos]
        FS[Sistema de ficheros]
    end
    A1 --> G
    A2 --> G
    A3 --> G
    G --> MEM
    G --> CPU
    G --> DEV
    G --> FS

    classDef usuario fill:#d9ead3,stroke:#4d7a33,color:#000;
    classDef kernel fill:#cfe2f3,stroke:#1f3f66,color:#000;
    classDef gate fill:#fce5a8,stroke:#a67c00,color:#000;
    class A1,A2,A3 usuario;
    class MEM,CPU,DEV,FS kernel;
    class G gate;
    style U fill:#eef8ee,stroke:#4d7a33,stroke-width:1px;
    style K fill:#eaf2fb,stroke:#1f3f66,stroke-width:1px;
```

*Los programas ordinarios se ejecutan con privilegios limitados. Para acceder a recursos protegidos deben solicitar un servicio al núcleo.*

## Flujo de una llamada al sistema

```mermaid
sequenceDiagram
    participant U as Programa · modo usuario
    participant W as Envoltorio de libc
    participant K as Núcleo · modo kernel
    U->>W: read(fd, buf, n)
    W->>K: instrucción de trap · nº de syscall + parámetros
    rect rgb(207, 226, 243)
        Note over K: cambio a modo kernel
        K->>K: valida parámetros y ejecuta el servicio
        K-->>W: valor de retorno / -1 y errno
    end
    Note over U: vuelta a modo usuario
    W-->>U: resultado
```

La llamada al sistema se comporta como una ventanilla segura: la aplicación entrega una petición y unos parámetros, el núcleo comprueba permisos y direcciones, ejecuta el servicio y devuelve el resultado, sin ceder nunca a la aplicación el control directo del hardware.

```mermaid
sequenceDiagram
    participant A as Aplicación
    participant V as Ventanilla segura · libc
    participant K as Núcleo
    participant H as Hardware
    A->>V: read(fd, búfer, tamaño)
    V->>K: número de servicio + parámetros
    rect rgb(207, 226, 243)
        Note over V,K: cambio controlado a modo kernel
        K->>K: comprueba permisos y direcciones
        K->>H: solicita los datos
        H-->>K: datos disponibles
        K-->>V: resultado o error
    end
    V-->>A: retorno a modo usuario
```

*Una llamada al sistema cruza temporalmente la frontera entre modo usuario y modo kernel sin entregar a la aplicación el control directo del hardware.*

## Estructuras: monolítico frente a microkernel

<img src="img/monolitico-vs-microkernel.svg" width="560" alt="Comparación en capas de un núcleo monolítico y un microkernel, con la frontera entre espacio de usuario y espacio de kernel marcada en cada caso">

Un núcleo monolítico reúne muchos servicios en un mismo espacio privilegiado; un microkernel conserva solo los mecanismos esenciales y delega el resto a procesos aislados que se comunican por mensajes, como talleres separados apoyados sobre el mismo hardware.

<img src="img/microkernel-talleres.svg" width="560" alt="Un gran taller único frente a varios talleres aislados que se comunican por mensajes, ambos apoyados sobre el mismo hardware">

*Un núcleo monolítico reúne muchos servicios en un mismo espacio privilegiado. Un microkernel conserva solo los mecanismos esenciales y delega otros servicios a procesos aislados.*

---

Figuras catalogadas en [`TEORIA/IMAGENES.md`](../IMAGENES.md).
