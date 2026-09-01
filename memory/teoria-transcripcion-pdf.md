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
- Diagramas: replicar con **mermaid**.
- Imágenes: catalogar **solo diagramas/figuras relevantes** (no iconos, fotos de hardware ni
  capturas). Prompt detallado de ChatGPT para cada una. Catálogo maestro en
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
- [x] 01 Introducción (Tema 1) — README reescrito + catálogo img (cu-ruta-datos, monoprog-vs-multiprog)
- [ ] 03 Procesos e hilos (Tema 2, pp.140–204)
- [ ] 04 Planificación (Tema 3, pp.205–272)
- [ ] 05 Concurrencia y sincronización (Tema 4, pp.273–364)
- [ ] 06 Gestión de memoria (Tema 5+6, pp.366–489)
- [ ] 11 Dispositivos de E/S (Tema 7, pp.491–556)
- [ ] 12 Sistemas de ficheros (Tema 8, pp.558–654)
- [ ] 02/07/08/09/10/13 — revisar PDFs de prácticas

Catálogo maestro creado: `TEORIA/IMAGENES-PENDIENTES.md`.
