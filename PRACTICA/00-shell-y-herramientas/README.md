# P0 — Shell y herramientas

## Objetivo

Familiarizarse con el entorno de trabajo del curso: Linux, el intérprete de comandos (shell) y el ciclo de un programa C — **editar, compilar, ejecutar y depurar**.

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

| Parte | Significado |
|-------|-------------|
| `usuario` | tu nombre de usuario (el mismo que devuelve el comando `whoami`) |
| `equipo` | nombre de la máquina o *hostname* (el de `hostname`) |
| `~` | directorio de trabajo actual; `~` es tu carpeta personal, `/home/usuario` |
| `$` | shell lista, usuario sin privilegios (`#` si fueras `root`) |

## Moverse por directorios y mirar ficheros

Primeros comandos para orientarse en el sistema de ficheros:

| Comando | Uso |
|---------|-----|
| `pwd` | imprime el directorio de trabajo actual (*print working directory*) |
| `ls` | lista el contenido del directorio. `ls -l` formato largo (permisos, tamaño, fecha), `ls -a` incluye ocultos, `ls -la` ambos |
| `cd <dir>` | cambia de directorio. `cd ..` sube uno, `cd` o `cd ~` va a tu carpeta personal, `cd -` vuelve al anterior |
| `cat <fichero>` | vuelca el contenido completo de un fichero en la terminal |
| `more <fichero>` | muestra el fichero **página a página**: `Espacio` avanza, `Enter` una línea, `q` sale |
| `less <fichero>` | como `more` pero también permite retroceder y buscar (`/patrón`); `q` sale |
| `head` / `tail` | primeras / últimas líneas (10 por defecto); `tail -f` sigue un fichero que crece |
| `clear` | limpia la pantalla (`Ctrl+L` hace lo mismo) |
| `man <comando>` | manual del comando; se navega como `less`. También `<comando> --help` |

Atajos de la shell que conviene usar desde el principio:

- **`Tab`**: autocompleta nombres de comandos y de ficheros. Doble `Tab` lista las opciones posibles.
- **`↑` / `↓`**: recorren los comandos anteriores. `history` los lista todos.
- **`Ctrl+C`**: interrumpe el programa en ejecución (le manda la señal `SIGINT`).
- **`Ctrl+Z`**: suspende el programa en ejecución (`SIGTSTP`) y devuelve el prompt; luego comandos `fg` lo reanuda en primer plano y `bg` en segundo plano.
- **`Ctrl+\`**: como `Ctrl+C` pero con `SIGQUIT`, que además genera un *coredump* para depurar.

## Procesos

Un programa en ejecución es un *proceso*, identificado por un número (PID).

| Comando | Uso |
|---------|-----|
| `ps` | lista procesos. Habitual: `ps axu \| less` (todos, con detalle) |
| `pstree` | muestra los procesos como árbol, según quién creó a quién (`pstree -p` añade el pid) |
| `top` / `htop` | procesos ordenados por consumo de CPU y memoria, en tiempo real; `q` sale |
| `kill <pid>` | manda `SIGTERM` (15): pide al proceso que termine, y este puede capturarla para limpiar antes de salir. `kill -9 <pid>` manda `SIGKILL` (9), que el núcleo aplica directamente: no se puede capturar ni ignorar |
| `killall <nombre>` | como `kill` pero por nombre de programa en vez de pid|
| `jobs` / `fg` / `bg` | procesos lanzados en segundo plano con `&` desde esta terminal |

## Referencia rápida de comandos

### Ficheros y directorios

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

### Texto y búsqueda

`file` (tipo de fichero), `wc` (cuenta líneas/palabras/caracteres), `sort` (ordena líneas), `grep <patrón>` (líneas que casan un patrón), `find` (busca ficheros), `diff` (diferencias entre dos ficheros).

```bash
file hola          # -> "ELF 64-bit ... executable"
wc -l hola.c       # número de líneas del fichero
grep printf hola.c            # líneas que contienen "printf"
grep -rn "int main" .        # búsqueda recursiva, con número de línea
find . -name "*.c"           # todos los .c bajo el directorio actual
sort nombres.txt             # ordena alfabéticamente por líneas
diff hola.c hola_v2.c        # qué cambió entre dos versiones
```


### Compresión

| Comando | Uso |
|---------|-----|
| `zip` / `unzip` | comprime / descomprime en formato ZIP |
| `tar` | empaqueta y comprime: `tar czvf <destino>.tar.gz <origen>`; extrae: `tar xzvf <fichero>.tar.gz` |
| `gzip` / `gunzip` | comprime / descomprime un fichero |

### Permisos de ficheros

Cada fichero tiene tres ternas de permisos (usuario, grupo, otros), cada una con lectura (`r`), escritura (`w`) y ejecución (`x`). En notación octal cada terna es un dígito: `rw- r-- ---` → `110 100 000` → `640`.

Forma simbólica: `chmod <u|g|o> <+|-> <r|w|x> <fichero>`, p. ej. `chmod g+r fichero.o`. El bit de ejecución (`x`) es el que permite lanzar un binario con `./programa`.

## Editar archivos de código

Un programa en C es texto plano en un fichero `.c`. Se puede escribir con cualquier editor.

### Editores en la terminal

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

### Editores gráficos

```bash
geany hola.c &     # editor ligero con resaltado y compilación para C
gedit hola.c &     # editor sencillo de GNOME
kate  hola.c &     # editor de KDE
```

El `&` final lanza el editor en segundo plano para no bloquear la terminal.

### VS Code

```bash
code hola.c        # abre un fichero
code .             # abre la carpeta actual como proyecto
```

Trae terminal integrada (`` Ctrl+` ``) y depurador gráfico sobre `gdb`.

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
- `-g`: incluye información (nombres de variables, números de línea) para el depurador (ver [Depurar](#depurar)).
- `-O0`: sin optimizar; el código ejecutado se corresponde con el fuente, imprescindible para depurar paso a paso. `-O2` optimiza para producción pero reordena y elimina código.

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

### Código de salida y redirección

```bash
$ ./saluda Ana ; echo $?      # $? = código de salida del último comando (0 = éxito)
Hola, Ana
0
$ ./saluda ; echo $?
Uso: ./saluda <nombre>
1
```

```bash
./programa  < datos.txt       # entrada estándar (stdin) desde un fichero, útil para no escribir por teclado las entradas de prueba
./saluda Ana > salida.txt     # salida estándar (stdout) a un fichero
./programa 2> errores.txt     # salida de error (stderr) a un fichero
./programa  | less            # tubería: la salida va a la stdin a otro comando
```


## Herramientas del curso

- **gdb** — depurador de C/C++ (sección anterior). VS Code y `ddd` son interfaces gráficas sobre él.
- **valgrind** — instrumenta el binario para detectar errores de memoria (lecturas/escrituras fuera de rango, uso de memoria sin inicializar, fugas de `malloc`).
- **strace** — muestra la secuencia de llamadas al sistema (`open`, `read`, `write`, `fork`…) que ejecuta un programa; imprescindible en los temas de procesos y ficheros.
- **tmux** — multiplexor de terminales: varias terminales (paneles y ventanas) en una sola sesión, que sigue viva aunque se cierre la conexión. Útil para tener a la vez el editor, la compilación y la ejecución.

## Depurar

Depurar es ejecutar el programa de forma controlada (p.ej paso a paso) para ver **dónde y por qué** falla. Requiere compilar con `-g`.

Hay dos formas de hacerlo con `gdb`:

- **En vivo**: se lanza el programa desde `gdb` y se controla su ejecución (`Caso 1` a `Caso 3`).
- **Post-mortem (autopsia)**: el programa ya se ha caído y ha dejado un **coredump** —un fichero con la foto de toda su memoria (pila, variables, registros) en el instante de morir—. Se abre ese fichero con `gdb` y se examina  (`Caso 4`).
- **Adjuntándose a un proceso en marcha**: el programa se está ejecutando ahora mismo (típicamente colgado) y se engancha `gdb` a él sin reiniciarlo (`Caso 5`).

### Fichero de ejemplo: [`depura.c`](depura.c)

Debería sumar los enteros `1..N`, pero tiene fallos: con `N` pequeño da un resultado absurdo y con `N` grande el programa se cae.

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <N>\n", argv[0]);
        return 1;
    }

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
$ gcc depura.c -g -Wall -Wextra -o depura     # compila sin avisos...
$ ./depura 5
Suma 1..5 = 21855                             # ...pero el resultado es erróneo (debería ser 15)
$ ./depura 500
Segmentation fault (core dumped)              # y con N grande, se cae
```

### gdb — el depurador

| Comando (abreviatura) | Acción |
|-----------------------|--------|
| `run [args]` (`r`) | inicia el programa con esos argumentos; corre hasta un `break` o el final |
| `start [args]` | como `run` pero parará en el `main` |
| `next` (`n`) | ejecuta la línea actual **sin entrar** en las funciones |
| `step` (`s`) | ejecuta la línea actual **entrando** en las funciones |
| `continue` (`c`) | continua hasta el próximo `break` o el final |
| `print <expr>` (`p`) | muestra el valor de una variable o expresión: `print i`, `print valores[0]` |
| `break <línea\|función>` (`b`) | pone un punto de ruptura; `break main`, `break depura.c:18` |
| `info breakpoints` (`i b`) | lista los puntos de ruptura y su número |
| `delete [N]` (`d`) | borra el punto de ruptura `N`; sin número, borra todos. `disable`/`enable N` lo desactiva sin borrarlo |
| `list` (`l`) | muestra el código fuente alrededor de la línea actual |
| `backtrace` (`bt`) | pila de llamadas |
| `info locals` | valor de todas las variables locales |
| `frame <N>` (`f`) | cambia al marco `N` de la pila (para inspeccionar sus variables) |
| `attach <pid>` | engancha gdb a un proceso que ya está corriendo; equivale a lanzar `gdb -p <pid>` |
| `detach` | suelta el proceso adjuntado; sigue ejecutándose por su cuenta |
| `quit` (`q`) | salir de gdb |

### Caso 1 — localizar la caída (segfault)

```bash
$ gdb ./depura                 # abre el depurador para el programa depura
(gdb) run 500                  # ejecuta con argv[1] = "500", como ./depura 500
Program received signal SIGSEGV, Segmentation fault.    # el programa casca
0x0000555555555199 in main (argc=2, argv=0x7fffffffe2b8) at depura.c:22    # dónde: función main, línea 22
22              valores[i] = i;    # la instrucción exacta que provocó el fallo
(gdb) print i                  # ¿cuánto valía i?
$1 = 108                       # i = 108, fuera del array valores[100] (0..99)
(gdb) print n                  # ¿y n?
$2 = 500                       # el bucle llega hasta 500, mucho más allá del tamaño
(gdb) backtrace                # pila de llamadas hasta el punto del fallo
#0  main (argc=2, argv=0x7fffffffe2b8) at depura.c:22    # solo un marco: el fallo está en main
(gdb) quit                     # salir del depurador
```

`gdb` detiene el programa justo en la instrucción que provoca el fallo: la línea 22 escribe en `valores[i]` con `i = 108`, fuera del array `valores[100]` (índices válidos `0..99`).

### Caso 2 — entender el resultado erróneo

```bash
$ gdb ./depura                 # abre el depurador con el binario
(gdb) break 25                 # pon un punto de ruptura en la línea 25 (el bucle de la suma)
(gdb) run 5                    # ejecuta con argv[1] = "5", como ./depura 5
Breakpoint 1, main (...) at depura.c:25    # gdb para al llegar a la línea 25
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
$ gdb ./depura                 # abre el depurador con el binario
(gdb) start 3                   # 'start' es como 'run' pero con un breakpoint automático en main
Temporary breakpoint 1, main (argc=2, argv=...) at depura.c:18
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

`gdb -tui ./depura` —o, ya dentro, `tui enable` (o `Ctrl+X` `A`)— divide la pantalla: el código fuente arriba, con la línea actual resaltada y actualizándose en cada `next`/`step`, y la consola de gdb abajo.

- `Ctrl+X` `2` — añade una segunda ventana (registros, o ensamblador); púlsalo de nuevo para rotarla.
- `Ctrl+X` `O` — cambia el foco entre ventanas.
- flechas `↑` `↓` — hacen scroll de la ventana con el foco.
- `tui disable` — vuelve a la vista normal de solo texto.

### Caso 4 — autopsia de un coredump

Cuando el fallo ya ha ocurrido (por ejemplo, en la máquina de otra persona) se puede analizar el coredump que dejó, sin volver a ejecutar el programa.

Por defecto el sistema no escribe coredumps; hay que habilitarlos en la sesión de shell actual:

```bash
$ ulimit -c unlimited          # sin límite de tamaño para el coredump (por defecto: 0, desactivado)
$ ./depura 500
Segmentation fault (core dumped)
$ ls
core   depura   depura.c       # 'core' (a veces core.<pid>) es el volcado de memoria
```

Se abre pasando a `gdb` el binario y el coredump:

```bash
$ gdb ./depura core            # binario + coredump
Core was generated by './depura 500'.                   # qué orden lo produjo
Program terminated with signal SIGSEGV, Segmentation fault.
#0  0x0000555555555199 in main (argc=2, argv=...) at depura.c:22    # dónde murió
22              valores[i] = i;
(gdb) print i                  # ¿qué valor tenía i cuando el programa casco?
$1 = 108
(gdb) backtrace                # pila de llamadas en el momento del fallo
#0  main (argc=2, argv=...) at depura.c:22
(gdb) quit
```

No se puede `continue` ni `next`: el proceso ya no existe, solo su "cadáver". Sirve para `backtrace`, `print` e `info locals`.

> A veces los coredumps los recoge `systemd` en vez de dejar un fichero `core`. Se listan con `coredumpctl list` y se abren con `coredumpctl gdb depura`.

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
(gdb) frame 1                  # sube al marco de main
#1  main () at primos.c:25
25              if (es_primo(candidato)) {
(gdb) print candidato          # ¿qué candidato está probando?
$1 = 4
(gdb) print encontrados        # ya encontró 2 y 3
$2 = 2
(gdb) continue                 # deja correr un poco...
^C                             # Ctrl+C devuelve el control a gdb
(gdb) frame 1
(gdb) print candidato          # sigue en 4: el candidato no avanza
$3 = 4
(gdb) detach                   # suelta el proceso (sigue colgado, pero ya libre de gdb)
(gdb) quit
$ kill 4242                    # y se mata desde fuera
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


### tmux — varios paneles en una terminal

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

### Otras herramientas de diagnóstico

```bash
valgrind ./depura 5           # detecta accesos a memoria inválidos y fugas
strace ./hola                 # traza las llamadas al sistema que hace el programa
```

`valgrind` sobre el `depura.c` original señala directamente `Invalid write of size 4` en la línea 22 y `Use of uninitialised value` en la suma.

`strace` mostrará las llamadas al sistema que hace un programa durante su ejecución, se pueden filtrar por tipo, se verá más detalladamente en temas siguientes.


## Ejercicios propuestos

1. **Editar.** Crea con un editor (a tu elección) un fichero `datos.c` que imprima, con dos `printf` distintos, tu nombre y tu titulación. Compílalo y ejecútalo.
2. **Compilar.** Sobre una copia de `hola.c`, introduce tres errores de una vez (quita un `;`, una comilla `"` y una llave `}`). Compila, copia los mensajes de `gcc` y explica qué significa cada uno; luego corrígelos y recompila.
3. **Ejecutar.** Modifica `saluda.c` para que acepte **varios** nombres y salude a todos (recorre `argv` de `1` a `argc-1`). Pruébalo con 0, 1 y 3 argumentos y comprueba el valor de `echo $?` en cada caso.
4. **Depurar.** Compila `depura.c` con `-g`, reproduce la caída con `./depura 500` bajo `gdb`, localiza la línea culpable con `backtrace` y `print i`, aplica la corrección y verifica que `./depura 5` imprime `15` y `./depura 500` imprime `125250`.
5. **Depurar.** Escribe un programa corto que desreferencie un puntero `NULL` o divida entre cero. Observa cómo `gdb` detiene la ejecución en la instrucción exacta e identifica la línea con `list` y `backtrace`. Repite el análisis con `valgrind`.
