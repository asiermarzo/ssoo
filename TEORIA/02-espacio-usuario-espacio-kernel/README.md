# Espacio usuario y espacio kernel. Llamadas al sistema

## Contenidos

- Modo usuario y modo núcleo (kernel). Bit de modo y protección.
- El SO como colección de procedimientos con interfaz de entrada/salida definida.
- Llamadas al sistema: paso de parámetros en lugares bien definidos, cambio de modo,
  ejecución del servicio y retorno al programa de usuario.
- Estructuras de los sistemas operativos:
  - Sistemas monolíticos (programa principal, procedimientos de servicio, utilidades).
    Núcleos tipo Unix (Linux, BSD, Solaris), tipo DOS, etc.
  - Microkernels: primitivas mínimas (espacios de direcciones, IPC, planificación básica);
    el resto de servicios como procesos servidores en espacio de usuario. Ventajas
    (complejidad, aislamiento de fallos, portabilidad, drivers) e inconvenientes
    (sincronización entre módulos).
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

## Material

_(pendiente de añadir)_
