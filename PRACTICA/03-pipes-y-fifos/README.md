# P3 — Pipes y fifos


## `pipe`

```c
#include <unistd.h>
int pipe(int fds[2]);
```

- `fds`: array de 2 enteros que rellenará la función `pipe()`; después de la llamada, `fds[0]` tendrá el fd de lectura, `fds[1]` para escritura.
- Devuelve `0` si todo fue bien, `-1` (con `errno`) si hubo error.

Ejemplo completo: el hijo escribe por la pipe una agenda y el padre lee los contactos uno a uno.

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct {
    char nombre[32];
    char apellido[32];
    int edad;
    char telefono[10];
} contacto_t;

int main(void) {
    int tuberia[2];
    pipe(tuberia);

    if (fork() == 0) { // rama hijo
        close(tuberia[0]); // cierra lectura, solo escribe
        contacto_t agenda[3] = {
            {"Ana",   "Garcia", 30, "645111222"},
            {"Luis",  "Perez",  21, "633333444"},
            {"Marta", "Lopez",  41, "612555666"}
        };
        write(tuberia[1], agenda, sizeof(agenda));
        exit(0);
    }

    // rama padre
    close(tuberia[1]); // cierra escritura, solo lee
    for (contacto_t p; read(tuberia[0], &p, sizeof(p)) > 0;) {
        printf("%s %s, %d años, tel. %s\n",
               p.nombre, p.apellido, p.edad, p.telefono);
    }
}
```

## `mkfifo` — tuberías con nombre


```c
#include <sys/types.h>
#include <sys/stat.h>
int mkfifo(const char *pathname, mode_t modo);
```

- `pathname`: ruta donde se crea la FIFO.
- `modo`: permisos, como en `chmod` (p. ej. `0660`).
- Devuelve `0` si todo fue bien, `-1` (con `errno`) si hubo error.

También es habitual crear y borrar FIFOs directamente desde la shell:

```bash
mkfifo /tmp/mi_fifo   # crea la FIFO (-m opcional para los permisos)
rm /tmp/mi_fifo              # se borra como cualquier fichero
```

Ejemplo completo: la FIFO se crea desde la shell (`mkfifo /tmp/aleatorios`) y su ruta se pasa por parámetro a los programas. `generador.c` escribe un `long` al azar cada segundo y `lector.c` los va leyendo e imprimiendo. Se pueden lanzar varias copias de `generador` a la vez.

```c
// generador.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 2) { fprintf(stderr, "Uso: %s <fifo>\n", argv[0]); return 1; }

    srand(time(NULL) ^ getpid()); //semilla rand depende de tiempo y pid
    int fp = open(argv[1], O_WRONLY);
    for (;;) {
        long numero = rand();
        write(fp, &numero, sizeof(numero));
        sleep(1);
    }
}
```

```c
// lector.c
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc != 2) { fprintf(stderr, "Uso: %s <fifo>\n", argv[0]); return 1; }

    int fp = open(argv[1], O_RDONLY); 
    for (long numero; read(fp, &numero, sizeof(numero)) > 0;) {
        printf("recibido: %ld\n", numero);
    }
}
```

```bash
mkfifo /tmp/aleatorios
./generador /tmp/aleatorios &
./lector /tmp/aleatorios & #y desde distintas shell
./generador /tmp/aleatorios &   # se pueden lanzar tantos como se quiera


kill $(jobs -p)                 # mata los procesos en segundo plano
pkill -x lector #también valdría este comando
rm /tmp/aleatorios
```

## Redirección entre fds: `dup2`

```c
#include <unistd.h>
int dup2(int b, int a);
```

`a --> b`: a partir de la llamada, todo lo que se escriba (o lea) usando el descriptor `a` va en realidad a parar a `b`.

- `b`: descriptor ya abierto, será el destino real.
- `a`: descriptor que se sobrescribe.

La implicación importante: cualquier código que use `a` queda afectado. Por ejemplo, `STDOUT_FILENO` (fd 1) apunta normalmente a la consola; si hacemos `dup2(fichero, STDOUT_FILENO)`, los `printf` posteriores ya no escriben en pantalla, escriben en `fichero`.

Ejemplo:
```c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
    printf("esto se ve en pantalla\n");

    int fichero = open("salida.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    dup2(fichero, STDOUT_FILENO);   // a (STDOUT_FILENO) --> b (fichero)
    close(fichero);

    printf("esto va al fichero, no a la pantalla\n");
}
```

## Ejercicios propuestos

1. Programa que cree una tubería y un `fork()`. El padre genera 10 números aleatorios entre 1 y 3999 y los manda por la tubería (uno por segundo); el hijo los recibe, los convierte a números romanos y los imprime por pantalla. Al terminar los 10, cada proceso imprime que acaba y su PID.

   Pista: aquí basta con **una** tubería (el padre solo escribe, el hijo solo lee): tras el `fork`, cada rama cierra el extremo que no usa. Para convertir a número romano puedes utilizar esta función:

   ```c
   void imprime_romano(int decimal) {
       int valores[]     = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
       char *simbolos[]  = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
       const int n_simbolos = sizeof(valores) / sizeof(valores[0]);

       for (int i = 0; i < n_simbolos; i++) {
           for (; decimal >= valores[i]; decimal -= valores[i]) 
                printf("%s", simbolos[i]);
       }
       printf("\n");
   }
   ```

   Traza de ejecución (PID 4021 = padre, PID 4022 = hijo):

   ```
   [4021] genera 1994
   [4022] recibe 1994 -> MCMXCIV
   [4021] genera 58
   [4022] recibe 58 -> LVIII
   [4021] genera 3999
   [4022] recibe 3999 -> MMMCMXCIX
   ...
   [4021] termino, soy el proceso 4021
   [4022] termino, soy el proceso 4022
   ```

2. Programa que cree **dos** tuberías, haga un `fork()` y que padre e hijo hagan pingpong con un `int` diez veces en total (cinco en cada sentido).

   Pista: crea dos tuberías antes del `fork`, `padre_a_hijo` e `hijo_a_padre`. El padre escribe en `padre_a_hijo` y lee de `hijo_a_padre`; el hijo escribe en `hijo_a_padre` y lee de `padre_a_hijo`. El padre manda el primer valor (`1`); a partir de ahí, los dos ejecutan el mismo bucle: reciben un valor, lo incrementan, lo devuelven, y si el valor recibido es `10`, terminan en vez de responder.

   Traza de ejecución (PID 4021 = padre, PID 4022 = hijo):

   ```
   [4021] envía 1
   [4022] recibe 1 de 4021 -> envía 2
   [4021] recibe 2 de 4022 -> envía 3
   [4022] recibe 3 de 4021 -> envía 4
   [4021] recibe 4 de 4022 -> envía 5
   [4022] recibe 5 de 4021 -> envía 6
   [4021] recibe 6 de 4022 -> envía 7
   [4022] recibe 7 de 4021 -> envía 8
   [4021] recibe 8 de 4022 -> envía 9
   [4022] recibe 9 de 4021 -> envía 10
   [4021] recibe 10 de 4022 -> termina, soy el proceso 4021
   [4022] termina, soy el proceso 4022
   ```

    Otra forma más limpia de organizarlo es con `int fds[2][2]`, creando cada una con `pipe(fds[i])` antes del `fork`: así padre e hijo ejecutan exactamente el mismo código, y lo único que cambia es qué índices usan.

3. Programa que calcule la sucesión de Fibonacci (`f0 = 0`, `f1 = 1`, `fn = fn-1 + fn-2`) empleando **tres procesos** P1, P2 y P3 comunicándose por tuberías según el esquema siguiente. P3 va imprimiendo en pantalla los valores obtenidos, uno por segundo.

   ```mermaid
   flowchart LR
       P1["P1<br/>f0 = 0"]
       P2["P2<br/>f1 = 1"]
       P3["P3<br/>padre"]
       OUT[["pantalla"]]

       P1 -- p1_p2 --> P2
       P2 -- p2_p1 --> P1
       P1 -- p1_p3 --> P3
       P2 -- p2_p3 --> P3
       P3 -- stdout --> OUT

       classDef proceso fill:#cfe2f3,stroke:#2b6f99,color:#000;
       classDef salida fill:#d9d9d9,stroke:#333,color:#000;
       class P1,P2,P3 proceso;
       class OUT salida;
   ```

   P3 es el proceso padre: crea las cuatro tuberías (`p1_p2`, `p2_p1`, `p1_p3`, `p2_p3`) y hace `fork()` dos veces para lanzar a P1 y P2.

   - **P1** guarda el término par, empieza en `f0 = 0`. Manda ese valor por `p1_p3` (a P3) y por `p1_p2` (a P2); luego, en bucle: lee por `p2_p1` el valor que le manda P2, lo suma al que él guardaba, duerme 1 segundo y manda el resultado por `p1_p3` y `p1_p2`.
   - **P2** guarda el término impar, empieza en `f1 = 1`. Hace lo mismo que P1, pero leyendo de `p1_p2` y escribiendo en `p2_p1` y `p2_p3`.
   - **P3** en cada vuelta: lee un valor (alternando entre `p1_p3` y `p2_p3`) y lo imprime por pantalla, duerme 1 segundo.

   Traza de P3 (una lectura por segundo): `0, 1, 1, 2, 3, 5, 8, 13, 21,...`
