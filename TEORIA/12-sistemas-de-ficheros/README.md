# Tema 8: Sistema de ficheros

Abstracción del sistema de ficheros · Conceptos de fichero y directorio · Nombrado del fichero, propietarios y permisos · Estructura y almacenamiento del fichero · Seguridad en los sistemas de ficheros · Sistemas de ficheros.

---

## Abstracción del sistema de ficheros

- Para el **usuario**, un archivo es un conjunto de datos con un nombre asociado que reside en un dispositivo de almacenamiento permanente.
- Para el **sistema operativo**, un archivo es un **tipo abstracto de datos** que permite gestionar el acceso de los procesos a los dispositivos de almacenamiento permanente.
- El archivo es la **unidad de almacenamiento** que el SO ofrece a los procesos de usuario.
- El acceso y la manipulación se realizan mediante **llamadas al sistema**. El SO puede imponer el formato de acceso a los datos (VMS, Macintosh) o no (Linux, Unix, Windows). Los sistemas de archivos hacen **transparente** el acceso a los dispositivos.

### Tipos de archivos

- Archivos **regulares**.
- **Directorios**.
- Archivos **especiales de dispositivo**: `stdin`, `stdout`, `stderr`.
- **Tuberías** (*pipes*) con nombre.
- **Enlaces simbólicos**.

Un **dispositivo RAID** es un conjunto de discos duros que actúan como una única unidad. Una **partición** es un grupo contiguo de cilindros que forman una única unidad sobre la que se sitúa un sistema de ficheros; las particiones permiten mejorar el acceso a disco, limitar el crecimiento de un sistema de ficheros y disponer de distintos sistemas operativos.

### Traducción flujo‑bloque

Un **sistema de archivos** es la estructura de datos que permite almacenar archivos en los bloques de bytes direccionables linealmente de los dispositivos. Vincula los bloques del almacenamiento para el **empaquetado** (*marshalling*) de los archivos; el **desempaquetado** (*unmarshalling*) los descompone en bloques. A este proceso se le denomina **traducción flujo‑bloque**.

<img src="img/traduccion-flujo-bloque.svg" width="520" alt="Traducción flujo-bloque entre el flujo de bytes del archivo y los bloques del dispositivo de almacenamiento">

- **Bajo nivel**: el SO solo provee traducción **flujo‑bloque**; la estructuración de los datos recae en las aplicaciones.
- **Alto nivel / estructurado**: el SO provee traducción **registro‑flujo**; requiere estructuras de datos específicas para el almacenamiento.

<img src="img/traduccion-registro-flujo-bloque.svg" width="620" alt="Cadena de traducción registro-flujo-bloque: de registros a flujo y de flujo a bloques">

## Conceptos de fichero y directorio

El sistema de archivos es un **servicio** para los usuarios: acceder a los dispositivos es incómodo, requiere conocer detalles físicos, depende de direcciones físicas (no seguro) y, a nivel físico, el usuario no tiene restricciones.

**Objetivos**: proporcionar mecanismos de **nombrado y localización** de datos no volátiles; ofrecer **primitivas de acceso** cómodas e independientes de los dispositivos; proporcionar **protección y seguridad**.

**Funciones**: fiabilidad (correcto almacenamiento y recuperación) · almacenamiento **permanente** y **estructurado** · privacidad (solo usuarios autorizados) · abstracción de los dispositivos físicos · accesibilidad mediante llamadas al sistema o librerías · **compartición** (acceso concurrente de usuarios autorizados).

### Asignación de bloques

El *driver* del disco permite ver el disco como una secuencia numerada de bloques de tamaño fijo (**clusters**). La asignación se hace buscando **tasa de transferencia elevada** y **minimizar la fragmentación**.

**Asignación de bloques adyacentes (contigua):**

- Asigna a un fichero tantos bloques **consecutivos** como necesite. Solo hay que guardar la posición del primer bloque y el número de bloques.
- Sencilla, fácil de implementar y con buen rendimiento (no hay saltos de bloques en la lectura).
- **Desventajas**: se desconoce el tamaño del fichero al crearlo; si crece por encima del espacio asignado, hay que **reubicarlo** si no hay bloques consecutivos; el disco se fragmenta y requiere **compactación** periódica costosa.

**Asignación de bloques no adyacentes:**

- Asigna tantos bloques como necesite, aunque **no** estén consecutivos. Precisa controlar los bloques libres/ocupados y qué bloques pertenecen a cada fichero.
- Gran penalización si hay que realizar muchos **posicionamientos** de la cabeza (saltos entre bloques).
- **Tamaño de bloque grande**: reduce el tiempo de posicionamiento pero aumenta la fragmentación del último bloque; no útil en ficheros pequeños.
- **Tamaño de bloque reducido**: más saltos y más tiempo de posicionamiento, pero menos fragmentación del último bloque; no útil en ficheros grandes.
- Si se conoce el tamaño medio de los ficheros se puede fijar el tamaño de bloque. Tamaños usados: **1K, 2K, 4K, 8K**.

Estructuras para gestionar los bloques no adyacentes:

| Estructura | Descripción |
|-----------|-------------|
| **Mapa de bits** | Un bit por bloque: ocupado o libre. |
| **Lista de bloques libres** | Lista de punteros a bloques libres. Un bloque de disco de 1 K puede contener 512 números de bloque de 16 bits. |
| **Listas de archivo** | Las primeras palabras de cada bloque contienen la dirección del siguiente bloque. Fácil de implementar; ineficiente en acceso directo; mezcla información de control y datos. |
| **Nodos índice** | Cada archivo tiene una tabla que describe sus propiedades y la ubicación de sus bloques. La tabla puede quedarse pequeña y requerir distintos niveles de direccionamiento. |

Como plazas de aparcamiento: la asignación **contigua** ocupa plazas consecutivas (favorece el acceso rápido); la **enlazada** deja que cada plaza señale la siguiente (no exige continuidad); la **indexada** concentra todas las referencias en una estructura específica.

<img src="img/asignacion-bloques-aparcamiento.svg" width="620" alt="Comparación de estrategias de asignación de bloques como plazas de aparcamiento: contigua, enlazada e indexada">

*La asignación contigua favorece el acceso rápido; la enlazada evita exigir continuidad; la indexada concentra las referencias en una estructura específica.*

### Directorios

- En muchos casos son un **tipo especial de archivo**. Organizan el acceso a los ficheros y almacenan información de los archivos que agrupan.
- Evolución: un único directorio → un directorio por usuario (multiusuario) → **estructura arbórea jerárquica** (sistemas actuales).
- Rutas de acceso: **absolutas** (desde la raíz del sistema de ficheros) o **relativas** (desde el directorio de trabajo indicado en el PCB del proceso).

### Ejemplo: resolución de `/usr/ast/mbox` con nodos‑i

El sistema de ficheros funciona como una biblioteca: el nombre se busca en un catálogo (el directorio), que remite a una ficha (el inodo) con los metadatos y las referencias a los estantes (los bloques) donde están los datos.

```mermaid
flowchart LR
    N(["Nombre solicitado<br/>/apuntes/tema.pdf"]) --> C["Catálogo · directorio<br/>tema.pdf → inodo 481"]
    C --> I["Ficha · inodo 481<br/>tipo · permisos · tamaño · punteros"]
    I --> E1[("Estante · bloque 120")]
    I --> E2[("Estante · bloque 905")]
    I --> E3[("Estante · bloque 411")]

    classDef solicitud fill:#eef2f7,stroke:#444,color:#222;
    classDef catalogo fill:#fdf3d0,stroke:#a06a1a,color:#222;
    classDef inodo fill:#cfe2f3,stroke:#2b6f99,color:#222;
    classDef bloque fill:#d9d9d9,stroke:#555,color:#222;

    class N solicitud;
    class C catalogo;
    class I inodo;
    class E1,E2,E3 bloque;
```

*El nombre se almacena en el directorio; el inodo conserva los metadatos y las referencias a los bloques que contienen los datos.*

| Paso | Estructura | Contenido relevante | Resultado |
|------|-----------|---------------------|-----------|
| 1 | Directorio raíz (bloque de `/`) | `1 .` · `1 ..` · `4 bin` · `7 dev` · `14 lib` · `9 etc` · `6 usr` · `8 tmp` | la búsqueda de `usr` produce el **nodo‑i 6** |
| 2 | Nodo‑i 6 | modo · tamaño · tiempos · datos en el **bloque 132** | `/usr` está en el bloque 132 |
| 3 | Bloque 132 (directorio de `/usr`) | `6 .` · `1 ..` · `19 dick` · `30 erik` · `51 jim` · `26 ast` · `45 bal` | `/usr/ast` es el **nodo‑i 26** |
| 4 | Nodo‑i 26 | modo · tamaño · tiempos · datos en el **bloque 406** | `/usr/ast` está en el bloque 406 |
| 5 | Bloque 406 (directorio de `/usr/ast`) | `26 .` · `6 ..` · `64 grants` · `92 books` · `60 mbox` · `81 minix` · `17 src` | `/usr/ast/mbox` es el **nodo‑i 60** |

## Nombrado del fichero, propietarios y permisos

- La mayoría de los sistemas de archivos modernos permiten asignar **permisos** (derechos de acceso) a usuarios y grupos, restringiendo o permitiendo visualización, modificación y/o ejecución.
- **UNIX, POSIX, Linux y macOS X** tienen un sistema simple para archivos individuales. POSIX especifica **listas de control de acceso (ACL)**, pero solo lo implementan ciertos sistemas.
- Las variantes de **DOS** no implementan ningún sistema de permisos.
- **Microsoft Windows**, así como VMS y OpenVMS, emplean **ACL** para un conjunto de permisos más complejo y variado.

### Notación simbólica en Linux

**Permisos**: `r` lectura · `w` escritura · `x` ejecución.

**Símbolo de tipo** (primer carácter):

| Símbolo | Tipo |
|---------|------|
| `-` | archivo regular |
| `d` | directorio |
| `b` | archivo especial de bloques |
| `c` | archivo especial de caracteres |
| `l` | enlace simbólico |
| `p` | tubo con nombre (*named pipe*) |
| `s` | socket de dominio |

Los permisos actúan como llaves diferenciadas: cada categoría de usuario (propietario, grupo, otros) recibe su propio juego de derechos de lectura, escritura y ejecución, que `chmod` modifica.

```mermaid
flowchart LR
    F["fichero<br/>rwx rw- r--"] --> U["propietario<br/>leer · escribir · ejecutar"]
    F --> G["grupo<br/>leer · escribir"]
    F --> O["otros<br/>solo leer"]
    CH[["chmod 764 fichero"]] -. modifica .-> F

    classDef fichero fill:#cfe2f3,stroke:#2b6f99,color:#222;
    classDef permiso fill:#d9ead3,stroke:#3a7a3a,color:#222;
    classDef comando fill:#fce5a8,stroke:#a06a1a,color:#222;

    class F fichero;
    class U,G,O permiso;
    class CH comando;
```

*Los permisos determinan qué operaciones puede realizar cada categoría de usuario sobre un fichero.*

## Estructura y almacenamiento del fichero

### Primitivas de acceso

| Acceso de bajo nivel | Acceso estructurado (secuencial / orientado a registros) | Secuencial indexado |
|----------------------|---------------------------------------------------------|---------------------|
| `abrir(nombreArchivo)` | `abrir(nombreArchivo)` | `obtenerRegistro(IDArchivo, indice)` |
| `cerrar(IDArchivo)` | `cerrar(IDArchivo)` | `ponerRegistro(IDArchivo, registro)` |
| `leer(IDArchivo, bufer, longitud)` | `obtenerRegistro(IDArchivo, registro)` | `borrarRegistro(IDArchivo, indice)` |
| `escribir(IDArchivo, bufer, longitud)` | `ponerRegistro(IDArchivo, registro)` | |
| `buscar(IDArchivo, posicionArchivo)` | `buscar(IDArchivo, posicionArchivo)` | |

### Descriptor de fichero

Al acceder a un archivo, el gestor de ficheros crea una instancia de un **descriptor de fichero** que incluye: nombre · compartible · propietario · indicadores de protección · longitud · tiempo de creación · tiempo de última modificación · tiempo de último acceso · cuenta de referencias · detalles del dispositivo de almacenamiento. Para el **descriptor de archivo abierto** se añaden: bloqueos · estado actual · usuario.

### Bloques y agrupaciones

- **Bloque**: agrupación lógica de sectores de disco; **unidad de transferencia mínima** del sistema de archivos. Optimiza la eficiencia de la E/S. Todos los SO ofrecen un tamaño de bloque por defecto; el usuario puede fijarlo con el mandato **`mkfs`**.
- **Agrupación** (*cluster*): conjunto de bloques que se gestionan como una unidad lógica de almacenamiento. Con agrupaciones y bloques grandes aparece **fragmentación interna**. Unidad de asignación: `1 agrupación = X bloques`.

### Formato de las entradas de directorio

| Sistema | Estructura de la entrada (bytes) |
|---------|--------------------------------|
| **UNIX** | 2 (número del nodo‑i) + 14 (nombre del archivo) |
| **MS‑DOS** | 8 (nombre) + 3 (extensión) + 1 (atributos) + 10 (reservado) + 2 (tiempo) + 2 (fecha) + 2 (primer número de bloque) + 4 (tamaño) |

### Estructura del nodo‑i (UNIX)

El nodo‑i de un archivo contiene: nodo del archivo · número de enlaces · UID y GID del propietario · tamaño del archivo · tiempos de creación, último acceso y última modificación · **10 números de bloque del disco** (punteros **directos**) · un puntero **indirecto individual** · un puntero **indirecto doble** · un puntero **indirecto triple**.

<img src="img/nodo-i-indirecciones.svg" width="560" alt="Nodo-i con 10 punteros directos a bloques de datos y punteros indirecto individual, doble y triple">

### Superbloque y mapas de bits

- **Superbloque**: descripción del sistema de archivos, parámetros…
- **Mapa de bits**: un bit que indica si un recurso está ocupado o no. Hay **mapa de bits de nodos‑i** y **mapa de bits de bloques** (de datos: archivos/directorios).

### Gestión del espacio libre

| Método | Comentario |
|--------|-----------|
| **Mapas de bits** | Un bit por recurso (descriptor de archivo, bloque o agrupación): libre = 1, ocupado = 0. Fácil y sencillo; eficiente si el dispositivo no está muy lleno ni muy fragmentado. |
| **Listas de recursos libres** | Lista enlazada con todos los recursos disponibles. Ineficiente salvo para dispositivos muy llenos y fragmentados. |
| **Uso de agrupaciones** | Gestión por unidades lógicas mayores. |

### Caché de bloques

Al leer un bloque: (1) comprobar si está en la caché; si no, leerlo del dispositivo y copiarlo a la caché — si está llena, se **reemplaza** un bloque según la política. (2) Si el bloque ha sido escrito (**sucio**), se aplica la **política de escritura**.

- **Políticas de reemplazo**: FIFO · segunda oportunidad · MRU (*Most Recently Used*) · **LRU** (la más frecuente). Los bloques más usados tienden a permanecer en la caché; el uso estricto de LRU puede crear problemas de fiabilidad si el ordenador falla.
- **Políticas de escritura**:

| Política | Descripción | Compromiso |
|----------|-------------|-----------|
| **Escritura inmediata** (*write‑through*) | Se escribe cada vez que se modifica el bloque. | Sin problema de fiabilidad; reduce el rendimiento. |
| **Escritura diferida** (*write‑back*) | Solo se escribe a disco cuando el bloque se elige para reemplazo por falta de espacio. | Optimiza el rendimiento; problemas de fiabilidad. |
| **Escritura retrasada** (*delayed‑write*) | Escribe los bloques modificados a disco periódicamente. Los bloques especiales se escriben de inmediato. Hay que **desmontar** el volumen antes de extraerlo. | Compromiso entre rendimiento y fiabilidad; reduce el alcance de los daños. |
| **Escritura al cierre** (*write‑on‑close*) | Al cerrar un archivo, se vuelcan sus bloques actualizados. | |

### Puntos de montaje

- **Punto de montaje**: directorio sobre el que se monta un sistema de ficheros. No todos los ficheros del árbol están en el mismo dispositivo.
- El *kernel* guarda la información de particiones y puntos de montaje en `/proc/mounts` y `/etc/mtab`. Se consultan con `mount` y `df`. Montaje automatizado: `/etc/fstab`.

### Estructura del árbol en Linux/UNIX

- La raíz del árbol jerárquico es **`/`**. El árbol se compone de directorios que contienen ficheros.
- Todo directorio contiene al menos **`.`** (el propio directorio) y **`..`** (enlace a su directorio padre).
- Cada directorio y su contenido pueden pertenecer a un punto de montaje distinto.

## Sistemas de ficheros

| SO | Sistemas de ficheros |
|----|----------------------|
| Unix | Unix System V, BSD |
| Linux | ext2, ext3, ext4, XFS, JFS, ReiserFS, Btrfs |
| Solaris | VxFS, QFS, UFS, ZFS |
| macOS X | HFS Plus, UFS |
| Windows | FAT16, FAT32, NTFS |

### Comparativa

| | FAT16 | FAT32 | HFS+ | ext3 | NTFS 5.0 | NTFS 6.0 | ext4 |
|---|---|---|---|---|---|---|---|
| Año de creación | 1984 | 1996 | 1998 | 1999 | 2001 | 2006 | 2006 |
| Empresa | Microsoft | Microsoft | Apple | Stephen Tweedie | Microsoft | Microsoft | Varios |
| SO inicial | MS‑DOS 3 | Windows 95 | Mac OS 8.1 | Linux 2.4.15 | Windows XP | Windows Vista | Linux 2.6.19 |
| Tamaño máx. de nombre | 8+3 | 8+3 | 255 car. UTF‑16 | 255 bytes | 255 car. | 255 car. | 256 bytes |
| Tamaño máx. de fichero | 2/4 GB | 4 GB | 8 EB | 2 TB | 16 EB | 16 EB | 16 TB |
| Tamaño máx. de partición | 2/4 GB | 2/16 TB | 8 EB | 32 TB | 16 EB | 16 EB | 1 EB |

### Sistemas de ficheros *journaled* o transaccionales

Tratan los cambios para crear, modificar o borrar un fichero como una **base de datos**, garantizando un comportamiento **transaccional** y almacenando un histórico. Propiedades **ACID**: Atomicidad, Consistencia, Aislamiento y Durabilidad.

- El **histórico** (*journal*) es una lista de transacciones que permite **reconstruir** el sistema de ficheros ante un error, devolviéndolo a su último estado consistente registrado.
- Minimiza el tiempo de recuperación y **evita** herramientas como `fsck` (solo se verifican los últimos cambios en los metadatos). Resuelve problemas de escalabilidad.
- Toda operación que modifique metadatos y datos de un archivo se **agrupa en la misma transacción**. Si el sistema falla, las acciones parcialmente realizadas se deshacen o se completan con el *log*. No se garantiza que el sistema esté **actualizado** al terminar la recuperación, sino que es **consistente**.
- Ejemplos: ReiserFS/Reiser4, ext3, ext4 (Linux) · NTFS (Windows) · UFS (Solaris) · XFS (IRIX/Linux) · JFS (Linux, IBM, OS/2, AIX) · HFS+ (macOS X).

### Sistemas de ficheros concretos

| Sistema | Características |
|---------|----------------|
| **FAT32** | Estándar de facto. Archivos individuales ≤ 4 GB; partición < 8 TB; nombrado solo con caracteres de la gramática inglesa; **sin características de seguridad**. Adecuado para unidades flash USB o discos externos, no para disco duro interno. |
| **exFAT** | Tabla de Asignación de Archivos Extendida. Similar a FAT32 pero permite archivos > 4 GB. Accesible en Linux, Windows y macOS X. |
| **NTFS** | Seguridad robusta: cifrado (**EFS**, *Encrypting File System*, con clave pública) que impide el acceso no autorizado. Admite cualquier símbolo UTF. Unidades de 40 GB a 2 TB. Grupos de archivos de 4 KB. Compresión. Permisos de usuario para archivos y carpetas. |
| **EXT3** | Transaccional (*journal*). Seguridad razonable. Compatible con EXT2. Bajo consumo de CPU. Límite del sistema de archivos: 2³² bloques. |
| **EXT4** | Transaccional. Volúmenes de hasta **1024 PiB** y ficheros de hasta **16 TiB**. Menor consumo de CPU que EXT3. Mayor velocidad de lectura/escritura. Emplea **extents** (conjuntos de bloques físicos contiguos). Permite montar sistemas EXT3. Escritura retrasada. Límite de 64 000 subdirectorios. |

**Límites de EXT3 según el tamaño de bloque:**

| Tamaño de bloque | Tamaño máximo de archivo | Tamaño máximo del sistema de ficheros |
|------------------|--------------------------|--------------------------------------|
| 1 KiB | 16 GiB | 2 TiB |
| 2 KiB | 256 GiB | 8 TiB |
| 4 KiB | 2 TiB | 16 TiB |
| 8 KiB | 2 TiB | 32 TiB |

### Virtual File System (VFS)

El **VFS** ofrece a las aplicaciones una **interfaz uniforme** de llamadas al sistema, aunque los datos estén almacenados con formatos diferentes. Bajo el **servidor de archivos** y el VFS, el **módulo organizador de archivos** integra los distintos sistemas (`ext2`, `vfat`, `reiserfs`, …, `proc`) y accede al **servidor de bloques**.

<img src="img/vfs-capas.svg" width="560" alt="Arquitectura en capas del VFS: procesos, servidor de archivos, VFS, módulo organizador de archivos con ext2, vfat, reiserfs y proc, y servidor de bloques">

Visto como un adaptador universal: las aplicaciones usan siempre las mismas llamadas (`open`, `read`, `write`, `close`) y el VFS las reparte hacia el sistema de ficheros concreto (ext4, NTFS, FAT, procfs…) y, de ahí, hacia el dispositivo real.

<img src="img/vfs-adaptador.svg" width="520" alt="El VFS como adaptador: aplicaciones sobre una interfaz uniforme que reparte hacia ext4, NTFS, FAT y procfs, y de ahí a los dispositivos reales">

*El sistema de ficheros virtual ofrece una interfaz uniforme aunque los datos estén almacenados con formatos diferentes.*

## Seguridad en los sistemas de ficheros

El sistema de ficheros debe **garantizar la consistencia** de los datos y que solo los usuarios **autorizados** accedan a ellos para operaciones autorizadas (lectura, escritura, ejecución).

| Soluciones hardware | Soluciones software |
|---------------------|---------------------|
| Controladores que tratan con sectores con fallos | *Backups* (copias de respaldo) en discos y en cintas; *backups* incrementales |
| Discos con información **redundante (RAID)** | **Replicación** |

### RAID

Un **RAID** (*Redundant Array of Independent Disks*) combina varios discos que actúan como una única unidad, para mejorar el rendimiento y/o la tolerancia a fallos.

**RAID 0** (*striping*): divide los datos entre los discos.

<img src="img/raid-0.svg" width="300" alt="RAID 0: dos discos con los bloques repartidos, sin redundancia">

**RAID 1** (*mirroring*): duplica los datos en dos discos.

<img src="img/raid-1.svg" width="300" alt="RAID 1: dos discos con contenido idéntico (espejo)">

**RAID 5**: división a nivel de bloques con **paridad distribuida** entre todos los discos (`Xp` = bloque de paridad).

<img src="img/raid-5.svg" width="520" alt="RAID 5: cuatro discos con bloques de datos y paridad distribuida">

**Niveles de RAID:**

| Nivel | Técnica | Tolerancia a fallos | Notas |
|-------|---------|---------------------|-------|
| **RAID 0** | *Striping* (nivel de bloque) | **Ninguna** | Buen rendimiento de lectura/escritura; aprovecha toda la capacidad; discos homogéneos; si falla una unidad se pierden **todos** los datos. No para sistemas críticos. |
| **RAID 1** | *Mirroring* | 1 disco | Muy simple; buena velocidad de lectura/escritura; ante un fallo basta hacer una nueva copia; capacidad eficaz = **mitad**; el intercambio en caliente no siempre es posible. |
| **RAID 2** | *Striping* a nivel de **bit** + disco de paridad + código de **Hamming** | corrección de errores | Apenas se emplea; no atiende varias peticiones simultáneas (discos paralelos pero no independientes). |
| **RAID 3** | *Striping* a nivel de **byte** + disco de paridad dedicado | 1 disco | No se emplea; la controladora sincroniza los discos; tasas de transferencia muy altas; una petición ocupa todos los discos de datos. |
| **RAID 4** | *Striping* a nivel de **bloque** + disco de paridad dedicado | 1 disco | Mínimo 3 discos; permite varias lecturas simultáneas; el disco de paridad es el **cuello de botella**. |
| **RAID 5** | *Striping* a nivel de bloque + **paridad distribuida** | 1 disco | Mínimo 3 discos; bajo coste de redundancia; paridad normalmente por hardware; las **escrituras son costosas**. |
| **RAID 6** | RAID 5 + **segundo bloque de paridad** | 2 discos | Amplía RAID 5. |
| **RAID 0+1** | *Mirror* de *stripes* | | Matriz anidada. |
| **RAID 1+0** (RAID 10) | *Stripe* de *mirrors* | | Matriz anidada. |
| **RAID 1E**, **RAID 50**, **RAID 100** | Matrices RAID combinadas | | Configuraciones híbridas. |

Información adicional: [Seagate — modos RAID](http://www.seagate.com/es/es/manuals/network-storage/business-storage-nas-os/raid-modes/) · [Wikipedia — RAID](https://es.wikipedia.org/wiki/RAID) · [prepressure.com — RAID](https://www.prepressure.com/library/technology/raid) · [IONOS — RAID](https://www.ionos.es/digitalguide/servidores/seguridad/raid/)

---

## Material gráfico

Las figuras de este tema están integradas en el texto y catalogadas en [`TEORIA/IMAGENES.md`](../IMAGENES.md). Queda como **material fotográfico** adicional (ilustrativo): fotos de cabinas y *enclosures* RAID. Los diagramas de RAID de Wikipedia (RAID 2, 3, 4, 6, 0+1, 1+0, 1E, 50, 100) se resumen en la tabla de niveles.
