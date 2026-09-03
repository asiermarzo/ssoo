# Tema 7: Dispositivos de Entrada/Salida

7.1 Conceptos básicos · 7.2 Funciones de entrada/salida · 7.3 Almacenamiento intermedio · 7.4 Tipos de discos duros · 7.5 Planificación de discos · 7.6 Caché de disco · 7.7 Entrada/salida en Linux.

---

## 7.1 Conceptos básicos

- Los **controladores** son la interfaz **software** entre el hardware del dispositivo y el sistema operativo.
- Para lograr **homogeneidad** en el conjunto de instrucciones que el SO puede invocar sobre un dispositivo, se dispone de **manejadores de dispositivos**: un conjunto de funciones que abstraen el funcionamiento de un controlador concreto.
- Se construyen **APIs** con las funciones básicas de acceso a un dispositivo: `open`, `close`, `control`, `seek`… El manejador exporta al SO una **interfaz común de llamadas al sistema** para los dispositivos.

<img src="img/capas-es.svg" width="420" alt="Pirámide de capas de la E/S: de abajo arriba, controladores y manejadores de interrupciones, manejadores de dispositivos, rutinas del sistema operativo y programas de usuario">

- Las operaciones de lectura y escritura son **secuenciales**. Interesa **paralelizar** al máximo las actividades de las aplicaciones durante los intervalos de acceso a los dispositivos ⇒ **solapamiento de E/S y procesador**. Requisitos:
  - El lenguaje de programación y el SO deben permitir que el hilo **inicie** una operación de E/S y **sondee** si ha concluido.
  - El hilo debe poder realizar otros trabajos mientras se llevan a cabo las operaciones de E/S.

### Características de los dispositivos

- **Unidad de transferencia**: los datos pueden intercambiarse como flujos de bytes/caracteres de longitud variable (**dispositivos modo carácter**, p. ej. teclado) o como flujos de bloques de tamaño fijo (**dispositivos modo bloque**, p. ej. disco duro).
- **Velocidad de transmisión**: muy variable según la naturaleza del dispositivo.
- **Utilidad**, **complejidad de control** y **representación de los datos**.

```mermaid
flowchart TB
    IO[Dispositivos de E/S] --> B["BLOQUE<br/>unidades direccionables"]
    IO --> C["CARÁCTER<br/>secuencia continua"]
    B --> SSD[SSD]
    B --> HDD[Disco duro]
    B --> USB[Memoria USB]
    C --> KEY[Teclado]
    C --> MOUSE[Ratón]
    C --> SER[Puerto serie · sensor]

    classDef raiz fill:#d9d9d9,stroke:#333,color:#000;
    classDef bloque fill:#cfe2f3,stroke:#2b6f99,color:#000;
    classDef caracter fill:#fce5a8,stroke:#333,color:#000;
    class IO raiz;
    class B,SSD,HDD,USB bloque;
    class C,KEY,MOUSE,SER caracter;
```

*Los dispositivos de bloque transfieren unidades direccionables de datos; los de carácter producen o consumen secuencias continuas.*

## 7.2 Funciones de entrada/salida

- El acceso mediante **E/S directa** transfiere los datos de entrada desde el controlador a un registro de la CPU y de éste a memoria principal (ídem para la salida).
- El acceso mediante **DMA** transfiere los datos entre el controlador y la memoria principal **directamente**.
- Si el dispositivo dispone de **interrupciones**, el software no necesita recurrir a operaciones periódicas de **sondeo**.

```mermaid
sequenceDiagram
    participant CPU as CPU
    participant DMA as Controlador DMA
    participant D as Dispositivo
    participant M as Memoria RAM
    CPU->>DMA: origen, destino y tamaño
    DMA->>D: inicia transferencia
    rect rgb(207, 226, 243)
        loop bloque completo
            D->>M: dato directo a memoria
        end
    end
    DMA-->>CPU: interrupción de fin
```

*El acceso directo a memoria (DMA) permite transferir bloques sin que la CPU tenga que copiar personalmente cada palabra.*

**Estrategias de acceso**: E/S directa con sondeo · E/S por DMA con sondeo *(muy poco frecuente)* · E/S directa con interrupciones · E/S por DMA con interrupciones.

### E/S con sondeo (lectura)

El proceso solicita la lectura y **espera activamente** consultando el registro de estado del controlador hasta que el dato está disponible:

```mermaid
sequenceDiagram
    participant P as Proceso / función leer
    participant C as Controlador del dispositivo
    P->>C: 1-2. leer(dispositivo, …): programa la operación
    P->>C: 3. escribe la orden en el registro «Operación»
    rect rgb(253, 243, 208)
        loop 4. sondeo
            P->>C: consulta el registro «Estado»
            C-->>P: ¿operación completada?
        end
    end
    C-->>P: 5. lee el registro «Datos» y lo copia a memoria
```

### E/S dirigida por interrupciones

El proceso se **bloquea** tras lanzar la operación; el controlador avisa con una interrupción al terminar:

```mermaid
sequenceDiagram
    participant P as Proceso
    participant D as Manejador (lectura/escritura)
    participant T as Tabla de estado de dispositivo
    participant MI as Manejador de interrupción
    participant C as Controlador
    P->>D: 1-3. leer(dispositivo, …)
    D->>T: 4. registra la operación pendiente
    D->>C: 5. programa Operación / Estado / Datos
    rect rgb(251, 224, 224)
        Note over P: el proceso queda BLOQUEADO, la CPU ejecuta otro proceso
    end
    rect rgb(207, 226, 243)
        C-->>MI: 6. interrupción al completar
        MI->>T: 7. actualiza la tabla de estado
        MI->>D: 8a-8b. copia los datos
    end
    D-->>P: 9. proceso LISTO (datos disponibles)
```

En lugar de consultar continuamente al dispositivo, el procesador puede dedicarse a otro proceso y recibir una interrupción («timbre») cuando la operación de E/S ha concluido:

```mermaid
sequenceDiagram
    participant P as Proceso
    participant CPU as CPU
    participant D as Dispositivo
    P->>CPU: solicita E/S
    CPU->>D: encarga la operación
    rect rgb(217, 234, 211)
        CPU->>CPU: ejecuta otro proceso
    end
    D-->>CPU: timbre · interrupción de fin
    CPU-->>P: datos disponibles · vuelve a listo
```

*En lugar de consultar continuamente al dispositivo, el procesador recibe una interrupción cuando la operación de E/S ha concluido.*

### Gestor general de interrupciones

1. Guarda el **contexto** del proceso en ejecución (contenido de los registros del procesador).
2. Determina el **tipo** de interrupción (sondeo o interrupciones vectorizadas).
3. Llama a la **rutina de servicio de interrupción** específica.

### Modelo de tiempos

| Componente | Significado |
|-----------|-------------|
| `tiempo_cómputo` | tiempo invertido en el cómputo |
| `tiempo_dispositivo` | tiempo invertido en las operaciones de E/S |
| `tiempo_sobrecarga` | tiempo que el proceso invierte en determinar cuándo se completa cada operación de E/S |
| `tiempo_sondeo` | tiempo que el proceso invierte en determinar la finalización de la E/S **con sondeo** |
| `tiempo_manejador` | tiempo acumulado en ejecutar el manejador de interrupciones y las rutinas de manejo del dispositivo |
| `tiempo_listo` | tiempo que el proceso acumula esperando para usar la CPU al concluir la E/S |

```text
tiempo_total = tiempo_cómputo + tiempo_dispositivo + tiempo_sobrecarga

E/S con sondeo:          tiempo_sobrecarga = tiempo_sondeo
E/S con interrupciones:  tiempo_sobrecarga = tiempo_manejador + tiempo_listo

   tiempo_sondeo  >  tiempo_manejador + tiempo_listo
```

Con varios hilos de ejecución y solapamiento mediante interrupciones, el `tiempo_total` deja de ser la suma de los `tiempo_total` individuales: mientras un hilo hace E/S (`tiempo_dispositivo`), otro computa (`tiempo_cómputo`). **Al emplear interrupciones se reduce el tiempo medio de ejecución de cada proceso.**

## 7.3 Almacenamiento intermedio

Un **búfer** es un almacenamiento en memoria principal que se emplea en la gestión de dispositivos de E/S para **mantener su ocupación** cuando un proceso no solicita operaciones de E/S, de modo que se **solapan** el funcionamiento del dispositivo y de la CPU. Permite realizar tareas en paralelo y reducir los tiempos de espera.

- **Búfer de entrada**: almacena en MP los datos leídos del dispositivo de entrada.
- **Búfer de salida**: almacena en MP los datos a volcar sobre un dispositivo de salida.
- Los **búferes hardware** están en el **controlador** del dispositivo (registros electrónicos).
- Los sistemas de **doble búfer** disponen de búferes **hardware** en el controlador y de búferes **software** en el manejador del dispositivo.

<img src="img/doble-bufer.svg" width="520" alt="Doble búfer: mientras el dispositivo llena un búfer, el proceso vacía el otro; los papeles se alternan en el instante siguiente">

## 7.4 Tipos de discos duros

| Tipo | Descripción |
|------|-------------|
| **HDD** (*Hard Disk Drive*), discos magnéticos | Dispositivos mecánicos de superficies giratorias; gran capacidad a coste reducido, pero más lentos y frágiles. |
| **SSD** (*Solid State Drive*), estado sólido | Usan chips de memoria flash; más rápidos, duraderos, silenciosos y de menor consumo, pero más caros. La interfaz **NVMe** es un SSD ultrarrápido para conexión directa a la placa base. |
| **SSHD**, híbridos | Combinan ambas tecnologías; sus prestaciones dependen de la cantidad de memoria flash, dedicada a los datos de acceso más frecuente. |

**Tipos de conexión**: **SATA** (discos internos) · **NVMe** (SSD de alta velocidad, placa base) · **USB** (discos externos) · **tarjetas SD** (externos).

## 7.5 Planificación de discos

### Geometría del disco

En un disco magnético, el tiempo de acceso depende del movimiento del cabezal (unido al brazo actuador) y de la rotación de los platos necesaria para alcanzar el sector.

<img src="img/disco-duro-abierto.jpg" width="460" alt="Disco duro Seagate abierto, con platos, brazo actuador y cabezales visibles">

<sub>Fuente: © Raimond Spekking / CC BY-SA 4.0, vía Wikimedia Commons. [Ficha y licencia](https://commons.wikimedia.org/wiki/File:Seagate_ST9300AG_-_opened._Platter_and_head_mechanics-9324.jpg).</sub>

- Los discos magnéticos se organizan en **placas** de discos físicos, cada uno con una o dos **superficies** de almacenamiento.
- Cada superficie se divide lógicamente en **sectores** (porciones angulares del círculo, del mismo tamaño en bytes) y en **pistas** (anillos concéntricos).
- La intersección de una pista con un sector es el **sector de pista**.
- Para leer/escribir se precisa: que el disco esté **girando**, **posicionar la cabeza** en la pista seleccionada, **posicionarla sobre el sector** seleccionado y **leer/escribir** los datos.
- Las cabezas de lectura/escritura están unidas a un **mismo brazo** y se mueven en paralelo (un único motor). El conjunto de pistas sobre el que se sitúan todas las cabezas para cada superficie —al que se accede **sin mover las cabezas**— se denomina **cilindro**.
- El número de bloques (registros físicos) del disco viene dado por el número de pistas, sectores y superficies.

<img src="img/geometria-disco.svg" width="420" alt="Superficie de un disco dividida en pistas concéntricas y sectores angulares">

### Tiempo de acceso a disco

```text
t_total = t_posicionamiento + t_rotación + t_transferencia
```

- **Tiempo de búsqueda o posicionamiento**: tiempo que tarda la cabeza en posicionarse sobre la pista de acceso.
- **Retardo de rotación o latencia**: tiempo desde que la cabeza está en la pista hasta que el bloque de datos pasa por debajo de la cabeza.
- **Tiempo de transferencia**: tiempo en transferir los datos entre el disco y la memoria.

### Algoritmos de planificación

Para todos los ejemplos, cola de **peticiones: 76, 124, 17, 269, 201, 29, 137, 12** con la cabeza inicialmente en la pista **76**.

#### FCFS — *first come, first served*

Las solicitudes se sirven en el **orden de llegada** al manejador. Recorrido total: **880** pistas.

<img src="img/planificacion-disco-fcfs.svg" width="480" alt="Movimiento de la cabeza con FCFS: 76, 124, 17, 269, 201, 29, 137, 12">

#### SSTF — *shortest seek time first*

Se sirven antes las solicitudes que requieren un **menor tiempo de búsqueda**. Minimiza el tiempo de búsqueda, pero puede producir **inanición** (*starvation*). Orden de servicio: `76 → 29 → 17 → 12 → 124 → 137 → 201 → 269`. Recorrido total: **321** pistas.

<img src="img/planificacion-disco-sstf.svg" width="480" alt="Movimiento de la cabeza con SSTF: 76, 29, 17, 12, 124, 137, 201, 269">

#### Barrido (SCAN) e Inspección (LOOK)

- **SCAN**: la cabeza comienza en la pista 0 y **barre todas las pistas** de abajo arriba y de arriba abajo, atendiendo las peticiones conforme alcanza las pistas deseadas. Al alcanzar la **última pista** invierte la dirección.
- **LOOK**: como SCAN, pero al alcanzar la **pista más alta de las seleccionadas** (no la última física) invierte la dirección.
- **Barrido/inspección circular (C‑SCAN / C‑LOOK)**: el barrido se produce **siempre en el mismo sentido**, de forma circular; se reducen los tiempos de espera al evitar que la cabeza vaya y regrese hasta alcanzar la posición deseada.

Recorridos totales para la cola del ejemplo: **LOOK ≈ 512** · **SCAN ≈ 512** (con vuelta a extremo) · **C‑LOOK** y **C‑SCAN** reducen la espera media al recorrer siempre en el mismo sentido.

SCAN reduce movimientos atendiendo las solicitudes de disco mientras la cabeza avanza en una dirección, de forma parecida a un ascensor que recoge pasajeros según sube y no cambia de sentido hasta llegar al extremo:

<img src="img/planificacion-disco-scan.svg" width="480" alt="Movimiento de la cabeza con SCAN en forma de ascensor: 10, 22, 35, 61, 88, extremo, invierte sentido, 74, 40">

*SCAN reduce movimientos atendiendo las solicitudes de disco mientras la cabeza avanza en una dirección, de forma parecida a un ascensor.*

## 7.6 Caché de disco

Es un conjunto de **búferes de memoria**, cada uno del tamaño de un bloque de disco. Cuando un proceso quiere acceder a un bloque, el **sistema de ficheros** (parte del SO) busca primero una copia en la caché:

- Si la **localiza**, devuelve al proceso el número del búfer correspondiente.
- Si **no**, busca un búfer desocupado o, si no hay libres, selecciona el que lleva **más tiempo sin usarse** (LRU) y lo sustituye por el bloque pedido, leído del disco. El búfer reemplazado se **graba a disco** antes si es necesario.

Como la memoria es mucho más rápida que el disco y los bloques más frecuentes se leen (salvo la primera vez) de memoria principal, se **reduce el tiempo de E/S**.

## 7.7 Entrada/salida en UNIX

<img src="img/es-unix.svg" width="420" alt="Capas de E/S en UNIX: programas y aplicaciones, herramientas del sistema, kernel y hardware">

- **Dispositivos orientados a bloques**: se pueden **direccionar** (el programador puede leer o escribir cualquier bloque tras una operación de posicionamiento); emplean bloques de **tamaño fijo** (512 o 1024 bytes). Ejemplos: discos duros, la memoria, discos compactos, unidades de cinta.
- **Dispositivos orientados a caracteres**: trabajan con secuencias de bytes sin agrupación; **no son direccionables**; funcionan byte a byte. Ejemplos: teclado, pantalla, impresora.
- UNIX emplea programas especiales en C, los **manejadores** (*drivers*), para atender a cada familia de dispositivos. Los procesos se comunican con los dispositivos mediante llamadas a su manejador, que **aparece como si fuera un archivo** en el que se lee o escribe (gran homogeneidad).
- Cada dispositivo se estructura con **descriptores**:
  - **número mayor** (*major number*): asigna el manejador, correspondiente a una familia de dispositivos.
  - **número menor** (*minor number*): pasa al manejador como argumento y le sirve para acceder a uno de varios dispositivos físicos semejantes.
  - **clase**: de bloque o de caracteres.
- Las rutinas de E/S son **independientes de los dispositivos** y de los tipos de acceso: no hay distinción entre acceso aleatorio y secuencial, ni un tamaño de registro lógico impuesto.
- El sistema mantiene una lista de **búferes** asignados a los dispositivos de bloques; el *kernel* los usa para reducir el tráfico de E/S. Cuando un programa solicita una transferencia, se busca primero en los búferes internos por si el bloque ya está en memoria (por una lectura anterior); si es así, no se realiza la operación física.

---

## Material gráfico

Las figuras de este tema están integradas en el texto y catalogadas en [`TEORIA/IMAGENES.md`](../IMAGENES.md). Queda como **material fotográfico** adicional (ilustrativo): las fotos de discos HDD/SSD/NVMe y de conectores SATA/USB/SD y el render 3D del disco con pistas y sectores.
