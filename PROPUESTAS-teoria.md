# Propuestas: fragmentos ejecutables y animaciones (Temas 01–13)

Fichero temporal para revisar y marcar.

---

## Tema 01 — Introducción a los sistemas operativos

### Ejecutables
- [ ] 1. Compilador en vivo: editar una línea C (`a = b + c;`) en un panel y ver el ASM generado (`gcc -S`) refrescarse en otro — conecta con "Arquitectura básica del ordenador". 
- [ ] 2. Monolítico vs módulos: `lsmod`, `sudo modprobe pcspkr`, `rmmod`, con `dmesg | tail` en un panel paralelo.
- [ ] 3. Jerarquía de memoria medida: programa C que recorre arrays de tamaño creciente (cabe en L1→L2→RAM) con acceso dependiente (un solo ciclo, no una permutación cualquiera) y mide ns/acceso real con `clock_gettime`.
- [ ] 4. Monoprogramación vs multiprogramación: tres procesos (`A`/`B`/`C`) anclados a un solo núcleo con `taskset`, imprimiendo su letra sin parar — se ven rachas de turno de CPU en la propia terminal; al lado, `top -p` filtrado a esos 3 PIDs; y una segunda ventana con los mismos tres uno detrás de otro (monoprogramación real).

### Animaciones
- [ ] 1. Fetch-execute paso a paso: registros (PC/SP/regs/flags) + memoria como cajas; botón "step" avanza la instrucción `mov/add/jump` del README.
- [ ] 2. Pirámide de memoria animada: un dato "sube" de disco→RAM→caché→registro con tamaños/latencias reales; toggle hit/miss de caché.
- [ ] 3. Timeline evolución: scrubber manual→batch→multiprogramación→tiempo compartido, con barra de "utilización de CPU".
- [ ] 4. Monolítico vs microkernel: mismo servicio ("leer un fichero") como 1 salto vs varios saltos IPC cruzando la frontera usuario/kernel.

---

## Tema 02 — Espacio usuario y espacio kernel. Llamadas al sistema

### Ejecutables
- [ ] 1. `strace -c` vs sin instrumentar sobre un programa que hace varias syscalls en bucle, comparando tiempo y nº de llamadas.
- [ ] 2. Syscall cruda vs libc: `syscall(SYS_getpid)` al lado de `getpid()`, viendo con `strace` que generan la misma llamada real.
- [ ] 3. `lee.c` (de PRACTICA/00) bajo `strace -T -tt`, con el código fuente al lado — ver el `openat` fallando y el `SIGSEGV` posterior.
- [ ] 4. Cepo vs syscall: `division_cero.c` bajo `strace -f` (SIGFPE) al lado de una división normal que sí retorna.

### Animaciones
- [ ] 1. "Ventanilla segura": token que viaja usuario→wrapper libc→kernel→wrapper→usuario, con franja de color que cambia (verde=usuario, azul=kernel).
- [ ] 2. Árbol de decisión: elegir un escenario (tick de reloj / fichero inexistente / `read()`) y clasificarlo en Interrupción/Cepo/Llamada al sistema.
- [ ] 3. Bucle fetch con chequeo de interrupción: el PC avanza instrucción a instrucción, con opción de forzar una interrupción y ver el salto al manejador y la vuelta.

---

## Tema 03 — Procesos e hilos

### Ejecutables
- [ ] 1. `ps -eLf`/`pstree` en vivo sobre un `fork()`: dos paneles — uno lanza un programa que hace `fork()` y ambos duermen unos segundos, otro con `watch` sobre `ps`/`pstree -p` mostrando pid/ppid/estado cambiar en tiempo real.
- [ ] 2. Estados reales por `/proc/<pid>/status`: el mismo programa recorriendo *running* (bucle CPU), *sleeping* (`sleep()`) y *zombie* (hijo termina, padre no hace `wait()` aún) — leer el estado (R/S/Z) desde `/proc` en cada fase.
- [ ] 3. Hilos como LWPs: programa con `pthread_create` lanzando varios hilos que duermen/giran, visto con `ps -eLf` o `top -H` — mismo PID, varios TID.
- [ ] 4. Coste fork vs hilo: ping-pong de N mensajes entre dos procesos por una pipe vs entre dos hilos por mutex/condvar, cronometrado — cuantifica "crear/cambiar de hilo es más barato que de proceso".

### Animaciones
- [ ] 1. Diagrama de estados clicable: nuevo/listo/en ejecución/en espera/terminado; botones "interrupción de reloj", "E/S solicitada", "E/S completada" mueven el token entre estados.
- [ ] 2. Colas animadas: cola de trabajos → preparados → CPU → bloqueados, con tokens moviéndose y contador de cambios de contexto.
- [ ] 3. Proceso vs hilo, memoria: dos rectángulos (espacios separados) vs uno con varias pilas dentro, toggle monohilo/multihilo.
- [ ] 4. Inspector de PCB: tabla de campos del PCB que se "congela" en un cambio de contexto y los valores del siguiente proceso se cargan en los registros.

---

## Tema 04 — Planificación de procesos

### Ejecutables
- [ ] 1. RR en vivo con `nice`/`top`: varios procesos CPU-bound con distinta `nice`, ver el reparto real con `top -o %CPU`.
- [ ] 2. Mini-simulador RR con `SIGSTOP`/`SIGCONT` sobre 2-3 hijos + `alarm()`, imprimiendo el "cambio de contexto" en cada turno (teaser de la práctica P8).
- [ ] 3. Inanición + envejecimiento: varios jobs con `nice` normal + uno con `nice -n 19`, ver con `ps -o pid,ni,pri,stat,time` que este último apenas acumula CPU.
- [ ] 4. Coste del cuanto: dos versiones del mini-simulador RR con cuanto muy pequeño vs muy grande, contando cambios de contexto y tiempo total.

### Animaciones
- [ ] 1. Cola circular Round Robin con cuanto: temporizador que se llena y rota al proceso al final de la cola.
- [ ] 2. Planificador de prioridades con envejecimiento: la prioridad numérica baja mientras el proceso espera, hasta que le toca.
- [ ] 3. Slider de tamaño de cuanto sobre un Gantt animado: más o menos cambios de contexto según el valor.
- [ ] 4. Garantía de latencia máxima: cola de alta y baja prioridad, comparando un planificador ingenuo (inanición) con uno que impone un tope de espera.

---

## Tema 05 — Concurrencia y sincronización

### Ejecutables
- [ ] 1. Carrera bancaria real: dos procesos incrementando un contador en memoria compartida sin mutex (pierde actualizaciones) vs con semáforo (resultado correcto).
- [ ] 2. Filósofos comensales con 5 semáforos en 5 paneles tmux: versión ingenua (deadlock, todos se cuelgan) vs solución con mutex+estado.
- [ ] 3. Productor-consumidor con buffer circular en memoria compartida + 2 semáforos contadores (`lleno`/`vacío`), mostrando la ocupación en vivo.
- [ ] 4. Deadlock por orden de bloqueos: dos procesos con `P(semA);P(semB)` vs `P(semB);P(semA)` — ambos se cuelgan; con el mismo orden, se resuelve.
- [ ] 5. `watch ipcs -s`/`-m` en paralelo a cualquiera de las anteriores, para ver aparecer/desaparecer los recursos IPC.

### Animaciones
- [ ] 1. Race condition bancaria interactiva: interleaving manual de dos cajeros sobre un saldo compartido, con toggle "con mutex".
- [ ] 2. Filósofos comensales interactivo: deadlock vs solución, con avatares y tenedores.
- [ ] 3. Productor-consumidor: anillo de slots, semáforos `lleno`/`vacío` como contadores visibles.
- [ ] 4. Semáforo P/V como contador + cola de espera de procesos.
- [ ] 5. Constructor de grafo de asignación de recursos con detección de ciclo en rojo.
- [ ] 6. Algoritmo del banquero paso a paso (tabla max/asignado/necesita, buscando secuencia segura).

---

## Tema 06 — Gestión de memoria

### Ejecutables
- [ ] 1. COW en vivo: el ejemplo `memalloc.c` del README con `ps -a -ocomm,rssize` en otro panel mostrando el salto de RSS al escribir.
- [ ] 2. Fallo de página forzado: `mmap(PROT_NONE)` + `sigaction` capturando `SIGSEGV` e imprimiendo `si_addr`.
- [ ] 3. Simulador first/best/worst-fit como CLI interactiva (se van tecleando peticiones de alloc/free y se ve la tabla de particiones).
- [ ] 4. `pmap -x <pid>` en vivo mientras otro programa hace malloc/free en bucle.

### Animaciones
- [ ] 1. Asignación de particiones (first/best/worst-fit) con fragmentación externa visible.
- [ ] 2. Traducción de dirección paginada paso a paso (página+desplazamiento → marco).
- [ ] 3. Reproductor de la anomalía de Belady con slider de nº de marcos, usando la cadena de referencias ya del README.
- [ ] 4. Timeline de fallo de página completo (trampa → MMU → disco → reanudación).
- [ ] 5. Simulador de hiperpaginación (thrashing): slider de marcos asignados vs tamaño de working set.

---

## Tema 07 — IPC: pipes y FIFOs

### Ejecutables
- [ ] 1. Chat con `mkfifo` entre dos terminales, mostrando bloqueo real si el lector tarda en arrancar.
- [ ] 2. Atomicidad de `PIPE_BUF`: dos escritores concurrentes con chunks >4KB (se entrelazan/corrompen) vs ≤4KB (no).
- [ ] 3. SIGPIPE: escritor en bucle a una FIFO, lector que abre y cierra enseguida — ver el proceso morir, y repetir ignorando `SIGPIPE` para ver `EPIPE`.
- [ ] 4. Relay TCP con `nc` (el que está comentado en el README) revivido: `nc -l 8080 < fifo | nc destino 80 > fifo`.
- [ ] 5. Reimplementar a mano con `pipe()+fork()+dup2()+execlp()` el pipeline `ls -lh | grep '^d'`.

### Animaciones
- [ ] 1. Buffer de pipe animado (llenado/vaciado/bloqueo del escritor y del lector).
- [ ] 2. EOF vs SIGPIPE: toggle "cierro el lado lector" vs "cierro el lado escritor", ver qué camino se dispara.
- [ ] 3. Constructor de pipeline de shell: arrastrar `cat`/`grep`/`sort`/`uniq -c` y ver `fork`+`dup2` conectando cada etapa.
- [ ] 4. Comparador de dos FIFOs remotas con `diff` en vivo (dos streams entrando, diferencias resaltadas).

---

## Tema 08 — IPC: señales

### Ejecutables
- [ ] 1. `kill -STOP`/`kill -CONT` sobre un `sleep 100 &`, viendo el estado `T`/`S` con `ps`.
- [ ] 2. El ejemplo de `waitpid(WNOHANG|WUNTRACED)` del README, al lado de la versión con el bug (sin `WUNTRACED`) para comparar.
- [ ] 3. Ping-pong de señales entre dos procesos con `SIGUSR1`/`SIGUSR2`, contador subiendo en ambos paneles.
- [ ] 4. No acumulación: bloquear `SIGUSR1` 5s mientras otro panel manda `kill -USR1` tres veces seguidas; al desbloquear, el manejador solo se dispara una vez.

### Animaciones
- [ ] 1. Timeline de entrega de señal (3 orígenes: hardware/kernel/proceso), con salto al manejador y vuelta.
- [ ] 2. Visualizador de máscara bloqueada/pendiente: grid de señales; mandar una bloqueada la marca "pendiente" en vez de dispararse.
- [ ] 3. Máquina de estados de un hijo (ejecutando→parado→continuado→terminado) y qué `SIGCHLD`+flags recibe el padre en cada transición.
- [ ] 4. Cadena Ctrl+C → SIGINT → terminal → proceso en primer plano, con/sin manejador instalado.

---

## Tema 09 — IPC: colas de mensajes

### Ejecutables
- [ ] 1. Productor/consumidor con `msgget/msgsnd/msgrcv` y `watch ipcs -q` mostrando la cola llenarse/vaciarse.
- [ ] 2. Retirada selectiva: enviar mensajes con varios `mtype` mezclados, luego `msgrcv` pidiendo un tipo concreto de en medio de la cola.
- [ ] 3. Persistencia: lanzar el productor, matarlo tras unos mensajes, y arrancar el consumidor después — sigue recibiendo lo que quedó en la cola.
- [ ] 4. 5 productores en 5 paneles con distinto periodo + `watch ipcs -q` viendo la profundidad de la cola.

### Animaciones
- [ ] 1. Cola de prioridad animada: mensajes de colores cayendo y ordenándose por prioridad antes de salir.
- [ ] 2. Timeline productor/consumidor desacoplado (actividad de cada uno independiente, mensajes persistiendo en medio).
- [ ] 3. Retirada selectiva: cola con mensajes de varios tipos, sacar uno concreto de en medio sin tocar los demás.

---

## Tema 10 — Memoria compartida y mutex

### Ejecutables
- [ ] 1. La carrera exacta del README (10→11 se pierde) sin mutex vs con mutex (10→11→12 correcto), con `shmget`+`semop`.
- [ ] 2. Bloqueo real: un proceso mantiene el semáforo 10s, otro intenta tomarlo y se queda esperando (visible con `ps -o stat`).
- [ ] 3. Direcciones distintas, mismo segmento: dos procesos hacen `shmat()` e imprimen su puntero — distinto valor, mismos bytes.
- [ ] 4. Persistencia tras `kill -9`: crear memoria compartida, matar el proceso creador, ver con `ipcs -m` que el segmento sigue vivo hasta `ipcrm`.

### Animaciones
- [ ] 1. Animación directa del "sin mutex pierde el 11 / con mutex llega a 12" (toggle mutex on/off).
- [ ] 2. Mutex como llave con cola de espera (la línea de tiempo estática del README hecha simulación jugable).
- [ ] 3. Semáforo contador con N slots, viendo cuándo se llena y empiezan a esperar los procesos.
- [ ] 4. Dos espacios de direcciones apuntando al mismo bloque físico compartido (arrastrar/zoom).

---

## Tema 11 — Dispositivos de E/S

### Ejecutables
- [ ] 1. `dd` con distinto `bs` (512/4K/1M) midiendo MB/s.
- [ ] 2. Sondeo vs bloqueante: dos programas leyendo de una FIFO, uno con `O_NONBLOCK` en bucle (gasta CPU), otro bloqueante — medir `%CPU` con `time`.
- [ ] 3. `strace -c` sobre escritura byte a byte sin buffer vs con `fwrite`/stdio, viendo el colapso del nº de syscalls.
- [ ] 4. Simulador en C de FCFS/SSTF/SCAN/LOOK sobre la cola exacta del README (`76 124 17 269 201 29 137 12`), verificando los recorridos totales.
- [ ] 5. `ls -l /dev/sda`, `/dev/null`, `/dev/tty` para ver mayor/menor y tipo bloque/carácter.

### Animaciones
- [ ] 1. Carrera sondeo vs interrupción (dos carriles: uno gasta ciclos comprobando, el otro atiende a otra cosa hasta el aviso).
- [ ] 2. Visualizador de planificación de disco: cola editable, cabezal moviéndose para FCFS/SSTF/SCAN/LOOK/C-SCAN/C-LOOK en paralelo.
- [ ] 3. Doble buffer animado (llenar uno mientras se vacía el otro).
- [ ] 4. Explorador de geometría de disco con sliders de RPM/velocidad de búsqueda.
- [ ] 5. Caché de disco LRU: bloques entrando/saliendo, hit/miss resaltado.

---

## Tema 12 — Sistemas de ficheros

### Ejecutables
- [ ] 1. Reproducir en vivo la resolución `/usr/ast/mbox` del README con ficheros reales: `ls -i`, `stat` en cada nivel.
- [ ] 2. Hard link vs symlink: `ln`/`ln -s`, mismo inodo vs distinto, y qué pasa al borrar el original.
- [ ] 3. Fichero disperso: `truncate -s 10G fichero`, comparar `du -h` (bloques reales) vs `ls -lh` (tamaño lógico).
- [ ] 4. Simulador en C de asignación de bloques (contigua/enlazada/indexada) sobre un "disco" como array, viendo fragmentación.
- [ ] 5. `mount` / `cat /proc/mounts` para ver puntos de montaje reales.

### Animaciones
- [ ] 1. Stepper de la resolución `/usr/ast/mbox` (directorio→inodo→bloques), clic para avanzar.
- [ ] 2. Explorador de inodo con punteros directos + indirecto simple/doble/triple, slider de tamaño de fichero.
- [ ] 3. Sandbox de asignación de bloques (contigua/enlazada/indexada) con fragmentación apareciendo con el uso.
- [ ] 4. Visualizador de RAID (0/1/5/6/10) con fallo de disco simulado.
- [ ] 5. Journaling: transacción escrita al log, "crash" a mitad, replay al arrancar.
- [ ] 6. Comparador de políticas de caché de escritura (write-through/write-back/delayed/write-on-close) en carriles paralelos.

---

## Tema 13 — GUI

### Ejecutables
- [ ] 1. `echo $XDG_SESSION_TYPE` / `loginctl show-session ... -p Type` para ver X11 vs Wayland real.
- [ ] 2. `xwininfo`/`xprop` sobre una ventana real (solo X11/XWayland).
- [ ] 3. `wmctrl -l` / `wmctrl -d` para listar ventanas y escritorios.
- [ ] 4. `evtest` sobre `/dev/input/eventX` (necesita root) viendo eventos de ratón en crudo — primer salto de "el viaje de un clic".

### Animaciones
- [ ] 1. Pipeline animado de "el viaje de un clic" (ratón→driver→compositor→aplicación→pantalla), clic a clic o automático.
- [ ] 2. Sandbox de composición: 3 ventanas semitransparentes arrastrables con Z-order, panel de "framebuffer final" combinado.
- [ ] 3. Toggle X11↔Wayland que reconfigura el grafo de arquitectura y re-enruta el evento animado.
- [ ] 4. Timeline histórica del Xerox Alto con puntos calientes (pantalla de bits/ratón/teclado) enlazando a conceptos modernos.
