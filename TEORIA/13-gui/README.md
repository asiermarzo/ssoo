# GUI

Tema de ampliación.

## Contenidos

- Arquitectura de un entorno gráfico: servidor gráfico, gestor de ventanas, gestor de composición.
- X Window System (X11) frente a Wayland.
- Toolkits y entornos de escritorio.
- Relación con el SO: eventos de entrada, framebuffer, aceleración por GPU.

## Arquitectura de un entorno gráfico (modelo X11)

```mermaid
flowchart LR
    APP1["aplicación cliente 1"] -->|protocolo X| XS["servidor X<br/>(dibuja, reparte eventos)"]
    APP2["aplicación cliente 2"] -->|protocolo X| XS
    WM["gestor de ventanas"] -->|protocolo X| XS
    XS --> KMS["núcleo: framebuffer / DRM-KMS, drivers de entrada"]
    KMS --> HW["pantalla · teclado · ratón · GPU"]
    HW -->|eventos| KMS --> XS -->|eventos| APP1
```

En Wayland el compositor asume el papel del servidor X y del gestor de ventanas, y cada cliente dibuja en su propio búfer.

## Galería visual complementaria

### T13.1 · El escritorio gráfico del Xerox Alto

![Estación de trabajo Xerox Alto con pantalla vertical, teclado, ratón y unidad central](img/xerox-alto.jpg)

*El Xerox Alto, desarrollado en Xerox PARC durante la década de 1970, reunió varias ideas que definirían la interacción gráfica posterior: pantalla de mapa de bits, ventanas, teclado y ratón.*

<sub>Fuente: Maksym Kozlenko, CC BY-SA 4.0, vía Wikimedia Commons. [Ficha y licencia](https://commons.wikimedia.org/wiki/File:Xerox_Alto_computer.jpg).</sub>

### T13.2 · Las ventanas como láminas compuestas

```mermaid
flowchart TB
    A1["Aplicación 1<br/>búfer azul"] --> C[Compositor]
    A2["Aplicación 2<br/>búfer verde"] --> C
    A3["Aplicación 3<br/>búfer gris"] --> C
    Z["posición · orden Z<br/>recorte · transparencia · sombras"] --> C
    C --> F["framebuffer final"] --> P[Pantalla]
```

*Las aplicaciones dibujan en superficies independientes. El compositor decide su posición, orden, transparencia y presentación final.*

### T13.3 · El viaje de un clic

```mermaid
sequenceDiagram
    participant M as Ratón
    participant K as Driver del núcleo
    participant C as Servidor gráfico o compositor
    participant A as Aplicación
    participant P as Pantalla
    M->>K: movimiento y botón
    K->>C: evento de entrada
    C->>A: clic en sus coordenadas
    A->>C: nuevo búfer con la respuesta
    C->>P: compone el siguiente fotograma
```

*Un clic atraviesa varias capas del sistema antes de convertirse en una respuesta visual.*

### T13.4 · X11 frente a Wayland

```mermaid
flowchart TB
    subgraph X["X11 · funciones separadas"]
        XA[Clientes] --> XS[Servidor X]
        XW[Gestor de ventanas] --> XS
        XC[Compositor opcional] --> XS
        XS --> XH[Hardware gráfico y entrada]
    end
    subgraph W["Wayland · compositor unificado"]
        WA[Clientes con sus búferes] --> WC["Compositor Wayland<br/>ventanas + composición + eventos"]
        WC --> WH[Hardware gráfico y entrada]
    end
```

*X11 distribuye el dibujo y los eventos mediante un servidor gráfico. En Wayland, el compositor coordina directamente clientes, entrada y presentación.*

## Material

Las figuras complementarias de este tema están incluidas en la galería anterior y catalogadas en [`TEORIA/IMAGENES.md`](../IMAGENES.md).
