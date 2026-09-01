# Catálogo de imágenes de teoría

Este documento registra la campaña visual complementaria de los 13 temas. Cada figura está
integrada en el `README.md` de su tema con un identificador estable, un texto explicativo y texto
alternativo cuando se trata de un archivo de imagen.

## Resumen

| Tema | Figuras | Técnica |
|---|---:|---|
| 01 · Introducción | T01.1–T01.8 | 2 generadas, 2 de archivo, 4 Mermaid |
| 02 · Usuario y kernel | T02.1–T02.3 | 3 Mermaid |
| 03 · Procesos e hilos | T03.1–T03.5 | 1 generada, 4 Mermaid |
| 04 · Planificación | T04.1–T04.5 | 1 generada, 4 Mermaid |
| 05 · Concurrencia | T05.1–T05.5 | 1 generada, 4 Mermaid |
| 06 · Memoria | T06.1–T06.5 | 5 Mermaid |
| 07 · Pipes y FIFOs | T07.1–T07.3 | 3 Mermaid |
| 08 · Señales | T08.1–T08.4 | 4 Mermaid |
| 09 · Colas de mensajes | T09.1–T09.3 | 3 Mermaid |
| 10 · Memoria compartida | T10.1–T10.3 | 3 Mermaid |
| 11 · Dispositivos de E/S | T11.1–T11.5 | 1 de archivo, 4 Mermaid |
| 12 · Sistemas de ficheros | T12.1–T12.4 | 4 Mermaid |
| 13 · GUI | T13.1–T13.4 | 1 de archivo, 3 Mermaid |
| **Total** | **57** | **5 generadas, 4 de archivo, 48 Mermaid** |

Las ilustraciones generadas no contienen texto ni marcas, de modo que la explicación y la
localización permanecen en Markdown. Los diagramas Mermaid son editables y se renderizan como
gráficos vectoriales en visores compatibles.

## Ilustraciones generadas

Se generaron mediante la herramienta integrada de generación de imágenes de OpenAI. Los PNG
finales están guardados dentro de las carpetas `img/` de los temas que los consumen.

### T01.1 · `01-*/img/dispositivos-con-so.png`

```text
Use case: scientific-educational
Asset type: landscape illustration for Spanish university operating-systems notes
Primary request: create a coherent visual showing the wide variety of physical devices that depend on operating-system software
Scene/backdrop: clean light neutral studio background
Subject: eight distinct devices arranged as one balanced family: smartwatch, smartphone, laptop, rack server, modern car dashboard, industrial robot arm, fighter aircraft in side view, and small communications satellite
Style/medium: polished editorial 3D illustration with realistic materials, technically credible but approachable
Composition/framing: wide 16:9 composition, all eight devices fully visible, generous spacing, clear silhouettes
Lighting/mood: soft studio light, calm educational tone
Color palette: restrained blues, graphite gray, white, with small amber accents
Constraints: no people; no brand marks; no flags; no weapons firing; no text; no labels; no watermark; do not merge devices into hybrids
```

### T01.6 · `01-*/img/sistemas-tiempo-real.png`

```text
Use case: scientific-educational
Asset type: landscape illustration for Spanish university operating-systems notes
Primary request: show three examples where a real-time operating system must react within a deadline
Scene/backdrop: clean triptych on a light neutral background, visually connected by a subtle timing pulse
Subject: an industrial robot arm positioning a component with precision; an automobile wheel with an ABS braking sensor; a fighter aircraft cockpit avionics display in flight, with no visible weapons
Style/medium: polished editorial 3D illustration, realistic engineering materials, technically credible
Composition/framing: wide 16:9, three equal visual zones, strong readable silhouettes, no internal borders
Lighting/mood: precise, controlled, professional
Color palette: restrained blue and graphite with amber deadline accents
Constraints: no people; no brand marks; no flags; no weapons; no explosions; no text; no labels; no watermark
```

### T03.1 · `03-*/img/procesos-vs-hilos-cocinas.png`

```text
Use case: scientific-educational
Asset type: landscape analogy illustration for Spanish university operating-systems notes
Primary request: visually compare processes and threads using professional kitchens
Scene/backdrop: clean split composition on a light neutral editorial background
Subject: first half shows two fully separate restaurant kitchens, each with its own pantry, workbench, tools, recipe and one cook; second half shows one larger kitchen with three cooks working concurrently while sharing the same pantry, workbench, tools and recipe area, but each cook has a small personal prep board
Style/medium: polished editorial isometric illustration, realistic enough to understand, simplified and uncluttered
Composition/framing: wide 16:9 split comparison, strong separation between independent kitchens and shared kitchen
Lighting/mood: bright, calm, instructional
Color palette: restrained blue, sage green, warm wood and graphite
Constraints: diverse adult cooks in practical neutral clothing; no brand marks; no text; no labels; no watermark; do not show unsafe kitchen behavior
```

### T04.1 · `04-*/img/planificador-pista-aeropuerto.png`

```text
Use case: scientific-educational
Asset type: landscape analogy illustration for Spanish university operating-systems notes
Primary request: explain CPU scheduling through an airport with one runway
Scene/backdrop: modern airport viewed from a slightly elevated oblique angle
Subject: one clearly visible active runway representing the CPU; several differently sized civilian aircraft waiting in organized queues at taxiway holding points; an air traffic control tower coordinating which aircraft uses the runway; one aircraft taking off while others wait
Style/medium: polished editorial 3D illustration, technically plausible airport layout, simplified for immediate understanding
Composition/framing: wide 16:9, runway as central focal line, queues and control tower clearly visible
Lighting/mood: bright daylight, orderly, instructional
Color palette: restrained blue, graphite, concrete gray, small amber accents
Constraints: civilian aircraft only; no airline branding; no readable markings; no military elements; no text; no labels; no watermark; exactly one active runway
```

### T05.4 · `05-*/img/interbloqueo-cruce.png`

```text
Use case: scientific-educational
Asset type: landscape analogy illustration for Spanish university operating-systems notes
Primary request: explain operating-system deadlock as four vehicles blocking one another in a compact intersection
Scene/backdrop: top-down view of a narrow four-way urban intersection with one lane entering from each direction
Subject: exactly four different compact vehicles have each entered the intersection and stopped nose-to-side in a closed circular wait; each vehicle occupies the space needed by the next, so none can move forward or reverse
Style/medium: polished editorial 3D illustration with clean geometry and realistic road materials
Composition/framing: near-orthographic top-down square-like intersection within a wide 16:9 canvas, the circular dependency immediately readable
Lighting/mood: neutral daylight, instructional rather than dramatic
Color palette: four distinct muted vehicle colors, asphalt gray, white road markings, small amber accents
Constraints: no collision damage; no people visible; no emergency vehicles; no brand marks; no road-sign text; no labels; no watermark; exactly four vehicles
```

## Fotografías externas y licencias

| Figura | Archivo local | Autor/fuente | Licencia | Página de origen |
|---|---|---|---|---|
| T01.2 | `01-*/img/operadora-tarjetas-perforadas.jpg` | U.S. Census Bureau | Dominio público, obra del Gobierno federal de EE. UU. | [Wikimedia Commons](https://commons.wikimedia.org/wiki/File:Keypunch_operator_1950_census_IBM_016.jpg) |
| T01.3 | `01-*/img/programadoras-eniac.jpg` | Fotógrafo no identificado del U.S. Army | Dominio público, obra del Gobierno federal de EE. UU. | [Wikimedia Commons](https://commons.wikimedia.org/wiki/File:Two_women_operating_ENIAC_(full_resolution).jpg) |
| T11.2 | `11-*/img/disco-duro-abierto.jpg` | © Raimond Spekking | [CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/) | [Wikimedia Commons](https://commons.wikimedia.org/wiki/File:Seagate_ST9300AG_-_opened._Platter_and_head_mechanics-9324.jpg) |
| T13.1 | `13-*/img/xerox-alto.jpg` | Maksym Kozlenko | [CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/) | [Wikimedia Commons](https://commons.wikimedia.org/wiki/File:Xerox_Alto_computer.jpg) |

Los archivos externos se descargaron a un tamaño máximo aproximado de 1600 píxeles para evitar
incorporar originales innecesariamente grandes. No se modificó su contenido; solo se emplea una
miniatura generada por Wikimedia cuando corresponde.
