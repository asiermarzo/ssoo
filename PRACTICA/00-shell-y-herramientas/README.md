# P0 — Shell y herramientas

## Objetivo

Familiarizarse con el intérprete de comandos (shell) y el ciclo de un programa C — **editar, compilar, ejecutar y depurar**.

## Cómo abrir una terminal

La terminal es una ventana donde se escriben comandos, uno por línea, y se pulsa `Enter` para ejecutarlos.

- Atajo de teclado: `Ctrl+Alt+T` (puede variar).
- Icono **Terminal** en la barra de aplicaciones o en el menú de aplicaciones (categoría *Sistema* / *Accesorios*).
- Desde el explorador de archivos: clic derecho sobre una carpeta → *Abrir en un terminal*.
- Dentro de VS Code: menú *Terminal → Nuevo terminal*, o `` Ctrl+` ``.

### El prompt

Al abrirla aparece una línea, el *prompt*, que indica que la shell espera un comando:

```
usuario@equipo:~$
```

<details> <summary> significado del prompt </summary>

| Parte | Significado |
|-------|-------------|
| `usuario` | tu nombre de usuario (el mismo que devuelve el comando `whoami`) |
| `equipo` | nombre de la máquina o *hostname* (el de `hostname`) |
| `~` | directorio de trabajo actual; `~` es tu carpeta personal, `/home/usuario` |
| `$` | shell lista, usuario sin privilegios (`#` si fueras `root`) |
</details>

## Moverse por directorios y mirar ficheros

| Comando | Uso |
|---------|-----|
| `pwd` | imprime el directorio de trabajo actual (*print working directory*) |
| `ls` | lista el contenido del directorio. `ls -l` formato largo (permisos, tamaño, fecha), `ls -a` incluye ocultos, `ls -la` ambos |
| `cd <dir>` | cambia de directorio. `cd ..` sube uno, `cd` o `cd ~` va a tu carpeta personal, `cd -` vuelve al anterior |
| `cat <fichero>` | vuelca el contenido completo de un fichero en la terminal |

<details><summary> Otros comandos: </summary>

| Comando | Uso |
|---------|-----|
| `man <comando/funcion>` | manual del comando o función |
| `more <fichero>` | muestra el fichero **página a página**: `Espacio` avanza, `Enter` una línea, `q` sale |
| `less <fichero>` | como `more` pero también permite retroceder y buscar (`/patrón`); `q` sale |
| `head` / `tail` | primeras / últimas líneas (10 por defecto); `tail -f` sigue un fichero que crece |
| `clear` | limpia la pantalla (`Ctrl+L` hace lo mismo) |
</details>


**Atajos** importantes de la **shell**:

- **`Tab`**: autocompleta nombres de comandos y de ficheros. Doble `Tab` lista las opciones posibles.
- **`↑` / `↓`**: recorren los comandos anteriores. `history` los lista todos.
- **`Ctrl+C`**: interrumpe el programa en ejecución (le manda la señal `SIGINT`).
<details><summary> otros: </summary>

- **`Ctrl+Z`**: suspende el programa en ejecución (`SIGTSTP`) y devuelve el prompt; luego comandos `fg` lo reanuda en primer plano y `bg` en segundo plano.
- **`Ctrl+\`**: como `Ctrl+C` pero con `SIGQUIT`, que además genera un *coredump* para depurar.
</details>

## Procesos

Un programa en ejecución es un *proceso*, identificado por un número (PID).

<details> <summary> Comandos para procesos: </summary>

| Comando | Uso |
|---------|-----|
| `ps` | lista procesos. Habitual: `ps -u $USER` (solo los míos) |
| `pstree` | muestra en árbol (`pstree -p` añade el pid) |
| `top` | procesos en tiempo real; `q` sale |
| `kill <pid>` | manda `SIGTERM` (15): pide al proceso que termine. `kill -9 <pid>` manda `SIGKILL` (9), termina el proceso |
| `killall <nombre>` | como `kill` pero por nombre en vez de pid|
<!--| `jobs` / `fg` / `bg` | procesos lanzados en segundo plano con `&` desde esta terminal |-->
</details>

## Referencia rápida de comandos

### Ficheros y directorios

<details> <summary> Comandos: </summary>

| Comando | Uso |
|---------|-----|
| `cp [-r] <origen> <destino>` | copia ficheros o directorios (`-r` recursivo) |
| `mv <origen> <destino>` | mueve o renombra |
| `mkdir <dir>` | crea un directorio |
| `rm [-r] [-f] [-i] <fichero>` | borra ficheros o directorios |
| `rmdir <dir>` | borra un directorio vacío |
| `touch <fichero>` | crea un fichero vacío o actualiza su fecha |
| `chmod <modo> <fichero>` | cambia permisos: `chmod 640 f` u `chmod g+r f` |
| `chown <usuario>:<grupo> <fichero>` | cambia propietario y grupo |
| `ln -s <objetivo> <enlace>` | crea un enlace simbólico |
</details>

### Texto y búsqueda

<details> <summary> Comandos: </summary>

| Comando | Uso |
|---------|-----|
| `file <fichero>` | tipo de fichero |
| `wc` | cuenta líneas/palabras/caracteres |
| `sort` | ordena líneas |
| `grep <patrón>` | líneas que casan un patrón |
| `find` | busca ficheros |
| `diff` | diferencias entre dos ficheros |

```bash
file hola                    # -> "ELF 64-bit ... executable" (tras compilar hola.c, ver sección Compilar)
wc -l hola.c                 # número de líneas del fichero
grep printf hola.c           # líneas que contienen "printf"
grep -rn "int main" .        # búsqueda recursiva, con número de línea
find . -name "*.c"           # todos los .c bajo el directorio actual
sort primos.c                # ordena alfabéticamente las líneas del fichero
diff hola.c saluda.c         # diferencias línea a línea entre dos ficheros
```
</details>

### Compresión

| Comando | Uso |
|---------|-----|
| `zip` / `unzip` | comprime / descomprime en formato ZIP |

<details> <summary> otros: </summary>

| Comando | Uso |
|---------|-----|
| `tar` | empaqueta y comprime: `tar czvf <destino>.tar.gz <origen>`; extrae: `tar xzvf <fichero>.tar.gz` |
| `gzip` / `gunzip` | comprime / descomprime un fichero |
</details>

### Permisos de ficheros

Cada fichero tiene tres permisos (usuario, grupo, otros), cada uno se puede fijar con lectura (`r`), escritura (`w`) y ejecución (`x`). Normalmente representado en octal, por ejemplo 777 es rwxrwxrwx lectura, escritura y ejecución para todos, 640 es rw-r----- el propietario lee y escribe, el grupo solo lee, el resto nada.

- **usuario**: el propietario del fichero.
- **grupo**: un grupo de usuarios. Se consulta con `ls -l` (columna del grupo) o `groups`.
- **otros**: cualquier usuario .


<details> <summary> chmod para cambiar permisos </summary>

`chmod` se utiliza para cambiar los permisos de los ficheros `chmod <quién><operador><permiso> <fichero>`.

- **quién**: `u` (usuario/propietario), `g` (grupo), `o` (otros), `a` (todos)
- **operador**: `+` añade el permiso, `-` lo quita, `=` lo deja exactamente así (y quita el resto)
- **permiso**: `r`, `w`, `x`; se pueden combinar, p. ej. `rw`

```bash
chmod u+x programa      # añade ejecución al propietario (típico tras compilar o crear un script)
chmod go-w fichero.txt  # quita escritura a grupo y otros: solo el propietario podrá modificarlo
```

El bit de ejecución (`x`) es el que permite lanzar un binario con `./programa`.

`chmod` también acepta la notación **octal**: se suman los pesos de cada permiso (`r`=4, `w`=2, `x`=1) y el resultado forma un dígito por terna (usuario, grupo, otros), fijando los tres a la vez.

```bash
chmod 777 script.sh   # rwxrwxrwx: lectura, escritura y ejecución para todos (úsalo con cuidado)
chmod 640 datos.txt   # rw-r-----: el propietario lee y escribe, el grupo solo lee, el resto nada
```
</details>


Este sistema de permisos también se usa para directorios, tuberías con nombre ([`mkfifo`](../03-pipes-y-fifos/README.md)) o memoria compartida ([`shm_open`](../06-memoria-compartida-y-semaforos/README.md)). Se verá en esas prácticas.

## Editar archivos de código C

Un programa en C es texto plano en un fichero `.c`. Se puede escribir con cualquier editor.

<details>  <summary> Editores en la terminal </summary>

<!-- 
**`nano`** — el más sencillo; muestra los atajos en pantalla (`^` significa `Ctrl`):

```bash
nano hola.c
```

| Atajo | Acción |
|-------|--------|
| `Ctrl+O` | guardar (*write out*) |
| `Ctrl+X` | salir |
| `Ctrl+K` / `Ctrl+U` | cortar / pegar línea |
| `Ctrl+W` | buscar |

-->

**`vim`** — más potente y presente en cualquier máquina, pero tiene *modos*. Supervivencia mínima:

```bash
vim hola.c
```

| Tecla | Acción |
|-------|--------|
| `i` | entra en **modo inserción** (escribir texto) |
| `Esc` | vuelve a **modo normal** (para dar órdenes) |
| `:w` | guardar |
| `:q` | salir · `:q!` salir descartando cambios · `:wq` guardar y salir |
| `dd` / `yy` / `p` | borrar / copiar / pegar línea (en modo normal) |

</details>

### Editores gráficos

```bash
geany hola.c &     # editor ligero con resaltado y compilación para C
kate  hola.c &     # editor de KDE
code hola.c &        # visual code en un fichero
code . &            #visual code, carpeta actual es el proyecto
```

El `&` final lanza el editor en segundo plano para no bloquear la terminal.


### Fichero de ejemplo: [`hola.c`](hola.c)

```c
#include <stdio.h>

int main(void) {
    printf("Hola, Sistemas Operativos\n");
    return 0;
}
```

## Compilar

El compilador `gcc` transforma el `.c` en un binario ejecutable.

```bash
# 1. Mínima: solo el fuente. El binario se llama a.out
gcc hola.c

# 2. Estricta: nombra el binario y activa todos los avisos
gcc hola.c -Wall -Wextra -o hola

# 3. Para depurar: añade símbolos (-g) y desactiva optimizaciones (-O0)
gcc hola.c -Wall -Wextra -g -O0 -o hola 
```

- `-o hola`: nombre del binario de salida (sin `-o`, el binario se llama `a.out`).
- `-Wall -Wextra`: activan **todos los avisos**
- `-g`: incluye información  para el depurador (nombres de variables, números de línea) (ver [Depurar](#depurar)).
- `-O0`: sin optimizar, imprescindible para depurar paso a paso. `-O2` optimiza para producción pero reordena y elimina código.

## Ejecutar

El binario se lanza con `./` delante (la shell no busca ejecutables en `.` por seguridad):

```bash
$ ./hola
Hola, Sistemas Operativos
```

### Argumentos de línea de comandos: [`saluda.c`](saluda.c)

```c
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <nombre>\n", argv[0]);
        return 1;
    }
    printf("Hola, %s\n", argv[1]);
    return 0;
}
```

```bash
$ gcc saluda.c -Wall -Wextra -o saluda
$ ./saluda Ana
Hola, Ana
$ ./saluda
Uso: ./saluda <nombre>
```

### Redirección


```bash
./programa  < datos.txt       # entrada estándar (stdin) desde un fichero, útil para no escribir por teclado las entradas de prueba
./saluda Ana > salida.txt     # salida estándar (stdout) a un fichero
./programa 2> errores.txt     # salida de error (stderr) a un fichero
./programa  | less            # tubería: la salida va a la stdin a otro comando
```

### Ejemplo: [`dec2rom.c`](dec2rom.c) / [`rom2dec.c`](rom2dec.c) / [`gen_rand.c`](gen_rand.c)

`dec2rom` lee un entero por consola y escribe su número romano; `rom2dec` hace lo contrario. `gen_rand` genera números aleatorios según `argv`: `[1]` cuántos (16 por defecto), `[2]` máximo (3999), `[3]` mínimo (1).

```bash
$ gcc dec2rom.c -o dec2rom
$ gcc rom2dec.c -o rom2dec
$ gcc gen_rand.c -o gen_rand

# desde teclado (stdin); Ctrl+D termina la entrada
$ ./dec2rom
1994
MCMXCIV

# números generados por gen_rand pasados por tuberia a dec2rom.
# resultado: genera 16 números romanos al azar
$ ./gen_rand | ./dec2rom

# ida y vuelta y comprobación
$ ./gen_rand 1000 > numeros.txt # 1000 números aleatorios a un fichero,
$ ./dec2rom < numeros.txt | ./rom2dec > vuelta.txt # numeros.txt -> romano -> decimal -> vuelta.txt
$ diff numeros.txt vuelta.txt   # sin salida -> son iguales; tanto rom2dec como dec2rom probablemente funcionen
```

```mermaid
flowchart LR
    A[numeros.txt] -->|stdin| B[dec2rom]
    B -->|stdout - tubería - stdin| C[rom2dec]
    C -->|stdout| D[vuelta.txt]
    A -.->|diff| D
```

<!-- 
### Código de salida
```bash
$ ./saluda Ana ; echo $?      # $? = código de salida del último comando (0 = éxito)
Hola, Ana
0
$ ./saluda ; echo $?
Uso: ./saluda <nombre>
1
```
-->

<!-- 
## Herramientas del curso

- **gdb** — depurador de C/C++ (sección anterior). VS Code y `ddd` son interfaces gráficas sobre él.
- **valgrind** — instrumenta el binario para detectar errores de memoria (lecturas/escrituras fuera de rango, uso de memoria sin inicializar, fugas de `malloc`).
- **strace** — muestra la secuencia de llamadas al sistema (`open`, `read`, `write`, `fork`…) que ejecuta un programa; imprescindible en los temas de procesos y ficheros.

- **tmux** — multiplexor de terminales: varias terminales (paneles y ventanas) en una sola sesión, que sigue viva aunque se cierre la conexión. Útil para tener a la vez el editor, la compilación y la ejecución.
-->

## Depurar

Depurar es ejecutar un programa  (p.ej paso a paso) para ver **dónde y por qué** falla. Requiere compilar con `-g`.

Hay tres formas de hacerlo con `gdb`:

- **En vivo**: se lanza el proceso desde `gdb` y se controla su ejecución (`Caso 1` a `Caso 3`).
- **Post-mortem (autopsia)**: el proceso ya se ha caído y ha dejado un **coredump** —un fichero con su memoria (pila, variables, registros) en el instante de morir—. Se abre ese fichero con `gdb` y se examina  (`Caso 4`).
- **Adjuntándose a un proceso en marcha**: el programa se está ejecutando ahora mismo (típicamente colgado) y se engancha `gdb` (`Caso 5`).

### Fichero de ejemplo: [`suma.c`](suma.c)

Debería sumar los enteros `1..N`, pero tiene fallos: con `N` pequeño da un resultado absurdo y con `N` grande el programa casca.

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    int valores[100];

    for (int i = 1; i <= n; i++)
        valores[i] = i;

    long suma = 0;
    for (int i = 0; i < n; i++)
        suma += valores[i];

    printf("Suma 1..%d = %ld\n", n, suma);
    return 0;
}
```

```bash
$ gcc suma.c -g -Wall -Wextra -o suma     # compila sin avisos...
$ ./suma 5
Suma 1..5 = 21855                             # ...pero el resultado es erróneo (debería ser 15)
$ ./suma 500
Segmentation fault (core dumped)              # y con N grande, se cae
```

### gdb — el depurador


<details> <summary> comandos: </summary>

| Comando (abreviatura) | Acción |
|-----------------------|--------|
| `run [args]` (`r`) | inicia el programa con esos argumentos; corre hasta un `break` o el final |
| `start [args]` | como `run` pero parará en el `main` |
| `next` (`n`) | ejecuta la línea actual **sin entrar** en las funciones |
| `step` (`s`) | ejecuta la línea actual **entrando** en las funciones |
| `continue` (`c`) | continua hasta el próximo `break` o el final |
| `print <expr>` (`p`) | muestra el valor de una variable o expresión: `print i`, `print valores[0]` |
| `list` (`l`) | muestra el código fuente alrededor de la línea actual |
| `backtrace` (`bt`) | pila de llamadas |
| `frame <N>` (`f`) | cambia al marco `N` de la pila (para inspeccionar sus variables) |
| `attach <pid>` | engancha gdb a un proceso que ya está corriendo; equivale a lanzar `gdb -p <pid>` |
| `detach` | suelta el proceso adjuntado; sigue ejecutándose por su cuenta |
| `break <línea\|función>` (`b`) | pone un punto de ruptura; `break main`, `break suma.c:18` |
| `info breakpoints` (`i b`) | lista los puntos de ruptura y su número |
| `delete [N]` (`d`) | borra el punto de ruptura `N`; sin número, borra todos. `disable`/`enable N` lo desactiva sin borrarlo |
| `info locals` | valor de todas las variables locales |
| `quit` (`q`) | salir de gdb |

</details>

Cada comando se puede escribir completo o con su abreviatura (`next` o `n`, `step` o `s`...). Pulsar `Enter` sin escribir nada repite el último comando: es habitual dar `n` una vez y luego solo `Enter` para ir avanzando línea a línea.


### Caso 1 — localizar la caída (segfault)

```bash
$ gdb ./suma                 # abre el depurador para el programa suma
(gdb) run 500                  # ejecuta con argv[1] = "500", como ./suma 500
Program received signal SIGSEGV, Segmentation fault.    # el programa casca
0x0000555555555199 in main (argc=2, argv=0x7fffffffe2b8) at suma.c:22    # dónde: función main, línea 22
22              valores[i] = i;    # la instrucción exacta que provocó el fallo
(gdb) print i                  # ¿cuánto valía i?
$1 = 108                       # i = 108, fuera del array valores[100] (0..99)
(gdb) print n                  # ¿y n?
$2 = 500                       # el bucle llega hasta 500, mucho más allá del tamaño
(gdb) quit                     # salir del depurador
```

`gdb` detiene el programa justo en la instrucción que provoca el fallo: la línea 22 escribe en `valores[i]` con `i = 108`, fuera del array `valores[100]` (índices válidos `0..99`).

### Caso 2 — entender el resultado erróneo

```bash
$ gdb ./suma                 # abre el depurador con el binario
(gdb) break 25                 # pon un punto de ruptura en la línea 25 (el bucle de la suma)
(gdb) run 5                    # ejecuta con argv[1] = "5", como ./suma 5
Breakpoint 1, main (...) at suma.c:25    # gdb para al llegar a la línea 25
25          for (int i = 0; i < n; i++)    # línea donde está detenido, aún sin ejecutar
(gdb) print valores[0]         # imprime el primer elemento del array
$1 = 21845                     # nunca se le asignó nada - valor indeterminado
(gdb) print valores[1]         # el segundo elemento
$2 = 1                         # el primer bucle sí lo escribió (valores[1] = 1)
(gdb) print valores[5]         # el elemento de índice 5
$3 = 5                         # se escribió aquí, pero el bucle de suma no lo lee
(gdb) quit                     # salir del depurador
```

**Diagnóstico:** el primer bucle rellena `valores[1..n]` y el segundo suma `valores[0..n-1]`. Sobra `valores[0]` (basura) y falta `valores[n]`. Los índices de un array de C van de `0` a `n-1`.

**Corrección:**

```c
for (int i = 0; i < n; i++)
    valores[i] = i + 1;

long suma = 0;
for (int i = 0; i < n; i++)
    suma += valores[i];
```

### Caso 3 — recorrer el programa paso a paso

Sin buscar ningún fallo: ejecutar línea a línea y observar cómo cambian las variables.

```bash
$ gdb ./suma                 # abre el depurador con el binario
(gdb) start 3                   # 'start' es como 'run' pero con un breakpoint automático en main
Temporary breakpoint 1, main (argc=2, argv=...) at suma.c:18
18          int n = atoi(argv[1]);         # detenido aquí, aún sin ejecutar
(gdb) display n                # muestra n automáticamente tras cada paso
(gdb) next                     # ejecuta la línea 18 y para en la siguiente
21          for (int i = 1; i <= n; i++)
1: n = 3                       # ya tiene valor tras el atoi
(gdb) next                     # entra en el cuerpo del bucle
22              valores[i] = i;
(gdb) next                     # vuelve a la cabecera del for (i++)
21          for (int i = 1; i <= n; i++)
(gdb) print i                  # inspecciona i puntualmente
$1 = 1
(gdb) print valores[1]         # el bucle ya escribió esta posición
$2 = 1
(gdb) info locals              # todas las variables locales visibles ahora
i = 1
n = 3
(gdb) continue                 # deja correr hasta el final (no hay más breakpoints)
Suma 1..3 = 6
[Inferior 1 (process 12345) exited normally]
(gdb) quit                     # salir del depurador
```

`next` pasa por encima de las llamadas a función; `step` entra en ellas. `display <expr>` fija una expresión para verla en cada parada; `undisplay <n>` la quita.

Casi todo se abrevia: `n`, `s`, `c`, `p`, `b`… Y **`Enter` a secas repite el último comando**, así que se avanza dando solo a `Enter` tras el primer `next`.

### Modo TUI (código y ejecución a la vez)

`gdb -tui ./suma` —o, ya dentro, `tui enable` (o `Ctrl+X` `A`)— divide la pantalla: el código fuente arriba, con la línea actual resaltada y actualizándose en cada `next`/`step`, y la consola de gdb abajo.

- `Ctrl+X` `2` — añade una segunda ventana (registros, o ensamblador); púlsalo de nuevo para rotarla.
- `Ctrl+X` `O` — cambia el foco entre ventanas.
- flechas `↑` `↓` — hacen scroll de la ventana con el foco.
- `tui disable` — vuelve a la vista normal de solo texto.

### Caso 4 — autopsia de un coredump

Cuando el fallo ya ha ocurrido (por ejemplo, en la máquina de otra persona) se puede analizar el coredump que dejó, sin volver a ejecutar el programa.

Por defecto el sistema no escribe coredumps; hay que habilitarlos en la sesión de shell actual:

```bash
$ ulimit -c unlimited          # sin límite de tamaño para el coredump (por defecto: 0, desactivado)
$ ./suma 500
Segmentation fault (core dumped)
$ ls
core   suma   suma.c       # 'core' (a veces core.<pid>) es el volcado de memoria
```

Se abre pasando a `gdb` el binario y el coredump:

```bash
$ gdb ./suma core            # binario + coredump
Core was generated by './suma 500'.                   # qué orden lo produjo
Program terminated with signal SIGSEGV, Segmentation fault.
#0  0x0000555555555199 in main (argc=2, argv=...) at suma.c:22    # dónde murió
22              valores[i] = i;
(gdb) print i                  # ¿qué valor tenía i cuando el programa casco?
$1 = 108
(gdb) backtrace                # pila de llamadas en el momento del fallo
#0  main (argc=2, argv=...) at suma.c:22
(gdb) quit
```

No se puede `continue` ni `next`: el proceso ya no existe, solo su "cadáver". Sirve para `backtrace`, `print` e `info locals`.

> A veces los coredumps los recoge `systemd` en vez de dejar un fichero `core`. Se listan con `coredumpctl list` y se abren con `coredumpctl gdb suma`.

### Caso 5 — depurar un proceso en marcha

Se puede enganchar `gdb` al proceso mientras sigue vivo y ver qué está haciendo.

Fichero de ejemplo: [`primos.c`](primos.c). Debería imprimir los 5 primeros primos y terminar, pero se cuelga:

```bash
$ gcc primos.c -g -Wall -Wextra -o primos
$ ./primos
2
3
                               # ...y aquí se queda para siempre
```

En **otra terminal** se busca el PID y se adjunta el depurador:

```bash
$ pgrep primos                 # averigua el PID del proceso
4242
$ gdb -p 4242                  # engancha gdb al proceso 4242 (puede requerir sudo, ver nota)
...
es_primo (n=4) at primos.c:15
15              if (n % d == 0)     # gdb congela el proceso justo donde estaba
(gdb) backtrace                # ¿dónde está atascado?
#0  es_primo (n=4) at primos.c:15
#1  main () at primos.c:25
(gdb) frame 1                  # sube del marco de es_primo al main
#1  main () at primos.c:25
25              if (es_primo(candidato)) {
(gdb) print candidato          # ¿qué candidato está probando?
$1 = 4
(gdb) print encontrados        #¿cuántos ha encontrado?
$2 = 2                         # ya encontró 2 (número 2 y 3)
(gdb) continue                 # deja correr durante un rato más...
^C                             # Ctrl+C devuelve el control a gdb
(gdb) frame 1
(gdb) print candidato          # sigue en 4: el candidato no avanza
$3 = 4
(gdb) detach                   # suelta el proceso 
(gdb) quit
$ kill 4242                    # terminamos el proceso desde fuera
```

**Diagnóstico:** `candidato++` está **dentro** del `if (es_primo(...))`, así que solo avanza cuando el candidato es primo. Al llegar a `candidato = 4` (no primo) nunca se incrementa y el `while` itera eternamente.

**Corrección:** sacar el incremento fuera del `if`, para que se pruebe cada número una vez.

```c
while (encontrados < 5) {
    if (es_primo(candidato)) {
        printf("%d\n", candidato);
        encontrados++;
    }
    candidato++;
}
```

> **`Operation not permitted` al adjuntar.** Si `gdb -p` falla, usa `sudo gdb -p <pid>` o baja la protección en la sesión actual: `echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope`.

> Mientras `gdb` está adjuntado, el proceso queda **detenido**: no consume CPU ni avanza hasta que se hace `continue` o `detach`.

<!-- 
## tmux — varios paneles en una terminal

`tmux` (*terminal multiplexer*) parte una sola terminal en varios **paneles** dentro de una **sesión**, que sigue viva aunque cierres la terminal. Por ejemplo sirve para tener a la vez el código en `gdb`, un panel que lo teledirige y otro para dar órdenes; no necesita entorno gráfico y funciona incluso desde SSH.

**No hace falta aprender a usar `tmux`**: algunas prácticas traen sesiones ya montadas en un fichero `sesion.conf`,

#### Ejemplo: [`tmux-demo/`](tmux-demo/)

`sesion.conf` tiene esta distribución:

```
+--------------------+--------------+
|                    |  auto 'n'    |  manda 'n' a gdb cada segundo
|    gdb -tui        +--------------+
|    (paso a paso)   |  killall demo|  escrito, SIN ejecutar
+--------------------+--------------+
```

- **Panel grande:** `gdb -tui ./demo`, parado en `main`. Recibe una `n` (*next*) cada segundo, así que el TUI avanza línea a línea solo.
- **Panel arriba-derecha:** el bucle que envía esa `n` al panel de `gdb`.
- **Panel abajo-derecha:** queda escrito `killall demo` **sin pulsar Enter**; lo ejecutas tú para cortar la demo.

```bash
cd PRACTICA/00-shell-y-herramientas/tmux-demo
gcc demo.c -g -O0 -Wall -Wextra -o demo
tmux kill-server 2>/dev/null        # cierra las sesiones anteriores, sin mostrar errores
tmux -f sesion.conf attach          # arranca tmux con sesion.conf (que monta los paneles) y se conecta a la sesión
```

**Para salir:** pulsa `Ctrl-b` y luego `d` para desconectarte y después `tmux kill-server`.
-->

##  valgrind — errores de memoria

```bash
valgrind ./suma 5           # detecta accesos a memoria inválidos y fugas
```

`valgrind` sobre el `suma` original señala directamente `Invalid write of size 4` en la línea 22 y `Use of uninitialised value` en la suma.

## strace — mostrar llamadas al sistema

`strace` muestra, llamada a llamada, lo que un proceso le pide al kernel; útil cuando el fallo está en una llamada al sistema (abrir un fichero, permisos, red...) y el código por sí solo no explica el porqué.

```c
//archivo lee.c
#include <stdio.h>

int main(void) {
    FILE *f = fopen("datos.txt", "r");
    int c = fgetc(f);
    printf("Primer caracter: %c\n", c);
    fclose(f);
    return 0;
}
```

`datos.txt` no existe, así que `fopen` devuelve `NULL`; el código no lo comprueba y usa ese puntero en `fgetc`, lo que provoca un segfault.

```bash
gcc lee.c -Wall -Wextra -o lee
strace ./lee

```
Entre la salida (recortada), se ve el intento de apertura fallido y la señal que mata al proceso:

```
openat(AT_FDCWD, "datos.txt", O_RDONLY) = -1 ENOENT (No existe el fichero o el directorio)
...
--- SIGSEGV {si_signo=SIGSEGV, si_code=SEGV_MAPERR, si_addr=0x18} ---
+++ killed by SIGSEGV +++
```

La línea de `openat` explica el origen del fallo (el fichero no existe). Se puede filtrar por tipo de llamada, por ejemplo `strace -e trace=open,openat,read ./lee`.

Muchas llamadas al sistema devuelven `-1` si fallan y guardan el motivo en la variable `errno`; `strace` traduce ese valor a un nombre descripción. 

<details> <summary> Los códigos más habituales: </summary>

| Código | Significado | Ejemplo típico |
|--------|-------------|-----------------|
| `ENOENT` | No existe el fichero o directorio | abrir una ruta que no existe |
| `EACCES` | Permiso denegado | abrir un fichero sin permiso de lectura/escritura |
| `EEXIST` | Ya existe | crear un fichero que ya está ahí |
| `EISDIR` | Es un directorio | abrir para escritura algo que en realidad es un directorio |
| `ENOTDIR` | No es un directorio | usar como directorio algo que no lo es |
| `EBADF` | Descriptor de fichero inválido | leer o escribir tras haber hecho `close()` |
| `ENOSPC` | No queda espacio en el disco | escribir en un disco lleno |
| `EINTR` | La llamada fue interrumpida por una señal | se verá con detalle en el tema de señales |
</details> 

<!-- 
## Ejercicios propuestos

1. **Editar.** Crea con un editor (a tu elección) un fichero `datos.c` que imprima, con dos `printf` distintos, tu nombre y tu titulación. Compílalo y ejecútalo.
2. **Compilar.** Sobre una copia de `hola.c`, introduce tres errores de una vez (quita un `;`, una comilla `"` y una llave `}`). Compila, copia los mensajes de `gcc` y explica qué significa cada uno; luego corrígelos y recompila.
3. **Ejecutar.** Modifica `saluda.c` para que acepte **varios** nombres y salude a todos (recorre `argv` de `1` a `argc-1`). Pruébalo con 0, 1 y 3 argumentos y comprueba el valor de `echo $?` en cada caso.
4. **Depurar.** Compila `suma.c` con `-g`, reproduce la caída con `./suma 500` bajo `gdb`, localiza la línea culpable con `backtrace` y `print i`, aplica la corrección y verifica que `./suma 5` imprime `15` y `./suma 500` imprime `125250`.
5. **Depurar.** Escribe un programa corto que desreferencie un puntero `NULL` o divida entre cero. Observa cómo `gdb` detiene la ejecución en la instrucción exacta e identifica la línea con `list` y `backtrace`. Repite el análisis con `valgrind`.
-->