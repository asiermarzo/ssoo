# Conceptos fundamentales en 3 niveles

1. **Nivel 1 — Genérico y conceptual**, para un estudiante que viene de bachiller.
2. **Nivel 2 — A nivel de máquina**, para un estudiante que sabe ensamblador, registros, subrutinas e interrupciones.
3. **Nivel 3 — Funcionamiento real**: código real, depuración de registros, trazas del sistema.

## Procesos e hilos

| Nivel | Resumen | Qué se ve |
|---|---|---|
| 1 | Un proceso es un programa en ejecución con su propio espacio. | Programa vs proceso, multiprogramación, ciclo de vida básico, ejemplo con el gestor de tareas. |
| 2 |  PCB que guarda su estado y registros. | PCB, tabla de procesos, relación con pila/subrutinas, `fork()` a nivel conceptual. |
| 3 | `task_struct`, creado con `fork`/`exec`. | `fork()`, `execve()`, `/proc/<pid>/status`, `ps`, adjuntar con `gdb`. |

## Planificación

| Nivel | Resumen | Qué se ve |
|---|---|---|
| 1 | El SO decide qué proceso usa la CPU y cuándo. | Analogía de turnos/colas, multitarea aparente en un solo núcleo. |
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
| 1 | Un programa pide al SO. | Analogía de pedir permiso, ejemplos cotidianos (abrir fichero, pedir memoria). |
| 2 | Se implementa con una interrupción software que cambia a modo kernel. | Instrucción `int`/`syscall`, vector de interrupción ya visto en arquitectura, paso de parámetros en registros. |
| 3 | Cada syscall tiene implementación en el kernel. | `strace`, `syscall(2)`, wrapper de glibc, `write()`/`read()` directo vs `printf()`. |

## Gestión de memoria

| Nivel | Resumen | Qué se ve |
|---|---|---|
| 1 | El SO reparte la memoria para los procesos. | Memoria como recurso limitado y compartido. |
| 2 | Cada proceso ve un espacio de direcciones propio, traducido por hardware. | Direcciones lógicas vs físicas, MMU, base/límite o paginación. |
| 3 | Linux gestiona memoria virtual por páginas, con swap. | `/proc/<pid>/maps`, `mmap()`, `malloc` vs `sbrk`, `-fsanitize=address` (ya usado en P0), segfault real. |

## Jerarquía de memoria

| Nivel | Resumen | Qué se ve |
|---|---|---|
| 1 | No toda la memoria es igual de rápida ni de grande; hay niveles. | Analogía mesa-cajón-armario-almacén, velocidad vs capacidad vs coste. |
| 2 | Registros, caché, RAM y disco. | Localidad temporal/espacial, ejemplo matriz por filas vs columnas. |
| 3 | ciertos/fallos de caché, TLB, *page cache*. | `perf stat` (cache-misses, page-faults), impacto medible en tiempo de ejecución, *page cache* del sistema de ficheros. |

## Comunicación entre procesos (IPC)

| Nivel | Resumen | Qué se ve |
|---|---|---|
| 1 | Los procesos están aislados; para comunicarse necesitan un mecanismo del SO. | Analogía de habitaciones incomunicadas: un tubo para pasar notas (tubería), un buzón con mensajes (cola de mensajes) o una notificación del móvil que interrumpe sin más info (señal) y el proceso decide luego que hacer. |
| 2 | Hay canales para flujo de datos y canales solo para notificar. | Tubería (flujo de bytes, sin estructura, FIFO) vs cola de mensajes (mensajes con tipo/tamaño) vs señal (aviso asíncrono sin datos, interrumpe al proceso receptor). |
| 3 | Linux implementa cada mecanismo con syscalls | tuberías `pipe()`, `mkfifo()`, colas de mensajes (`msgget`/`msgsnd`/`msgrcv`), señales (`kill`, `signal`/`sigaction`). |

## Concurrencia y sincronización

| Nivel | Resumen | Qué se ve |
|---|---|---|
| 1 | Cuando varias tareas comparten algo, el orden en que actúan puede dar resultados erróneos. | Analogía de dos personas escribiendo a la vez en la misma pizarra o sacando dinero del mismo cajero, idea de "turno" para evitar pisarse. |
| 2 | Dos hilos que leen y escriben la misma variable sin control pueden generar una condición de carrera; hace falta exclusión mutua. | Instrucciones no atómicas a nivel de ensamblador (leer-modificar-escribir en varios pasos), por qué una interrupción en medio rompe el resultado, idea de sección crítica. |
| 3 | En Linux la exclusión mutua y la sincronización se implementan con primitivas del sistema. | Semáforos (`sem_t`, `sem_open`/`sem_wait`/`sem_post`), memoria compartida (`shm_open`/`mmap`) entre procesos. |

## Sistemas de ficheros y RAID

| Nivel | Resumen | Qué se ve |
|---|---|---|
| 1 | El SO organiza los datos en ficheros y carpetas para que persistan y se puedan encontrar; a veces se guardan copias en varios discos por seguridad. | Analogía de archivador con carpetas y documentos, y de tener una copia de seguridad en otro cajón por si el primero se estropea. |
| 2 | Un fichero es una secuencia de bloques indexada por una estructura que guarda sus metadatos; un directorio es una tabla que traduce nombres a esa estructura. | Fichero como bloques enlazados/indexados, directorio como tabla nombre→nodo, permisos como control de acceso; combinar varios discos puede repartir datos (rendimiento) o duplicarlos (tolerancia a fallos). |
| 3 | Linux usa nodos-i y sistemas de ficheros concretos (ext4, etc.); varios discos se combinan en RAID con niveles estandarizados. | Nodo-i, superbloque, `stat`/`ls -i`, permisos en octal (`chmod`), `df`/`mount`; RAID 0 (*striping*), RAID 1 (*mirroring*), RAID 5 (paridad distribuida), `mdadm`, `/proc/mdstat`. |

