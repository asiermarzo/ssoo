# Sistemas Operativos — material de teoría y prácticas

Material para la asignatura **Sistemas Operativos** (240304) del Grado en Ingeniería Informática de la Universidad Pública de Navarra. 2º curso, semestre de otoño. 6 ECTS: 3 de teoría y 3 de prácticas.

## Cómo obtener el material

```bash
git clone https://github.com/asiermarzo/ssoo.git
```

O descargar como ZIP desde la página del repositorio en GitHub (botón *Code → Download ZIP*) y descomprimirlo con el gestor de archivos o con el comando `unzip ssoo-main.zip`.

## Entornos de ejecución y compilación

Las prácticas se realizan sobre Linux. Los distintos entornos disponibles (ordenadores de la UPNA, VDI, máquinas virtuales, GitHub Codespaces, o el ordenador propio con Linux, WSL2 o Docker) y cómo compilar el material están en [`entornos_ejecucion.md`](entornos_ejecucion.md).

## Teoría

| Nº | Tema | Carpeta |
|----|------|---------|
| 1 | Introducción a los sistemas operativos | [`introduccion-a-los-sistemas-operativos`](TEORIA/introduccion-a-los-sistemas-operativos/) |
| 2 | Espacio usuario y espacio kernel. Llamadas al sistema | [`espacio-usuario-espacio-kernel`](TEORIA/espacio-usuario-espacio-kernel/) |
| 3 | Procesos e hilos | [`procesos-e-hilos`](TEORIA/procesos-e-hilos/) |
| 4 | Planificación de procesos | [`planificacion-de-procesos`](TEORIA/planificacion-de-procesos/) |
| 5 | IPC: pipes y fifos | [`ipc-pipes-y-fifos`](TEORIA/ipc-pipes-y-fifos/) |
| 6 | IPC: señales | [`ipc-senales`](TEORIA/ipc-senales/) |
| 7 | IPC: colas de mensajes | [`ipc-colas-de-mensajes`](TEORIA/ipc-colas-de-mensajes/) |
| 8 | Gestión de memoria | [`gestion-de-memoria`](TEORIA/gestion-de-memoria/) |
| 9 | Concurrencia y sincronización | [`concurrencia-y-sincronizacion`](TEORIA/concurrencia-y-sincronizacion/) |
<!-- 
| 10 | Memoria compartida y mutex | [`memoria-compartida-y-mutex`](TEORIA/memoria-compartida-y-mutex/) |
| 11 | Dispositivos de E/S | [`dispositivos-de-es`](TEORIA/dispositivos-de-es/) |
| 12 | Sistemas de ficheros | [`sistemas-de-ficheros`](TEORIA/sistemas-de-ficheros/) |
| 13 | GUI | [`gui`](TEORIA/gui/) |
-->

## Prácticas

| Nº | Práctica | Carpeta |
|----|----------|---------|
| 0 | Shell y herramientas | [`shell-y-herramientas`](PRACTICA/shell-y-herramientas/) |
| 1 | Entrada y salida: consola, ficheros, file descriptors | [`entrada-salida-y-ficheros`](PRACTICA/entrada-salida-y-ficheros/) |
| 2 | Procesos: `fork`, `execvp`, `wait`, `pstree` | [`procesos`](PRACTICA/procesos/) |
| 3 | Pipes y fifos: `pipe`, `mkfifo`, `dup2` | [`pipes-y-fifos`](PRACTICA/pipes-y-fifos/) |
| 4 | Acueducto: ejercicio de procesos y pipes | [`ejercicio-procesos-y-pipes`](PRACTICA/ejercicio-procesos-y-pipes/) |
| 5 | Señales: `kill`, `signal`, `sigaction`, `sleep`, `pause` | [`senales`](PRACTICA/senales/) |
| 6 | Colas de mensajes: `msgget`, `msgsnd`, `msgrcv`, `ipcs` | [`colas-de-mensajes`](PRACTICA/colas-de-mensajes/) |
| 7 | Memoria compartida y semáforos | [`memoria-compartida-y-semaforos`](PRACTICA/memoria-compartida-y-semaforos/) |
| 8 | Planificador de procesos (colas multinivel) | [`planificador-de-procesos`](PRACTICA/planificador-de-procesos/) |
| 9 | Sistema concurrente | [`sistema-concurrente`](PRACTICA/sistema-concurrente/) |

El `README.md` de cada práctica incluye la descripción, los comandos, llamadas al sistema y ejercicios propuestos.

## Guía de estilo

### Presentaciones
Para teoría, se utilizan archivos Markdown con diagramas mermaid, SVG y JPG. También hay simulaciones interactivas embebidas, normalmente en HTML + JS. Se utilizará un markdown mínimo y el texto estará siempre en una línea.

Se prefieren explicaciones concisas y resumidas.

No se numerarán los contenidos de los temas, la única numeración será la de la tabla raíz para poder reordenarse según disponibilidad de sesiones.

Para prácticas, se utilizan archivos Markdown, `.c`, Makefiles, y scripts bash.

<details> <summary> <- clickar en la flecha. Habrá texto expandible  </summary>
Que contienen información útil, pero que no es obligatorio su conocimiento.
</details>

### Código

En código utilizará alojamiento estático (en la pila - stack) para las variables y se evitará el uso de `malloc`. 

Se evitarán programas con varios archivos C: cada proceso o grupo de procesos será un `.c` que acabará compilado en un binario. Esto hace el uso de Makefile opcional. Se pueden utilizar `.h` para compartir constantes y estructuras comunes.

Se pueden dejar `#include` que no sean necesarios si esto evita estar buscando qué includes necesita cada función.

Se puede omitir el tratamiento de errores para que el código quede más claro. La captura de errores se hace con strace o gdb.

Estílos de código, aunque cada estudiante puede seguir el suyo:

- La llave de apertura `{` se coloca en la misma línea.
- Se usa `snake_case` en lugar de `CamelCase`.
- Se intenta usar `for` siempre que sea posible (frente a `while` o `do`/`while`).
- Las variables se declaran e inicializan en la misma línea, en el *scope* más interno posible.
- Los `struct` se definen con `typedef struct` sin darle nombre al `struct`.
- Los permisos se indican en octal (`0640`) y nos con los flags (`S_IRUSR`, `SHM_R`…).

## Material adicional

Vídeos, Películas y Series [`material_adicional.md`](material_adicional.md).