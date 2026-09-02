# Sistemas Operativos — material de teoría y prácticas

Material para la asignatura **Sistemas Operativos** (240304) del Grado en Ingeniería Informática de la Universidad Pública de Navarra. 2º curso, semestre de otoño. 6 ECTS: 3 de teoría y 3 de prácticas.

## Cómo obtener el material

```bash
git clone https://github.com/asiermarzo/ssoo.git
```

O descargar como ZIP desde la página del repositorio en GitHub (botón *Code → Download ZIP*) y descomprimirlo con el gestor de archivos o con el comando `unzip ssoo-main.zip`.

Las prácticas se realizan sobre un PC virtualizado con Linux, disponible 24×7 a través de la infraestructura VDI del Aulario o del [Laboratorio Virtual](https://eim-laboratoriovirtual.unavarra.es/laboratorio/).

## Cómo compilar las prácticas

Cada práctica se compila con `gcc` y trae su propio script de compilación en bash. El uso de `make` o `cmake` es opcional (cada proceso o grupo de procesos es un único `.c` que compila a un binario).

```bash
gcc -Wall -Wextra -o programa programa.c
```

## Teoría

| Tema | Carpeta |
|------|---------|
| Introducción a los sistemas operativos | [`TEORIA/01`](TEORIA/01-introduccion-a-los-sistemas-operativos/) |
| Espacio usuario y espacio kernel. Llamadas al sistema | [`TEORIA/02`](TEORIA/02-espacio-usuario-espacio-kernel/) |
| Procesos e hilos | [`TEORIA/03`](TEORIA/03-procesos-e-hilos/) |
| Planificación de procesos | [`TEORIA/04`](TEORIA/04-planificacion-de-procesos/) |
| Concurrencia y sincronización | [`TEORIA/05`](TEORIA/05-concurrencia-y-sincronizacion/) |
| Gestión de memoria | [`TEORIA/06`](TEORIA/06-gestion-de-memoria/) |
| IPC: pipes y fifos | [`TEORIA/07`](TEORIA/07-ipc-pipes-y-fifos/) |
| IPC: señales | [`TEORIA/08`](TEORIA/08-ipc-senales/) |
| IPC: colas de mensajes | [`TEORIA/09`](TEORIA/09-ipc-colas-de-mensajes/) |
| Memoria compartida y mutex | [`TEORIA/10`](TEORIA/10-memoria-compartida-y-mutex/) |
| Dispositivos de E/S | [`TEORIA/11`](TEORIA/11-dispositivos-de-es/) |
| Sistemas de ficheros | [`TEORIA/12`](TEORIA/12-sistemas-de-ficheros/) |
| GUI (ampliación) | [`TEORIA/13`](TEORIA/13-gui/) |

## Prácticas

| Práctica | Carpeta |
|----------|---------|
| P0 — Shell y herramientas (`.c` → `.o` → binario, `tmux`, `gdb`, `strace`) | [`PRACTICA/00`](PRACTICA/00-shell-y-herramientas/) |
| P1 — Entrada y salida: consola, ficheros, file descriptors | [`PRACTICA/01`](PRACTICA/01-entrada-salida-y-ficheros/) |
| P2 — Procesos e hilos: `fork`, `execvp`, `wait`, `pstree` | [`PRACTICA/02`](PRACTICA/02-procesos-e-hilos/) |
| P3 — Pipes y fifos: `pipe`, `mkfifo`, `dup2` | [`PRACTICA/03`](PRACTICA/03-pipes-y-fifos/) |
| P4 — Señales: `kill`, `signal`, `sigaction`, `sleep`, `pause` | [`PRACTICA/04`](PRACTICA/04-senales/) |
| P5 — Minishell | [`PRACTICA/05`](PRACTICA/05-minishell/) |
| P6 — Memoria compartida y semáforos | [`PRACTICA/06`](PRACTICA/06-memoria-compartida-y-semaforos/) |
| P7 — Colas de mensajes: `msgget`, `msgsnd`, `msgrcv`, `ipcs` | [`PRACTICA/07`](PRACTICA/07-colas-de-mensajes/) |
| P8 — Planificador de procesos (colas multinivel) | [`PRACTICA/08`](PRACTICA/08-planificador-de-procesos/) |
| P9 — Sistema concurrente | [`PRACTICA/09`](PRACTICA/09-sistema-concurrente/) |

El `README.md` de cada práctica es autocontenido: incluye la descripción del tema, los comandos y llamadas al sistema utilizadas (prototipo, parámetros, valor devuelto, errores, `#include` y ejemplos) y los ejercicios propuestos.

## Guía de estilo

Para teoría, se utilizan archivos Markdown con diagramas mermaid, SVG y JPG. También se pueden dar simulaciones interactivas embebidas, normalmente en HTML + JS. Se utilizará un markdown mínimo y el texto estará siempre en una línea.

Para prácticas, se utilizan archivos Markdown, archivos `.c`, Makefiles, scripts bash y configuraciones tmux de terminales.

Se prefieren explicaciones concisas frente a detalladas y verbosas.

En código utilizará alojamiento estático (en la pila - stack) para las variables siempre que sea posible y se evitará el uso de `malloc`. Quizás sea necesario `malloc` para mostrar diferencias entre pila (*stack*) y montículo (*heap*), y para el tema de gestión de memoria.

Se evitarán programas con varios archivos C: cada proceso o grupo de procesos será un `.c` que acabará compilado en un binario. Esto hace el uso de make opcional. Se pueden utilizar `.h` para compartir constantes y estructuras comunes.

Se pueden dejar `#include` que no sean necesarios si esto evita estar buscando qué includes necesita cada función. Es más como incluir un bloque común de `#include <>` y quitar los que den *warning* por no usarse.

Sobre convenciones de código: aunque cada estudiante puede seguir su propia preferencia, el código de este material sigue estas normas:

- La llave de apertura `{` se coloca en la misma línea.
- Se usa `snake_case` en lugar de `CamelCase`.
- Se intenta usar `for` siempre que sea posible (frente a `while` o `do`/`while`).
- Las variables se declaran e inicializan en la misma línea, en el *scope* más interno posible.
- Se evita el uso de variables globales.
- Los `struct` se definen con `typedef struct` sin darle nombre al `struct`.

## Enlaces a material

Vídeos complementarios por tema (canal CoreDumpped) en [`material_adicional.md`](material_adicional.md).