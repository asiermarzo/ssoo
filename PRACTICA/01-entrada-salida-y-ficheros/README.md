# P1 — Entrada y salida: consola, ficheros, file descriptors

## Descripción general

C ofrece dos niveles para trabajar con ficheros:

- **`FILE*`**: funciones que empiezan por `f` y trabajan con el tipo `FILE *` (un puntero a una estructura opaca). Secuencia habitual: declarar un `FILE *` y abrir con `fopen`, operar (lectura/escritura), cerrar con `fclose`. Internamente usa un *file descriptor*  + un buffer en espacio de usuario, el buffer acumula datos y hace menos llamadas al sistema (más optimizado), pero lo escrito no llega al fichero hasta `fflush`/`fclose` o hasta que el búfer se llena.
- **file descriptors (`int`)**: Es más bajo nivel, los file descriptors (enteros no negativos) representan archivos, directorios, tuberías o sockets. La ventaja es que un programa que usa `read`/`write` tiene el mismo código tanto si escribe/lee de la consola, de archivos, de sockets o de tuberías. 

Cada proceso arranca con tres descriptores abiertos: `0` = `STDIN_FILENO` (entrada), `1` = `STDOUT_FILENO` (salida), `2` = `STDERR_FILENO` (error). A nivel `stdio` esos tres descriptores están envueltos por los `FILE *` `stdin`, `stdout` y `stderr` (declarados en `<stdio.h>`)

Se puede pasar de un nivel a otro:

- de `FILE *` a descriptor: `int fd = fileno(fp);`
- de descriptor a `FILE *`: `FILE *fp = fdopen(fd, "r");` (el `mode` debe ser compatible con cómo se abrió el descriptor).

Solo hace falta cerrar el de más alto nivel. Si tienes un `FILE *`, cierra con `fclose(fp)` (que ya cierra el descriptor interno).

### Resumen de operaciones

| Operación | Consola (`stdio`) | Fichero con `FILE *` | Fichero con descriptor |
|-----------|-------------------|----------------------|------------------------|
| Escribir con formato | `printf(fmt, ...)` | `fprintf(fp, fmt, ...)` | `dprintf(fd, fmt, ...)` |
| Abrir | ya abierto (`stdin`, `stdout`, `stderr`) | `fopen(ruta, modo)` | `open(ruta, flags[, mode])` / `creat` |
| Cerrar | — (se cierran al terminar proceso) | `fclose(fp)` | `close(fd)` |
| Leer línea / bloque | `fgets(buf, n, stdin)` | `fgets(buf, n, fp)` / `fread` | `read(fd, buf, n)` |
| Leer con formato | `scanf(fmt, ...)` | `fscanf(fp, fmt, ...)` | — (parsear con sscanf tras `read`) |
| Escribir cadena / bloque | `puts(s)` | `fputs(s, fp)` / `fwrite` | `write(fd, buf, n)` |
| Fin de fichero / error | `feof` / `ferror` | `feof` / `ferror` | `read` devuelve `0` / `-1` |
| Vaciar el búfer | `fflush(stdout)` | `fflush(fp)` | — (no hay búfer de usuario) |
| Leer carácter | `getchar()` | `fgetc(fp)` | `read(fd, &c, 1)` |
| Escribir carácter | `putchar(c)` | `fputc(c, fp)` | `write(fd, &c, 1)` |
| Mover el cursor | — | `fseek` / `rewind` / `ftell` | `lseek(fd, off, whence)` |

## Salida con formato: `printf`, `fprintf`, `dprintf`

Las tres construyen el mismo texto a partir de una cadena de formato; solo cambia el destino.

```c
#include <stdio.h>
int printf (const char *fmt, ...);            /* a stdout */
int fprintf(FILE *fp, const char *fmt, ...);  /* a un FILE * (fichero, stdout, stderr...) */
int dprintf(int fd,   const char *fmt, ...);  /* a un fd: fichero, socket, pipe...*/
```

- `printf(fmt, ...)` es equivalente a `fprintf(stdout, fmt, ...)`.
- `dprintf` (POSIX.1-2008) útil cuando solo tienes un `fd` (una tubería, un socket) y no quieres envolverlo en un `FILE *`.
- **Devuelven** el número de caracteres escritos, o un valor negativo si hay error.
- Para escribir a una cadena en memoria: `snprintf(buf, sizeof buf, fmt, ...)` (preferible a `sprintf`, que no comprueba el tamaño).

### Anatomía de un especificador

```
%[flags][anchura][.precisión][longitud]conversión
```

| Parte | Valores | Efecto |
|-------|---------|--------|
| flags | `-` `0` `+` espacio `#` | `-` alinea a la izquierda; `0` rellena con ceros; `+`/espacio fuerzan signo; `#` añade `0x`, `0`, punto decimal |
| anchura | número | mínimo de caracteres; se rellena con espacios (o ceros) |
| precisión | `.número` | decimales en `%f`/`%e`; máx. de caracteres en `%s`; dígitos mínimos en enteros |
| longitud | `l` `ll` `h` `hh` `z` | tamaño del argumento: `%ld` long, `%lld` long long, `%zu` size_t |
| **conversión** | `d` `i` `u` `o` `x`/`X` `c` `s` `f` `e`/`E` `g` `p` `%` | tipo a imprimir (ver tabla siguiente) |

| Conversión | Tipo del argumento | Qué imprime |
|------------|--------------------|-------------|
| `%d` / `%i` | `int` | entero decimal con signo |
| `%s` | `char *` | cadena terminada en `'\0'` |
| `%f` | `double` | decimal en punto fijo (`3.140000`) |
| `%p` | `void *` | dirección de memoria (hex) |
| `%e` / `%E` | `double` | notación científica (`3.14e+00`) |
| `%g` | `double` | `%e` o `%f`, el más corto |
| `%u` | `unsigned int` | entero decimal sin signo |
| `%o` | `unsigned int` | entero en octal |
| `%x` / `%X` | `unsigned int` | entero en hexadecimal (minúsculas / mayúsculas) |
| `%c` | `int` | un carácter |
| `%%` | — | un `%` literal |

### Conversiones básicas

```c
int    n = 42;
float  x = 3.5f;
double d = 2.71828;
char   c = 'A';

printf("%d\n",  n);     /* 42        — entero con signo */
printf("%i\n",  n);     /* 42        — en printf, %i es idéntico a %d */
printf("%c\n",  c);     /* A         — un único carácter */
printf("%f\n",  x);     /* 3.500000  — 6 decimales por defecto */
printf("%f\n",  d);     /* 2.718280  — %f sirve para float y double en printf */
```
### Anchura y precisión

```c
printf("|%5d|\n",   42);          /* |   42|   */
printf("|%-5d|\n",  42);          /* |42   |   */
printf("|%05d|\n",  42);          /* |00042|   */
printf("%.2f\n",    3.14159);     /* 3.14      */
printf("%8.2f\n",   3.14159);     /* |    3.14| */
printf("%s\n",    "abcdef");    /* abcdef       */
printf("%zu\n",     sizeof(int)); /* 4         */
```

### Varios especificadores en una llamada

```c
int    dia = 3, mes = 9, anio = 2026;
char   inicial = 'A';
double media = 7.456;

printf("%02d/%02d/%d\n", dia, mes, anio);       /* 03/09/2026 */
printf("%d + %d = %d\n", dia, mes, dia + mes);  /* 3 + 9 = 12 */
printf("%c. media %.1f\n", inicial, media);     /* A. media 7.5 */
```

### Valor y dirección de una variable

```c
#include <stdio.h>
int main(void) {
    int numero = 42;
    printf("valor     = %d\n", numero);          /* valor     = 42 */
    printf("dirección  = %p\n", (void *)&numero); /* dirección  = 0x7ffca3b4c8ec (varía en cada ejecución) */
}
```

`%p` espera un `void *`; por eso se hace el *cast* `(void*)&numero`.

## Entrada con formato: `scanf`, `fscanf`, `sscanf`

```c
#include <stdio.h>
int scanf (const char *fmt, ...);              /* desde stdin */
int fscanf(FILE *fp, const char *fmt, ...);    /* desde un FILE * */
int sscanf(const char *str, const char *fmt, ...); /* desde una cadena en memoria */
```

- Los argumentos son **punteros** a donde poner los valores parseados: `scanf("%d", &n)`. Para `%s` se pasa el array (ya es un puntero).
- **Devuelve** el número de asignaciones realizadas con éxito, o `EOF` si se llega al final del fichero / hay error antes de la primera asignación.
- Antes de `%d`, `%f`, `%s`... `scanf` **salta** los espacios en blanco (incluidos `'\n'` y tabuladores). `%c` y `%[...]` no los saltan.

### El tamaño se limita con la anchura

`%s` copia caracteres hasta el primer espacio en blanco, **sin mirar** cuánto mide el array de destino. Si la entrada es más larga, `scanf` escribe fuera del buffer: eso es un *buffer overflow*, un fallo de seguridad, es el mismo motivo por el que `gets` está prohibida.

La defensa es indicar en el especificador la anchura máxima a copiar, **una menos** que el tamaño del array (hay que dejar sitio para el `'\0'`):

```c
char nombre[64];
scanf("%63s", nombre);     /* copia como mucho 63 caracteres + '\0'; nunca desborda */
```

## E/S con `stdio` (`FILE *`)

### abrir y cerrar `fopen` / `fclose`

```c
#include <stdio.h>
FILE *fopen(const char *path, const char *mode);
int   fclose(FILE *stream);
```

- `path`: ruta del fichero.
- `mode`: `r` (lectura, archivo debe existir), `w` (escritura, crea/trunca), `a` (añadir al final, crea si no existe), `r+` (lectura+escritura, debe existir), `w+` (lectura+escritura, crea/trunca), `a+` (lectura+escritura al final).
- **Devuelve** (`fopen`): puntero `FILE *`, o `NULL` si hay error.
- **Devuelve** (`fclose`): `0` si se cierra correctamente, `EOF` si hay error.

```c
#include <stdio.h>
int main(void) {
    FILE *fp = fopen("fichero.txt", "r");
    if (fp == NULL) return 1;
    /* ... */
    fclose(fp);
}
```

### Lectura

```c
int   fgetc(FILE *f);                                   /* un carácter; EOF al final o error */
char *fgets(char *buf, int tam, FILE *f);               /* una línea (hasta tam-1 o '\n') */
size_t fread(void *ptr, size_t tam, size_t nmemb, FILE *f);  /* nmemb registros de tam bytes */
int   fscanf(FILE *f, const char *formato, ...);        /* como scanf, desde fichero */
```

`fread` devuelve el número de **elementos** leídos (no bytes); usar `feof`/`ferror` para distinguir fin de fichero de error.

```c
#include <stdio.h>
int main(void) {
    FILE *archivo = fopen("prueba.txt", "r");
    if (archivo == NULL) { printf("Error de apertura.\n"); return 1; }
    for (char linea[100]; fgets(linea, sizeof linea, archivo) != NULL; )
        printf("%s", linea);
    fclose(archivo);
}
```

### Escritura

```c
int   fputc(int c, FILE *f);                             /* un carácter */
int   fputs(const char *buf, FILE *f);                   /* una cadena (sin '\n' ni '\0') */
size_t fwrite(const void *ptr, size_t tam, size_t nmemb, FILE *f);  /* nmemb registros */
int   fprintf(FILE *f, const char *formato, ...);        /* como printf, a fichero */
```

`fwrite` y `fread` son útiles para structs fijos:

```c
typedef struct { char nombre[25]; int edad; float sueldo; } registro_t;
registro_t registro = { "Ana Ruiz", 30, 1850.0f };
/* escribir */ fwrite(&registro, sizeof registro, 1, fichero);
/* leer    */  fread(&registro, sizeof registro, 1, fichero);
```

## E/S con fd (descriptores de fichero)

### `open` / `creat`

```c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int open(const char *pathname, int flags);
int open(const char *pathname, int flags, mode_t mode);
int creat(const char *pathname, mode_t mode);   /* == open con O_WRONLY|O_CREAT|O_TRUNC */
```

- `flags`: uno de `O_RDONLY`, `O_WRONLY`, `O_RDWR`, combinable con OR (`|`) con `O_CREAT` (crea si no existe), `O_EXCL` (error si ya existe), `O_APPEND` (escribe siempre al final), `O_TRUNC` (trunca a 0).
- `mode`: permisos del fichero si se crea (p. ej. `S_IRUSR|S_IWUSR`, o `0644`).
- **Devuelve**: el descriptor (el entero libre más bajo), o `-1` si hay error y fija la variable global `errno` (`<errno.h>`) con el código concreto del fallo.

### `read` / `write` / `close`

```c
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
int     close(int fd);
```

- `read`: **devuelve** el nº de bytes leídos, `0` al llegar al final del fichero, `-1` si hay error.
- `write`: **devuelve** el nº de bytes escritos, o `-1` si hay error.
- `close`: `0` si correcto, `-1` si error.

Errores típicos (`errno`, `<errno.h>`): `EACCES` (sin permiso), `ENOENT` (no existe), `EEXIST` (ya existe con `O_EXCL`), `ENOSPC` (sin espacio), `EBADF` (descriptor inválido). Mostrar el error con `perror("mensaje")` (`#include <stdio.h>`).

### Ejemplo: escritura y lectura por descriptores

```c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    char ruta[] = "/tmp/prueba";
    char mensaje[] = "hola\n";
    char buffer[50];

    int fd = open(ruta, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    ssize_t n = write(fd, mensaje, strlen(mensaje));
    close(fd);

    fd = open(ruta, O_RDONLY);
    n  = read(fd, buffer, n);
    write(1, buffer, n);          /* 1 = STDOUT_FILENO */
    close(fd);
}
```

### Ejemplo: copia de fichero

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char buffer[2048];
    if (argc != 3) { fprintf(stderr, "Se precisan 2 argumentos\n"); exit(1); }

    int fdold = open(argv[1], O_RDONLY);
    if (fdold == -1) { perror("open origen"); exit(1); }
    int fdnew = creat(argv[2], 0666);
    if (fdnew == -1) { perror("creat destino"); exit(1); }

    for (int cuenta; (cuenta = read(fdold, buffer, sizeof buffer)) > 0; )
        write(fdnew, buffer, cuenta);
    close(fdold); close(fdnew);
}
```

### Argumentos de la línea de comandos: `argc` / `argv`

la función `main` puede recibir los argumentos con que se invocó el programa:

```c
int main(int argc, char *argv[])
```

- `argc`: número de argumentos, contando el propio nombre del programa.
- `argv`: array de cadenas. `argv[0]` es el nombre del programa; `argv[1]`..`argv[argc-1]` son los argumentos; `argv[argc]` es `NULL`.

Con `./copia origen.txt destino.txt`: `argc == 3`, `argv[0] == "./copia"`, `argv[1] == "origen.txt"`, `argv[2] == "destino.txt"`.

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) { fprintf(stderr, "uso: %s <numero>\n", argv[0]); return 1; }
    int n = atoi(argv[1]);      /* los argumentos son cadenas: convertir con atoi/strtol */
    printf("el doble de %d es %d\n", n, 2 * n);
}
```

### Operaciones opcionales

Con `FILE *`:

| Función | Prototipo | Descripción |
|---------|-----------|-------------|
| `feof` | `int feof(FILE *f)` | ≠0 si el cursor alcanzó el final del fichero |
| `ferror` | `int ferror(FILE *f)` | ≠0 si hubo error en una operación previa |
| `rewind` | `void rewind(FILE *f)` | Sitúa el cursor al principio del fichero |
| `fflush` | `int fflush(FILE *f)` | Vuelca el búfer de salida al fichero |
| `remove` | `int remove(const char *nombre)` | Elimina el fichero |

Con descriptores (`#include <unistd.h>`):

| Función | Prototipo | Descripción |
|---------|-----------|-------------|
| `lseek` | `off_t lseek(int fd, off_t offset, int whence)` | Mueve el cursor (`whence`: `SEEK_SET` / `SEEK_CUR` / `SEEK_END`); devuelve la nueva posición, o `-1` si error |
| `unlink` | `int unlink(const char *pathname)` | Borra el fichero; `0` si correcto, `-1` si error |

## Ejercicios propuestos

1. Programa que abra un fichero cuyo nombre se pase como argumento y escriba en él `"Hola mundo"`.
2. Programa que abra un fichero cuyo nombre se pase como argumento, lea su contenido y lo imprima por pantalla. Se puede resolver de varias formas; conviene probarlas todas:
   - carácter a carácter (`fgetc` / `read` de 1 byte),
   - línea a línea (`fgets`),
   - por bloques de tamaño fijo (`fread` / `read` a un buffer de, p. ej., 512 bytes),
   - cargando el fichero entero a memoria y volcándolo después a consola de una vez.
3. Programa que cambie las vocales en minúscula de un fichero a mayúsculas (modificando el propio fichero). Como el reemplazo no cambia el tamaño, hay dos enfoques:
   - **Sobrescritura en el sitio**: abrir en lectura+escritura (`r+` / `O_RDWR`), leer el contenido a memoria, convertir, volver el cursor al principio (`rewind` / `lseek(fd, 0, SEEK_SET)`) y reescribir encima.
   - **Cerrar y reabrir**: leer todo, cerrar, reabrir con truncado (`w` / `O_WRONLY|O_TRUNC`) y escribir el resultado.
   La primera evita dejar el fichero vacío si el programa falla a mitad; la segunda es más simple. Conviene implementar ambas y comparar.
