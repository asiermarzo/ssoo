# P5 — Ejercicios extra

Cinco ejercicios pequeños y autocontenidos que construyen, paso a paso, las piezas de una shell. No sustituyen a la práctica (que exige la biblioteca `fragmenta` y la entrega completa), sino que sirven para practicar cada mecanismo por separado. Compilar todos con `make`.

## 1. Tokenizador (`extra_01.c`)

Implementar una función `trocea(linea, tokens, max)` que parta una línea por espacios y tabuladores y rellene un array de punteros terminado en `NULL` (como necesita `execvp`). El `main` lee líneas de la entrada estándar y muestra los tokens obtenidos.

## 2. Ejecutor de comandos (`extra_02.c`)

Bucle de shell mínimo: prompt, lectura de línea, troceado, comando interno `exit`, y ejecución de comandos externos con `fork` + `execvp` + `waitpid`, informando del código de salida cuando no es 0. Sin redirecciones ni tuberías.

## 3. Redirección de salida `>` (`extra_03.c`)

Ampliar el ejecutor para detectar `> fichero` en la línea, abrir el fichero con `O_WRONLY | O_CREAT | O_TRUNC` y redirigir la salida estándar del hijo con `dup2` antes del `execvp`.

## 4. Redirección `<`, `>` y `>>` (`extra_04.c`)

Añadir la redirección de entrada (`< fichero`) y la anexión (`>> fichero`), admitiendo varias redirecciones en cualquier orden dentro de la misma línea.

## 5. Una tubería y `SIGINT` (`extra_05.c`)

Añadir el soporte de una tubería simple `cmd1 | cmd2` con `pipe` + `fork` + `dup2`, y hacer que la shell ignore `SIGINT` (`Ctrl-C` no la mata) mientras que los procesos hijo lo reciben con la acción por defecto.
