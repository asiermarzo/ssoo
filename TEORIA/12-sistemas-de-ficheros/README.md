# Sistemas de ficheros

## Contenidos

- Abstracción del sistema de ficheros: un archivo como conjunto de datos con nombre en un
  dispositivo de almacenamiento permanente.
- Conceptos de fichero y directorio. Tipos de fichero (ordinarios, directorios, especiales).
- Nombrado del fichero, propietarios y permisos (usuario/grupo/otros; lectura/escritura/
  ejecución; `chmod`, `chown`).
- Estructura y almacenamiento del fichero: asignación contigua, enlazada e indexada;
  inodos; bloques y gestión del espacio libre.
- Seguridad en los sistemas de ficheros: listas de control de acceso, cifrado.
- Sistemas de ficheros concretos (ext4, FAT, NTFS…) y el VFS de Linux.

## Del nombre al contenido: directorio → inodo → bloques

```mermaid
flowchart LR
    DIR["entrada de directorio<br/>( nombre , nº de inodo )"] --> INODE["inodo<br/>tipo · permisos · propietario · tamaño<br/>punteros a bloques"]
    INODE --> B0["bloque de datos 0"]
    INODE --> B1["bloque de datos 1"]
    INODE --> IND["bloque indirecto"]
    IND --> B2["bloque de datos 2"]
    IND --> B3["bloque de datos …"]
```

## Estrategias de asignación

```mermaid
flowchart TB
    A["Contigua: rápida, pero fragmentación externa"]
    B["Enlazada: sin fragmentación externa, acceso directo malo"]
    C["Indexada (inodos): bloque índice con los punteros"]
```

## Material

_(pendiente de añadir)_
