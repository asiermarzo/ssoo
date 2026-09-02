# Espacio usuario y espacio kernel. Llamadas al sistema

## Contenidos

- Modo usuario y modo núcleo (kernel). Bit de modo y protección.
- El SO como colección de procedimientos con interfaz de entrada/salida definida.
- Llamadas al sistema: paso de parámetros en lugares bien definidos, cambio de modo, ejecución del servicio y retorno al programa de usuario.
- Estructuras de los sistemas operativos:
  - Sistemas monolíticos (programa principal, procedimientos de servicio, utilidades). Núcleos tipo Unix (Linux, BSD, Solaris), tipo DOS, etc.
  - Microkernels: primitivas mínimas (espacios de direcciones, IPC, planificación básica); el resto de servicios como procesos servidores en espacio de usuario. Ventajas (complejidad, aislamiento de fallos, portabilidad, drivers) e inconvenientes (sincronización entre módulos).
  - Sistemas por capas, máquinas virtuales, exokernels.

## Flujo de una llamada al sistema

```mermaid
sequenceDiagram
    participant U as Programa · modo usuario
    participant W as Envoltorio de libc
    participant K as Núcleo · modo kernel
    U->>W: read(fd, buf, n)
    W->>K: instrucción de trap · nº de syscall + parámetros
    Note over K: cambio a modo kernel
    K->>K: valida parámetros y ejecuta el servicio
    K-->>W: valor de retorno / -1 y errno
    Note over U: vuelta a modo usuario
    W-->>U: resultado
```

## Estructuras: monolítico frente a microkernel

```mermaid
flowchart TB
    subgraph MONO["Monolítico"]
        direction TB
        MA["aplicaciones (usuario)"]
        MK["núcleo: planificador + memoria + FS + drivers + IPC"]
        MA -->|syscall| MK
    end
    subgraph MICRO["Microkernel"]
        direction TB
        UA["aplicaciones"]
        SRV["servidores en usuario:<br/>FS · drivers · memoria"]
        UK["microkernel: IPC + planificación + direcciones"]
        UA <-->|IPC| SRV
        SRV <-->|IPC| UK
        UA <-->|IPC| UK
    end
```

## Galería visual complementaria

### T02.1 · El kernel como zona restringida

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
```

*Los programas ordinarios se ejecutan con privilegios limitados. Para acceder a recursos protegidos deben solicitar un servicio al núcleo.*

### T02.2 · La llamada al sistema como ventanilla

```mermaid
sequenceDiagram
    participant A as Aplicación
    participant V as Ventanilla segura · libc
    participant K as Núcleo
    participant H as Hardware
    A->>V: read(fd, búfer, tamaño)
    V->>K: número de servicio + parámetros
    Note over V,K: cambio controlado a modo kernel
    K->>K: comprueba permisos y direcciones
    K->>H: solicita los datos
    H-->>K: datos disponibles
    K-->>V: resultado o error
    V-->>A: retorno a modo usuario
```

*Una llamada al sistema cruza temporalmente la frontera entre modo usuario y modo kernel sin entregar a la aplicación el control directo del hardware.*

### T02.3 · Núcleo monolítico y microkernel como talleres

```mermaid
flowchart TB
    subgraph M["Gran taller central · monolítico"]
        MA[Aplicaciones] --> MK["Kernel<br/>memoria · ficheros · red · drivers · IPC"]
        MK --> MH[Hardware]
    end
    subgraph X["Talleres aislados · microkernel"]
        XA[Aplicaciones] <-->|mensajes| XS1[Servidor de ficheros]
        XA <-->|mensajes| XS2[Servidor de dispositivos]
        XS1 <-->|IPC| XK["Microkernel<br/>planificación · direcciones · IPC"]
        XS2 <-->|IPC| XK
        XK --> XH[Hardware]
    end
```

*Un núcleo monolítico reúne muchos servicios en un mismo espacio privilegiado. Un microkernel conserva solo los mecanismos esenciales y delega otros servicios a procesos aislados.*

## Material

Las figuras complementarias de este tema están incluidas en la galería anterior y catalogadas en [`TEORIA/IMAGENES.md`](../IMAGENES.md).
