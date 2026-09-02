# Tema 7: Dispositivos de Entrada/Salida

7.1 Conceptos básicos · 7.2 Funciones de entrada/salida · 7.3 Almacenamiento intermedio · 7.4 Tipos de discos duros · 7.5 Planificación de discos · 7.6 Caché de disco · 7.7 Entrada/salida en Linux.

---

## 7.1 Conceptos básicos

- Los **controladores** son la interfaz **software** entre el hardware del dispositivo y el sistema operativo.
- Para lograr **homogeneidad** en el conjunto de instrucciones que el SO puede invocar sobre un dispositivo, se dispone de **manejadores de dispositivos**: un conjunto de funciones que abstraen el funcionamiento de un controlador concreto.
- Se construyen **APIs** con las funciones básicas de acceso a un dispositivo: `open`, `close`, `control`, `seek`… El manejador exporta al SO una **interfaz común de llamadas al sistema** para los dispositivos.

```mermaid
flowchart TD
    PU[Programas de usuario] --> RSO[Rutinas del sistema operativo]
    RSO --> MD[Manejadores de dispositivos]
    MD --> CMI[Controladores y manejadores de interrupciones]
```

- Las operaciones de lectura y escritura son **secuenciales**. Interesa **paralelizar** al máximo las actividades de las aplicaciones durante los intervalos de acceso a los dispositivos ⇒ **solapamiento de E/S y procesador**. Requisitos:
  - El lenguaje de programación y el SO deben permitir que el hilo **inicie** una operación de E/S y **sondee** si ha concluido.
  - El hilo debe poder realizar otros trabajos mientras se llevan a cabo las operaciones de E/S.

### Características de los dispositivos

- **Unidad de transferencia**: los datos pueden intercambiarse como flujos de bytes/caracteres de longitud variable (**dispositivos modo carácter**, p. ej. teclado) o como flujos de bloques de tamaño fijo (**dispositivos modo bloque**, p. ej. disco duro).
- **Velocidad de transmisión**: muy variable según la naturaleza del dispositivo.
- **Utilidad**, **complejidad de control** y **representación de los datos**.

## 7.2 Funciones de entrada/salida

- El acceso mediante **E/S directa** transfiere los datos de entrada desde el controlador a un registro de la CPU y de éste a memoria principal (ídem para la salida).
- El acceso mediante **DMA** transfiere los datos entre el controlador y la memoria principal **directamente**.
- Si el dispositivo dispone de **interrupciones**, el software no necesita recurrir a operaciones periódicas de **sondeo**.

**Estrategias de acceso**: E/S directa con sondeo · E/S por DMA con sondeo *(muy poco frecuente)* · E/S directa con interrupciones · E/S por DMA con interrupciones.

### E/S con sondeo (lectura)

El proceso solicita la lectura y **espera activamente** consultando el registro de estado del controlador hasta que el dato está disponible:

```mermaid
sequenceDiagram
    participant P as Proceso / función leer
    participant C as Controlador del dispositivo
    P->>C: 1-2. leer(dispositivo, …): programa la operación
    P->>C: 3. escribe la orden en el registro «Operación»
    loop 4. sondeo
        P->>C: consulta el registro «Estado»
        C-->>P: ¿operación completada?
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
    Note over P: el proceso queda BLOQUEADO; la CPU ejecuta otro proceso
    C-->>MI: 6. interrupción al completar
    MI->>T: 7. actualiza la tabla de estado
    MI->>D: 8a-8b. copia los datos
    D-->>P: 9. proceso LISTO (datos disponibles)
```

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

```mermaid
flowchart BT
    D1[Dispositivo] --> C1["Controlador: búfer HW (A/B)"]
    C1 --> M1["Manejador: búfer SW (A/B)"]
    M1 --> P1((Proceso))
```

## 7.4 Tipos de discos duros

| Tipo | Descripción |
|------|-------------|
| **HDD** (*Hard Disk Drive*), discos magnéticos | Dispositivos mecánicos de superficies giratorias; gran capacidad a coste reducido, pero más lentos y frágiles. |
| **SSD** (*Solid State Drive*), estado sólido | Usan chips de memoria flash; más rápidos, duraderos, silenciosos y de menor consumo, pero más caros. La interfaz **NVMe** es un SSD ultrarrápido para conexión directa a la placa base. |
| **SSHD**, híbridos | Combinan ambas tecnologías; sus prestaciones dependen de la cantidad de memoria flash, dedicada a los datos de acceso más frecuente. |

**Tipos de conexión**: **SATA** (discos internos) · **NVMe** (SSD de alta velocidad, placa base) · **USB** (discos externos) · **tarjetas SD** (externos).

## 7.5 Planificación de discos

### Geometría del disco

- Los discos magnéticos se organizan en **placas** de discos físicos, cada uno con una o dos **superficies** de almacenamiento.
- Cada superficie se divide lógicamente en **sectores** (porciones angulares del círculo, del mismo tamaño en bytes) y en **pistas** (anillos concéntricos).
- La intersección de una pista con un sector es el **sector de pista**.
- Para leer/escribir se precisa: que el disco esté **girando**, **posicionar la cabeza** en la pista seleccionada, **posicionarla sobre el sector** seleccionado y **leer/escribir** los datos.
- Las cabezas de lectura/escritura están unidas a un **mismo brazo** y se mueven en paralelo (un único motor). El conjunto de pistas sobre el que se sitúan todas las cabezas para cada superficie —al que se accede **sin mover las cabezas**— se denomina **cilindro**.
- El número de bloques (registros físicos) del disco viene dado por el número de pistas, sectores y superficies.

<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 420 300" font-family="sans-serif" font-size="13" role="img" aria-label="Superficie de un disco dividida en pistas concéntricas y sectores angulares">
  <rect width="420" height="300" fill="#ffffff"/>
  <g transform="translate(150,160)">
    <circle r="120" fill="#2f8f8f" stroke="#1f6060"/>
    <circle r="90" fill="none" stroke="#bfe3e3"/>
    <circle r="60" fill="none" stroke="#bfe3e3"/>
    <circle r="30" fill="none" stroke="#bfe3e3"/>
    <circle r="16" fill="#1f6fd0" stroke="#144a90"/>
    <!-- radios (sectores) -->
    <g stroke="#bfe3e3">
      <line x1="0" y1="0" x2="0" y2="-120"/>
      <line x1="0" y1="0" x2="104" y2="-60"/>
      <line x1="0" y1="0" x2="104" y2="60"/>
      <line x1="0" y1="0" x2="0" y2="120"/>
      <line x1="0" y1="0" x2="-104" y2="60"/>
      <line x1="0" y1="0" x2="-104" y2="-60"/>
    </g>
    <!-- un sector resaltado -->
    <path d="M0 0 L0 -120 A120 120 0 0 1 104 -60 Z" fill="#ffe14d" fill-opacity="0.7" stroke="#b59a00"/>
  </g>
  <text x="300" y="70" font-size="12">sector (porción angular)</text>
  <line x1="300" y1="75" x2="235" y2="95" stroke="#333"/>
  <text x="300" y="175" font-size="12">pista (anillo concéntrico)</text>
  <line x1="300" y1="170" x2="238" y2="150" stroke="#333"/>
  <text x="300" y="255" font-size="12">eje / centro del disco</text>
  <line x1="300" y1="250" x2="160" y2="165" stroke="#333"/>
</svg>

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

<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 480 300" font-family="sans-serif" font-size="12" role="img" aria-label="Movimiento de la cabeza con FCFS: 76, 124, 17, 269, 201, 29, 137, 12">
  <rect width="480" height="300" fill="#ffffff"/>
  <line x1="55" y1="20" x2="55" y2="250" stroke="#333"/>
  <line x1="55" y1="250" x2="460" y2="250" stroke="#333"/>
  <text x="10" y="25">300</text><text x="20" y="250">0</text>
  <!-- escala vertical: pista 0 -> y250, pista 300 -> y20; y = 250 - pista*(230/300) -->
  <!-- puntos en x acumulado: 0,48,155,407,475,647,755,880 -> x = 55 + cum*(400/880) -->
  <polyline fill="none" stroke="#2b4a8b" stroke-width="1.6"
    points="55,192 77,155 104,237 240,44 271,96 348,228 396,145 462,241"/>
  <g fill="#c0392b">
    <circle cx="55" cy="192" r="3"/><circle cx="77" cy="155" r="3"/><circle cx="104" cy="237" r="3"/>
    <circle cx="240" cy="44" r="3"/><circle cx="271" cy="96" r="3"/><circle cx="348" cy="228" r="3"/>
    <circle cx="396" cy="145" r="3"/><circle cx="462" cy="241" r="3"/>
  </g>
  <g fill="#333">
    <text x="45" y="188">76</text><text x="80" y="150">124</text><text x="108" y="240">17</text>
    <text x="244" y="42">269</text><text x="275" y="94">201</text><text x="352" y="232">29</text>
    <text x="400" y="142">137</text><text x="452" y="238">12</text>
  </g>
</svg>

#### SSTF — *shortest seek time first*

Se sirven antes las solicitudes que requieren un **menor tiempo de búsqueda**. Minimiza el tiempo de búsqueda, pero puede producir **inanición** (*starvation*). Orden de servicio: `76 → 29 → 17 → 12 → 124 → 137 → 201 → 269`. Recorrido total: **321** pistas.

<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 480 300" font-family="sans-serif" font-size="12" role="img" aria-label="Movimiento de la cabeza con SSTF: 76, 29, 17, 12, 124, 137, 201, 269">
  <rect width="480" height="300" fill="#ffffff"/>
  <line x1="55" y1="20" x2="55" y2="250" stroke="#333"/>
  <line x1="55" y1="250" x2="460" y2="250" stroke="#333"/>
  <text x="10" y="25">300</text><text x="20" y="250">0</text>
  <!-- cum: 0,47,59,64,176,189,253,321 -> x = 55 + cum*(400/321) ; pistas 76,29,17,12,124,137,201,269 -->
  <polyline fill="none" stroke="#2b4a8b" stroke-width="1.6"
    points="55,192 114,228 130,237 135,241 194,155 210,145 260,96 384,44"/>
  <g fill="#c0392b">
    <circle cx="55" cy="192" r="3"/><circle cx="114" cy="228" r="3"/><circle cx="130" cy="237" r="3"/>
    <circle cx="135" cy="241" r="3"/><circle cx="194" cy="155" r="3"/><circle cx="210" cy="145" r="3"/>
    <circle cx="260" cy="96" r="3"/><circle cx="384" cy="44" r="3"/>
  </g>
  <g fill="#333">
    <text x="45" y="188">76</text><text x="106" y="244">29</text><text x="118" y="252">17</text>
    <text x="138" y="255">12</text><text x="180" y="152">124</text><text x="204" y="140">137</text>
    <text x="264" y="94">201</text><text x="388" y="42">269</text>
  </g>
</svg>

#### Barrido (SCAN) e Inspección (LOOK)

- **SCAN**: la cabeza comienza en la pista 0 y **barre todas las pistas** de abajo arriba y de arriba abajo, atendiendo las peticiones conforme alcanza las pistas deseadas. Al alcanzar la **última pista** invierte la dirección.
- **LOOK**: como SCAN, pero al alcanzar la **pista más alta de las seleccionadas** (no la última física) invierte la dirección.
- **Barrido/inspección circular (C‑SCAN / C‑LOOK)**: el barrido se produce **siempre en el mismo sentido**, de forma circular; se reducen los tiempos de espera al evitar que la cabeza vaya y regrese hasta alcanzar la posición deseada.

Recorridos totales para la cola del ejemplo: **LOOK ≈ 512** · **SCAN ≈ 512** (con vuelta a extremo) · **C‑LOOK** y **C‑SCAN** reducen la espera media al recorrer siempre en el mismo sentido.

## 7.6 Caché de disco

Es un conjunto de **búferes de memoria**, cada uno del tamaño de un bloque de disco. Cuando un proceso quiere acceder a un bloque, el **sistema de ficheros** (parte del SO) busca primero una copia en la caché:

- Si la **localiza**, devuelve al proceso el número del búfer correspondiente.
- Si **no**, busca un búfer desocupado o, si no hay libres, selecciona el que lleva **más tiempo sin usarse** (LRU) y lo sustituye por el bloque pedido, leído del disco. El búfer reemplazado se **graba a disco** antes si es necesario.

Como la memoria es mucho más rápida que el disco y los bloques más frecuentes se leen (salvo la primera vez) de memoria principal, se **reduce el tiempo de E/S**.

## 7.7 Entrada/salida en UNIX

```mermaid
flowchart TD
    APP["Programas y aplicaciones de otros niveles"] --> TOOLS
    subgraph TOOLS["Programas de sistema y de aplicación"]
        CC["cc (cpp · comp · as · ld)"]
        SH["sh · a.out · date · vi · …"]
    end
    TOOLS --> K[Kernel]
    K --> HW[Hardware]
```

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

## Galería visual complementaria

### T11.1 · Dispositivos de bloque y de carácter

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
```

*Los dispositivos de bloque transfieren unidades direccionables de datos; los de carácter producen o consumen secuencias continuas.*

### T11.2 · Anatomía de un disco duro

![Disco duro Seagate abierto, con platos, brazo actuador y cabezales visibles](img/disco-duro-abierto.jpg)

*En un disco magnético, el tiempo de acceso depende del movimiento del cabezal y de la rotación necesaria para alcanzar el sector.*

<sub>Fuente: © Raimond Spekking / CC BY-SA 4.0, vía Wikimedia Commons. [Ficha y licencia](https://commons.wikimedia.org/wiki/File:Seagate_ST9300AG_-_opened._Platter_and_head_mechanics-9324.jpg).</sub>

### T11.3 · SCAN como ascensor

```mermaid
flowchart LR
    C10[cilindro 10] --> C22[22 · petición]
    C22 --> C35[35 · petición]
    C35 --> C61[61 · petición]
    C61 --> C88[88 · petición]
    C88 --> E[extremo]
    E -->|invierte el sentido| C74[74 · petición]
    C74 --> C40[40 · petición]
```

*SCAN reduce movimientos atendiendo las solicitudes de disco mientras la cabeza avanza en una dirección, de forma parecida a un ascensor.*

### T11.4 · DMA: ruta directa entre dispositivo y memoria

```mermaid
sequenceDiagram
    participant CPU as CPU
    participant DMA as Controlador DMA
    participant D as Dispositivo
    participant M as Memoria RAM
    CPU->>DMA: origen, destino y tamaño
    DMA->>D: inicia transferencia
    loop bloque completo
        D->>M: dato directo a memoria
    end
    DMA-->>CPU: interrupción de fin
```

*El acceso directo a memoria permite transferir bloques sin que la CPU tenga que copiar personalmente cada palabra.*

### T11.5 · E/S dirigida por interrupciones

```mermaid
sequenceDiagram
    participant P as Proceso
    participant CPU as CPU
    participant D as Dispositivo
    P->>CPU: solicita E/S
    CPU->>D: encarga la operación
    CPU->>CPU: ejecuta otro proceso
    D-->>CPU: timbre · interrupción de fin
    CPU-->>P: datos disponibles · vuelve a listo
```

*En lugar de consultar continuamente al dispositivo, el procesador recibe una interrupción cuando la operación de E/S ha concluido.*

---

## Material gráfico

Todos los diagramas del Tema 7 están replicados como mermaid, SVG o tabla dentro de este documento (capas de la E/S, E/S con sondeo y con interrupciones, doble búfer, geometría del disco, FCFS y SSTF, arquitectura UNIX). Queda como **material fotográfico** (ilustrativo): las fotos de discos HDD/SSD/NVMe y de conectores SATA/USB/SD y el render 3D del disco con pistas y sectores.
