# Dispositivos de E/S

## Contenidos

- Conceptos básicos: controladores como interfaz software entre el hardware y el SO.
  Dispositivos de bloque y de carácter.
- Funciones de entrada/salida: E/S programada, dirigida por interrupciones y por acceso
  directo a memoria (DMA).
- Almacenamiento intermedio (*buffering*).
- Tipos de discos duros y geometría (cilindro, cabeza, sector).
- Planificación de discos: FCFS, SSTF, SCAN, C-SCAN, LOOK, C-LOOK.
- Caché de disco.
- Entrada/salida en Linux.

## E/S dirigida por interrupciones

```mermaid
sequenceDiagram
    participant P as Proceso
    participant SO as Núcleo · driver
    participant C as Controlador
    participant DEV as Dispositivo
    P->>SO: read() → llamada al sistema
    SO->>C: programa la operación
    SO-->>P: proceso BLOQUEADO (otro proceso usa la CPU)
    C->>DEV: ejecuta la transferencia (posible DMA)
    DEV-->>C: fin de transferencia
    C-->>SO: interrupción
    SO->>SO: copia datos / actualiza estado
    SO-->>P: proceso LISTO (datos disponibles)
```

## Planificación de disco

```mermaid
flowchart LR
    Q["cola de peticiones<br/>por nº de cilindro"] --> ALG{"algoritmo"}
    ALG --> FCFS["FCFS: en orden de llegada"]
    ALG --> SSTF["SSTF: la más cercana a la cabeza"]
    ALG --> SCAN["SCAN / LOOK: barrido en un sentido y vuelta"]
    ALG --> CSCAN["C-SCAN / C-LOOK: barrido siempre en el mismo sentido"]
```

## Material

_(pendiente de añadir)_
