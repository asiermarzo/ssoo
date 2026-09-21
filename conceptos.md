# ¿Cómo trabajar esta asignatura? - Guía del profesor

## Punto de partida del alumnado

Se da en segundo curso, primer semestre. Los estudiantes no han tenido ninguna asignatura de arquitectura de computadores, no han dado nada de ensamblador. En Programación y Estructura de Datos (1º) se programa sobre una máquina abstracta, sin aproximarse al hardware, C se ve como una traducción de pseudocódigo. Deberían tener el concepto de memoria, las variables en memoria y punteros. Sí que tienen el concepto flujo de ejecución de las instrucciones, saltos a funciones; y algoritmos básicos (p.ej., búsqueda, ordenación).

Arquitectura de computadores se da en paralelo a SSOO, pero tardan uno o dos meses en llegar a interrupciones o saltos a subrutina. Redes (sockets & select()) se dan en el siguiente semestre, por lo que no conocen nada de cliente/servidor o conexiones. Carecen de conceptos como caché o multiproceso; no han dado nada de concurrencia.

Seguir Tanenbaum o Silberschatz parece imposible ya que requieren un buen conocimiento de C y de la arquitectura de un computador. OSTEP podría ser más adecuado ya que no asume tanto conocimiento previo.

## Posibles enfoques de la asignatura

Sistemas operativos podría impartirse desde distintos puntos de vista:
1. De usuario: instalar Linux, Windows, y utilizar herramientas típicas. Línea de comandos y bash.
2. Estudiar y modificar un SSOO de juguete (p.ej Minix, Pintos...).
3. Programar y depurar con las utilidades que da Linux: procesos, hilos, IPCs...; gdb, strace.
4. Estudiar los conceptos a nivel básico.

## Niveles de explicación

Los conceptos se pueden explicar por niveles, con estas estructuras:
- A dos niveles: básico y práctico. Básico, se puede explicar sin conocimientos previos, normalmente analogías y simplificaciones. Práctico es el necesario para poder realizar los ejercicios de prácticas.
- A tres niveles: básico, a nivel de máquina, funcionamiento real (p.ej en Linux).
- Primero a nivel básico + práctico. Después del parcial, revisar los conceptos pero a nivel máquina.

# Conceptos fundamentales en 3 niveles

1. **Nivel 1 — Genérico y conceptual**, para un estudiante que viene de bachiller.
2. **Nivel 2 — A nivel de máquina**, para un estudiante que sabe ensamblador, registros, subrutinas e interrupciones.
3. **Nivel 3 — Funcionamiento real**: código real, depuración de registros, trazas del sistema.

--- 

## Procesos e hilos

| Nivel | Resumen | Qué se ve |
|---|---|---|
| 1 | Un proceso es un programa en ejecución. | Programa vs proceso, multiprogramación, ciclo de vida básico. |
| 2 |  PCB que guarda su estado y registros. | PCB, tabla de procesos, relación con pila/subrutinas. |
| 3 | `task_struct`, creado con `fork`/`exec`. | `fork()`, `execve()`, `/proc/<pid>/status`, `ps`, adjuntar con `gdb`. |

## Planificación

| Nivel | Resumen | Qué se ve |
|---|---|---|
| 1 | El SO decide qué proceso usa la CPU y cuánto tiempo. | Analogía de turnos/colas, multitarea aparente en un solo núcleo. |
| 2 | Un temporizador interrumpe y fuerza el cambio de proceso en ejecución. | Interrupción de reloj, *quantum*, algoritmos simples (RR, FCFS). |
| 3 | Linux usa CFS con prioridades configurables. | `nice`/`chrt`, `sched_setscheduler()`, `/proc/sys/kernel/sched_*`, medir con `perf`. |

## Cambios de contexto

| Nivel | Resumen | Qué se ve |
|---|---|---|
| 1 | Al pausar un proceso, el SO guarda "por dónde iba" para retomarlo después. | Analogía de pausar/guardar una partida, sin detalle de qué se guarda. |
| 2 | Cambiar de proceso implica guardar/restaurar contexto: PC, SP y registros. | Relación con interrupciones y subrutinas, coste del cambio. |
| 3 | `switch_to()`/`context_switch()` y tablas de páginas. | Registros vía `gdb`/`ptrace`, `/proc/<pid>/stat` (voluntary/involuntary ctxt switches). |

## Llamadas al sistema

| Nivel | Resumen | Qué se ve |
|---|---|---|
| 1 | Un proceso pide al SO realizar acciones. | Analogía de pedir permiso, ejemplos cotidianos (abrir fichero, pedir memoria). |
| 2 | Interrupción software que cambia a modo kernel. | Instrucción `int`/`syscall`, vector de interrupción ya visto en arquitectura, paso de parámetros en registros. |
| 3 | Cada syscall tiene implementación en el kernel. | `strace`, `syscall(2)`, wrapper de glibc, `write()`/`read()` directo vs `printf()`. |


## Comunicación entre procesos (IPC)

| Nivel | Resumen | Qué se ve |
|---|---|---|
| 1 | Los procesos están aislados; para comunicarse necesitan un mecanismo del SO. | Analogía de habitaciones incomunicadas: un tubo para pasar letras (tubería), un buzón con mensajes (cola de mensajes) o una alarma sonora que interrumpe (señal). |
| 2 | Hay canales para flujo de datos y canales solo para notificar. | Tubería (flujo de bytes, sin estructura, FIFO) vs cola de mensajes (mensajes con tipo/tamaño y prioridad) vs señal (aviso asíncrono sin datos, interrumpe al proceso receptor). |
| 3 | Linux implementa cada mecanismo con syscalls | tuberías `pipe()`, `mkfifo()`, colas de mensajes (`msgget`/`msgsnd`/`msgrcv`), señales (`kill`, `signal`/`sigaction`). |

## Gestión de memoria

| Nivel | Resumen | Qué se ve |
|---|---|---|
| 1 | El SO reparte la memoria para los procesos. | Memoria como recurso limitado y compartido. |
| 2 | Cada proceso ve un espacio de direcciones propio, traducido por hardware. | Direcciones lógicas vs físicas, MMU, base/límite o paginación. |
| 3 | Linux gestiona memoria virtual por páginas, con swap. | `/proc/<pid>/maps`, `mmap()`, `malloc` vs `sbrk`, `-fsanitize=address` (ya usado en P0), segfault real. |


---

## Concurrencia y sincronización

| Nivel | Resumen | Qué se ve |
|---|---|---|
| 1 | Cuando varias tareas comparten memoria (o cualquier recurso), el orden en que actúan puede dar resultados erróneos, dejarlas bloqueadas esperándose entre sí para siempre, o dejar a alguna sin turno indefinidamente. | Analogía de dos personas escribiendo a la vez en la misma pizarra (condición de carrera); varios comensales en una mesa con un solo cubierto entre cada dos, cada uno coge el de su izquierda y espera a que quede libre el de su derecha, que su vecino también tiene cogido (interbloqueo); alguien que nunca consigue hablar porque siempre hay otro con más prisa (inanición). |
| 2 | Dos hilos que leen y escriben la misma memoria compartida sin control generan una condición de carrera; la exclusión mutua lo evita, pero mal usada provoca interbloqueo o inanición. | Instrucciones no atómicas (leer-modificar-escribir en varios pasos), sección crítica; interbloqueo por espera circular al pedir dos candados en orden distinto; inanición si la política de acceso siempre prioriza a otros. |
| 3 | En Linux la exclusión mutua sobre memoria compartida se implementa con primitivas del sistema; usarlas mal reproduce interbloqueo e inanición reales. | Semáforos (`sem_t`, `sem_open`/`sem_wait`/`sem_post`), memoria compartida (`shm_open`/`mmap`) entre procesos; interbloqueo con dos `pthread_mutex_lock` en orden cruzado, procesos en estado `D`/bloqueados vistos con `ps`/`top` o hilos colgados con `gdb`. |

## Sistemas de ficheros y RAID

| Nivel | Resumen | Qué se ve |
|---|---|---|
| 1 | El SO organiza los datos en ficheros y carpetas para que persistan y se puedan encontrar; a veces se guardan copias en varios discos por seguridad. | Analogía de archivador con carpetas y documentos, y de tener una copia de seguridad en otro cajón por si el primero se estropea. |
| 2 | Un fichero es una secuencia de bloques indexada por una estructura que guarda sus metadatos; un directorio es una tabla que traduce nombres a esa estructura. | Fichero como bloques enlazados/indexados, directorio como tabla nombre→nodo, permisos como control de acceso; combinar varios discos puede repartir datos (rendimiento) o duplicarlos (tolerancia a fallos). |
| 3 | Linux usa nodos-i y sistemas de ficheros concretos (ext4, etc.); varios discos se combinan en RAID con niveles estandarizados. | Nodo-i, superbloque, `stat`/`ls -i`, permisos en octal (`chmod`), `df`/`mount`; RAID 0 (*striping*), RAID 1 (*mirroring*), RAID 5 (paridad distribuida), `mdadm`, `/proc/mdstat`. |

## Jerarquía de memoria

| Nivel | Resumen | Qué se ve |
|---|---|---|
| 1 | No toda la memoria es igual, hay niveles. | Analogía mesa-cajón-armario-almacén, velocidad vs capacidad y coste. |
| 2 | Registros, caché, RAM y disco. | Localidad temporal/espacial, ejemplo matriz por filas vs columnas. |
| 3 | Aciertos/fallos de caché, TLB, *page cache*. | `perf stat` (cache-misses, page-faults), impacto medible en tiempo de ejecución, *page cache* del sistema de ficheros. |

## Dispositivos de E/S y DMA

| Nivel | Resumen | Qué se ve |
|---|---|---|
| 1 | La CPU tiene que intercambiar datos con dispositivos externos (disco, red, teclado); hacerlo ella misma dato a dato le quita tiempo de cómputo. | Analogía del jefe de cocina que va él mismo al almacén a por cada ingrediente, en vez de mandar a un ayudante que se los traiga todos y le avise al terminar. |
| 2 | Los dispositivos pueden escribir en RAM sin pasar por la CPU (DMA), o la CPU puede leer/escribir dato a dato (*polling*); una interrupción avisa a la CPU cuando el dispositivo termina. | DMA vs *polling*, controlador de dispositivo, interrupción (IRQ) de fin de transferencia. |
| 3 | En Linux el driver del dispositivo programa la transferencia DMA y atiende la IRQ de fin de transferencia. | `dmesg` programa el DMA y atiende la IRQ de fin de transferencia; ejemplos en discos y red. |


# Problemas y soluciones


## Desconexión entre teoría y prácticas

- Hay conceptos de teoría que no se trabajan en prácticas: gestión de memoria (modo protegido, paginación, cachés).
- Algunos ejercicios de práctica resultan artificiales. ¿Qué programa real necesita crear un árbol de procesos? ¿O simular un sistema concurrente mezclando pipes y colas de mensaje?

