# Imágenes pendientes de generar

Catálogo de figuras del PDF *Sistemas Operativos — 240304-25-26* (J. J. Astrain) que **no se
han podido replicar con mermaid** y conviene regenerar como imagen. Los diagramas de flujo,
capas, pirámides y bloques ya están hechos en mermaid dentro de cada `README.md` y no aparecen
aquí.

**Flujo de trabajo:**

1. El usuario genera cada imagen con ChatGPT usando el *prompt* indicado.
2. Pega el archivo resultante en la conversación.
3. Claude lo guarda en `TEORIA/NN-*/img/` con el nombre sugerido y lo enlaza en el `README.md`.

**Convenciones para los prompts:** estilo diagrama vectorial limpio, fondo blanco o
transparente, texto **en español**, sin marcas de agua, relación de aspecto apaisada (4:3 o
16:9), paleta sobria (azules/grises) coherente con el resto del material.

---

## Tema 1 — Introducción a los sistemas operativos

### 1.1 · `img/cu-ruta-datos.png` — Ruta de datos de la Unidad de Control (p. 59)

Diagrama esquemático del camino de datos de una CPU sencilla.

> Prompt para ChatGPT:
> "Diagrama esquemático limpio, estilo libro de arquitectura de computadores, fondo blanco,
> etiquetas en español. A la izquierda, una pila vertical de registros etiquetados «R1», «R2»,
> «…», «Rn». Desde arriba bajan dos flechas anchas etiquetadas «Operando izquierdo» y
> «Operando derecho» que entran en un bloque central etiquetado «Unidad funcional». Del bloque
> sale hacia la derecha una flecha etiquetada «Resultado» que llega a otra pila vertical de
> celdas etiquetada «Registros de estado». Una flecha bidireccional en la parte inferior
> izquierda conecta los registros con una etiqueta «A/desde la memoria primaria». Colores
> azules suaves, líneas finas, sin sombras ni 3D. Relación de aspecto 4:3."

### 1.6 · `img/monoprog-vs-multiprog.png` — Monoprogramado vs multiprogramado (p. 97) *(opcional)*

Cronograma comparando el uso de CPU y de E/S. En el `README.md` hay una aproximación en
mermaid; esta imagen sería una versión más pulida.

> Prompt para ChatGPT:
> "Diagrama de líneas de tiempo (diagrama de Gantt sencillo) con fondo blanco y texto en
> español, para comparar un sistema monoprogramado y uno multiprogramado. Dos bloques
> apilados. Bloque superior titulado «Monoprogramado», con dos filas etiquetadas «CPU» y
> «E/S»: la CPU ejecuta «Tarea 1», luego queda un hueco vacío marcado «(ociosa)» mientras la
> fila E/S muestra «Tarea 1», después la CPU retoma «Tarea 1» y a continuación «Tarea 2», etc.
> Bloque inferior titulado «Multiprogramado», mismas filas «CPU» y «E/S», pero sin huecos: la
> CPU alterna «Tarea 1» y «Tarea 2» de forma que siempre hay trabajo, y la fila E/S se solapa.
> Eje horizontal con una flecha etiquetada «tiempo». Rectángulos en dos tonos (azul claro para
> Tarea 1, gris para Tarea 2). Estilo plano, sin 3D. Relación de aspecto 16:9."
