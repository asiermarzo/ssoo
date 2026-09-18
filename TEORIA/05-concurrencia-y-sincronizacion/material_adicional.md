# Tema 4 material adicional

## Formas de garantizar la concurrencia

- **Multiprogramación**: varios procesos en un sistema monoprocesador.
- **Multiprocesamiento**: varios procesos en un sistema multiprocesador.
- **Procesamiento distribuido**: varios procesos en sistemas de computadores múltiples y distribuidos (ejemplo: *clusters*).

La concurrencia comprende: comunicación entre procesos, compartición y competencia por los recursos, sincronización de la ejecución de varios procesos y asignación del tiempo de procesador.

## Contextos en que aparece la concurrencia

- **Múltiples aplicaciones**: compartición dinámica del procesador entre varias aplicaciones.
- **Aplicaciones estructuradas**: una aplicación se diseña como un conjunto de procesos concurrentes.
- **Estructura del sistema operativo**: el propio SO se implementa como un conjunto de procesos o hilos.

## Tareas del SO referentes a la concurrencia

Seguimiento de los procesos activos; asignación y liberación de recursos (tiempo de procesador, memoria, archivos, dispositivos de E/S); protección de datos y recursos de cada proceso frente a injerencias de otros; garantía de la correcta ejecución de un proceso independientemente de la velocidad de los demás.
