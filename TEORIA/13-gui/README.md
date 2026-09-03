# GUI

Tema de ampliación.

## Contenidos

- Arquitectura de un entorno gráfico: servidor gráfico, gestor de ventanas, gestor de composición.
- X Window System (X11) frente a Wayland.
- Toolkits y entornos de escritorio.
- Relación con el SO: eventos de entrada, framebuffer, aceleración por GPU.

## Antecedentes: el Xerox Alto

El Xerox Alto, desarrollado en Xerox PARC durante la década de 1970, reunió varias ideas que definirían la interacción gráfica posterior: pantalla de mapa de bits, ventanas, teclado y ratón.

<img src="img/xerox-alto.jpg" width="440" alt="Estación de trabajo Xerox Alto con pantalla vertical, teclado, ratón y unidad central">

<sub>Fuente: Maksym Kozlenko, CC BY-SA 4.0, vía Wikimedia Commons. [Ficha y licencia](https://commons.wikimedia.org/wiki/File:Xerox_Alto_computer.jpg).</sub>

## Arquitectura de un entorno gráfico (modelo X11)

<img src="img/arquitectura-x11.svg" width="560" alt="Las aplicaciones y el gestor de ventanas hablan con el servidor X mediante el protocolo X; el servidor X se apoya en el núcleo para acceder al hardware gráfico y de entrada">

En Wayland el compositor asume el papel del servidor X y del gestor de ventanas, y cada cliente dibuja en su propio búfer.

## Composición de ventanas

Las aplicaciones dibujan en superficies independientes. El compositor decide su posición, orden Z, recorte, transparencia y sombras, y produce el framebuffer final que se envía a la pantalla.

<img src="img/composicion-ventanas.svg" width="560" alt="Tres aplicaciones dibujan en búferes independientes representados como láminas superpuestas; el compositor las combina en el framebuffer final que se muestra en pantalla">

*Las aplicaciones dibujan en superficies independientes. El compositor decide su posición, orden, transparencia y presentación final.*

## El viaje de un clic

Un clic atraviesa varias capas del sistema antes de convertirse en una respuesta visual.

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

## X11 frente a Wayland

X11 distribuye el dibujo y los eventos mediante un servidor gráfico con funciones separadas. En Wayland, un compositor unificado coordina directamente clientes, entrada y presentación.

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

    classDef nucleo fill:#1f3f66,stroke:#132840,color:#ffffff;
    classDef cliente fill:#cfe2f3,stroke:#2b6f99,color:#222;
    classDef hardware fill:#d9d9d9,stroke:#555,color:#222;

    class XS,WC nucleo;
    class XA,XW,XC,WA cliente;
    class XH,WH hardware;
```

*X11 distribuye el dibujo y los eventos mediante un servidor gráfico. En Wayland, el compositor coordina directamente clientes, entrada y presentación.*

Figuras catalogadas en [`TEORIA/IMAGENES.md`](../IMAGENES.md).
