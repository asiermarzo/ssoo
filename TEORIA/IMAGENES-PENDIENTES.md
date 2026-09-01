# Material gráfico: estado por tema

Registro del tratamiento de las figuras del PDF *Sistemas Operativos — 240304-25-26*
(J. J. Astrain).

> Este registro se conserva para las figuras procedentes del PDF original. La nueva galería
> complementaria de 57 imágenes, junto con sus fuentes y prompts, se documenta en
> [`IMAGENES.md`](IMAGENES.md).

**Criterio:** los diagramas (flujo, capas, estados, bloques, cronogramas, rutas de datos…) se
replican con **mermaid** o **SVG embebido** directamente en el `README.md` de cada tema. Solo
se deja pendiente el **material fotográfico** o las **imágenes externas** (fotos históricas,
fotos de hardware, capturas de pantalla, *timelines* con copyright), que el profesor puede
añadir o no.

Para ese material pendiente, cada entrada incluye un *prompt* para regenerar una imagen
equivalente con ChatGPT si se desea. Flujo: el usuario genera la imagen, la pega aquí, y
Claude la guarda en `TEORIA/NN-*/img/` y la enlaza en el `README.md`.

---

## Tema 1 — Introducción a los sistemas operativos

**Diagramas replicados** (mermaid/SVG en el README): definición del SO, capas de componentes,
arquitectura de Von Neumann, Unidad de Control, ruta de datos, ciclo de instrucción,
interrupciones, jerarquía de memoria, estructuras mono/micro/híbrido, cronograma
monoprogramado vs multiprogramado, tabla batch vs time-sharing.

**Pendiente (fotográfico / externo):**

- Fotos históricas: Charles Babbage, ENIAC, Colossus, John von Neumann, UNIVAC I. *(Ilustrativas; el profesor decide si incluirlas.)*
- Cronologías (*timelines*) de Windows, Macintosh, Android e iOS: imágenes externas con
  copyright. En el README se enlazan a Wikipedia / lifewire. No procede regenerarlas.

---

## Tema 2 — Procesos e hilos

**Diagramas replicados** (mermaid/SVG en el README): disposición de memoria monohilo vs
multihilo (SVG), tabla de las 4 combinaciones proceso/hilo, colas de estado, diagrama básico
de estados, diagrama completo de estados de un proceso en UNIX (mermaid), estados en Linux
(2 diagramas).

**Pendiente (fotográfico / externo):** ninguno.

---

## Tema 3 — Planificación de procesos

**Diagramas replicados** (mermaid/SVG/tabla en el README): flujo de planificación, sucesos que
disparan la planificación, niveles de planificador (largo/medio/corto plazo), cambio de
contexto (SVG), tabla de prioridades de Windows 2000, tabla de estados de hilos en Windows.

**Pendiente (fotográfico / externo):**

- Capturas de pantalla del Administrador de tareas y del Monitor de recursos de Windows
  (pp. 265–269). Se omiten; son ilustrativas y dependientes de la versión del SO.

---

## Tema 4 — Concurrencia y sincronización

**Diagramas replicados** (mermaid/SVG en el README): concurrencia vs paralelismo (SVG),
inanición sobre un recurso, mesa de los 5 filósofos (SVG), interbloqueo con semáforos, espera
circular, grafo de asignación de recursos (SVG). Todo el pseudocódigo (semáforos,
productor‑consumidor, filósofos, lectores‑escritores, monitores, rendez‑vous) va como bloques
de texto.

**Pendiente (fotográfico / externo):**

- Lámina de los filósofos monjes (p. 319): ilustración.
- *Clipart* de personas «lector / escritor» (p. 330): decorativo.
- Fotos de atascos de tráfico como metáfora de interbloqueo (p. 349): decorativo.
- Capturas del panel «Control de Aforo» ESPERE/PASE (p. 292): ilustrativas de exclusión mutua.

## Tema 5 — Gestión de memoria (Temas 5 y 6 del PDF)

**Diagramas replicados** (mermaid/SVG/tabla en el README): registro base/límite, tabla de
descripción de particiones, traducción de direcciones en paginación (SVG) y en segmentación
(SVG), vista MMU de la segmentación, segmentación paginada (SVG), esquema de memoria virtual,
correspondencia programa fuente/absoluto/imagen, TLB + tabla de páginas + fallo de página,
memoria de intercambio (desalojo/realojo), tratamiento del fallo de página, anomalía de Belady
(tablas FIFO 3 vs 4 marcos). Ejemplos de código (`malloc`/COW, `ps rssize`) como bloques.

**Pendiente (fotográfico / externo):**

- El «símil del aparcamiento» (pp. 384‑385): collage de fotos de coches aparcados.
- Capturas de las herramientas de gestión de memoria de Windows y Linux (pp. 428‑430).

## Tema 7 del PDF — Dispositivos de E/S (carpeta `11`)

**Diagramas replicados** (mermaid/SVG/tabla en el README): capas de la E/S, E/S con sondeo,
E/S dirigida por interrupciones, doble búfer, geometría del disco (SVG), planificación FCFS y
SSTF (SVG de recorrido de la cabeza), arquitectura de la E/S en UNIX, modelo de tiempos.

**Pendiente (fotográfico / externo):**

- Fotos de discos HDD, SSD y NVMe y de conectores SATA, USB y tarjetas SD (pp. 518‑522).
- Render 3D del disco con pistas y sectores (p. 529): ilustración decorativa.

## Tema 8 del PDF — Sistemas de ficheros (carpeta `12`)

**Diagramas replicados** (mermaid/SVG/tabla en el README): traducción flujo/registro‑bloque,
resolución de la ruta `/usr/ast/mbox` con nodos‑i (tabla), estructura del nodo‑i con
indirecciones (SVG), formato de entradas de directorio UNIX/MS‑DOS, comparativa de sistemas de
ficheros, VFS, RAID 0 / 1 / 5 (SVG), tabla de niveles RAID (0‑6, 0+1, 1+0, 1E, 50, 100).

**Pendiente (fotográfico / externo):**

- Fotos de cabinas y *enclosures* RAID (p. 631): ilustrativas.
- Diagramas de RAID de Wikipedia para los niveles no dibujados (RAID 2, 3, 4, 6, 0+1, 1+0, 1E,
  50, 100): resumidos en la tabla; se pueden añadir sus SVG si se desea (recreables en el
  mismo estilo que RAID 0/1/5).
