# Tema 3: Planificación de procesos

## Conceptos básicos

Un sistema operativo carga en memoria varios procesos a la vez (**multiplexación espacial**) y comparte la CPU mediante **multiplexación temporal**. 

Se denomina **planificador** (*scheduler*) al mecanismo que elige a qué hilo/proceso le toca ejecutarse (darle tiempo de CPU) y durante cuanto rato.

Sucesos que disparan la planificación: el proceso se duerme, cede la CPU (`yield`), realiza una llamada al sistema, finaliza, o salta el timer de interrupción (se acabó el cuanto asignado).

### Cesión de la CPU

Un proceso puede **ceder voluntariamente** la CPU (`yield`), o **verse obligado** a cederla por el planificador mediante el timer. Un **planificador apropiativo** (*preemptivo*) es el que puede forzar esa cesión.


### Cambio de contexto

Cada vez que el planificador decide sustituir el proceso en ejecución por otro, el SO realiza un **cambio de contexto**: guarda el estado del proceso saliente y restaura el del entrante. Detalle paso a paso en [`TEORIA/03`](../03-procesos-e-hilos/).


## Algoritmos de planificación

A menudo se combinan varias de estas estrategias:

### Por prioridad

- A cada proceso se le asigna una  **prioridad** (mayor cuanto menor es el valor asignado), por ejemplo números del 1 al 20.
- Las prioridades pueden reajustarse con el tiempo.
- Puede provocar **inanición** en los procesos de baja prioridad. Se soluciona con **envejecimiento** (*aging*: la prioridad aumenta con el tiempo de espera), o fijando un tiempo límite o una latencia máxima (ver siguiente sección).

### Latencia máxima

- A cada proceso se le garantiza una **latencia máxima**: el tiempo que puede pasar esperando antes de recibir la CPU, aunque haya procesos de mayor prioridad.
- Es propia de sistemas de tiempo real (sensores, control, audio/vídeo…).
- procesos interactivos, para garantizar una respuesta fluida aunque haya procesos de mayor prioridad esperando. Por ejemplo, videojuegos o ventanas gráficas.


### Round Robin (RR) o turno rotatorio

Los procesos preparados forman una cola circular. Cada uno recibe la CPU por un tiempo fijo (el **cuanto**); si no ha terminado al agotarlo, se le desaloja y pasa al final de la cola, dando paso al siguiente.

- Es la planificación más habitual en sistemas de **tiempo compartido**: reparte la CPU de forma equitativa entre todos los procesos listos.
- **No hay inanición**: todos los procesos reciben su turno tarde o temprano.
- El tamaño del cuanto es un compromiso: si es muy pequeño, el rendimiento baja por el exceso de cambios de contexto; si es muy grande, cada proceso termina antes de agotar el cuanto y equivale a atenderlos por orden de llegada, sin repartir turnos.

---

Más detalle (niveles de planificación, dispatcher, tipos de planificación, criterios de evaluación, FCFS, SJN/SJF, SRJF, colas múltiples, multiprocesador, y la planificación clásica en UNIX/Linux/Windows/macOS): [`material_adicional.md`](material_adicional.md).
