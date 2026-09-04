# Demo: depuración guiada con tmux

Levanta con una sola orden una sesión de tmux con tres paneles:

```
+--------------------+--------------+
|                    |  auto 'n'    |  manda 'n' a gdb cada segundo
|    gdb -tui        +--------------+
|    (panel 0)       |  killall     |  comando escrito, SIN ejecutar
+--------------------+--------------+
```

## Ficheros

| Fichero | Qué es |
|---|---|
| [`demo.c`](demo.c) | programa de juguete (suma `1..12`) para recorrer paso a paso |
| [`comandos.gdb`](comandos.gdb) | órdenes que gdb ejecuta al abrirse (`start`) |
| [`sesion.conf`](sesion.conf) | descripción de la sesión de tmux (paneles + comandos) |

## Uso

```bash
cd PRACTICA/00-shell-y-herramientas/tmux-demo
gcc -g -O0 -Wall -Wextra -o demo demo.c
tmux kill-server 2>/dev/null      # cierra cualquier sesión anterior (2>/dev/null: calla el error si no había ninguna)
tmux -f sesion.conf attach        # arranca tmux leyendo sesion.conf (que monta los paneles) y se conecta a la sesión
```

- **Panel grande:** `gdb -tui ./demo`, parado en `main`.
- **Panel arriba-derecha:** un bucle `while sleep 1; do tmux send-keys -t depura:debug.0 n Enter; done` que pulsa `n` en el panel de gdb cada segundo; el TUI avanza línea a línea solo.
- **Panel abajo-derecha:** queda escrito `killall demo` en el prompt **sin pulsar Enter**. Haz clic en ese panel y pulsa `Enter` cuando quieras terminar.

El ratón está activado (`set -g mouse on`): clic en un panel para pasarle el foco, rueda para hacer scroll.

## Salir

- Clic en el panel de abajo-derecha y `Enter` para lanzar el `killall demo`, o
- `Ctrl-b` y luego `d` (*detach*) y después `tmux kill-server`.

## Idea clave

`tmux send-keys -t <panel> <teclas>` escribe en otro panel como si lo teclearas tú;
`Enter` al final envía la orden. Así un panel puede teledirigir a otro: es lo que
hace el auto-stepper con gdb.
