---
name: teoria-transcripcion-pdf
description: Tarea en curso: volcar el PDF "Sistemas Operativos" a los README de TEORIA
metadata:
  type: project
---

Tarea (iniciada 2026-09-01): transcribir el contenido de `Sistemas Operativos - 240304-25-26.pdf`
(732 pp., J.J. Astrain, 9 "Temas") a los `TEORIA/NN-*/README.md`.

**Criterios acordados con el usuario:**
- Fidelidad: **prosa fiel** — todo el texto, cifras, listas y tablas del PDF, redactado en
  prosa/listas legibles en Markdown, estilo conciso del repo. No verbatim bullet-a-bullet.
- Diagramas: Claude replica **todos** con **mermaid** o **SVG embebido inline en el .md**
  (confirmado por el usuario; NO renderizar a PNG). Solo queda pendiente el material
  **fotográfico/externo** (fotos históricas, hardware, capturas, timelines con copyright).
- Prompt ChatGPT para ese material pendiente. Registro de estado por tema en
  `TEORIA/IMAGENES-PENDIENTES.md`. El usuario генera las imágenes y luego Claude las
  distribuye en cada carpeta y las linka en el `.md`.
- Tema 9 del PDF (Sistemas distribuidos, pp.656–732): **OMITIR**, no hay carpeta.
- Temas del repo sin cobertura en este PDF (02 parcial, 07 pipes, 08 señales, 09 colas,
  10 parcial mutex, 13 GUI): revisar `Practicas_240304-25-26.pdf` y el anexo.

**Mapa PDF → carpetas repo:**
| Carpeta | PDF | Páginas |
|---|---|---|
| 01 Introducción | Tema 1 | 51–139 |
| 03 Procesos e hilos | Tema 2 | 140–204 |
| 04 Planificación | Tema 3 | 205–272 |
| 05 Concurrencia y sincronización | Tema 4 | 273–364 |
| 06 Gestión de memoria (+ virtual) | Tema 5 + Tema 6 | 366–489 |
| 11 Dispositivos de E/S | Tema 7 | 491–556 |
| 12 Sistemas de ficheros | Tema 8 | 558–654 |

Texto del PDF extraído en scratchpad: `pdf_text.txt`, `slide_titles.txt`.

**Progreso:**
- [x] 01 Introducción (Tema 1) — README + todos los diagramas mermaid/SVG inline. Pendiente: fotos históricas + timelines (externos)
- [x] 03 Procesos e hilos (Tema 2) — README + todos los diagramas mermaid/SVG inline. Nada pendiente
- [x] 04 Planificación (Tema 3) — README + todos los diagramas mermaid/SVG/tabla inline. Pendiente: capturas Task Manager (se omiten)
- [x] 05 Concurrencia y sincronización (Tema 4) — README + diagramas mermaid/SVG inline + pseudocódigo. Pendiente: ilustraciones/clipart/fotos
- [x] 06 Gestión de memoria (Temas 5+6) — README + diagramas mermaid/SVG/tabla inline. Pendiente: aparcamiento, capturas Win/Linux
- [x] 11 Dispositivos de E/S (Tema 7) — README + diagramas mermaid/SVG inline. Pendiente: fotos discos/conectores
- [x] 12 Sistemas de ficheros (Tema 8) — README + diagramas mermaid/SVG/tabla inline (i-nodo, RAID 0/1/5, VFS…). Pendiente: fotos cabinas RAID
- [ ] 02/07/08/09/10/13 — revisar PDFs de prácticas (pendiente)

**TODOS los temas del PDF principal (1–8) COMPLETADOS.** Falta: Tema 9 distribuidos (omitido
por acuerdo), y los temas repo 02/07/08/09/10/13 (revisar PDFs de prácticas).
El usuario está creando en paralelo `TEORIA/IMAGENES.md` (galería complementaria de 57 imágenes).

Catálogo maestro creado: `TEORIA/IMAGENES-PENDIENTES.md`.
