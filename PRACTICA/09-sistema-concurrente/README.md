# P9 — Sistema concurrente

Construcción de un pequeño sistema concurrente. **Entrega evaluable** (curso 2025/26: 12/12/2025).

## Descripción general

Construir un **simulador de un sistema hidráulico** mediante procesos, mecanismos de comunicación entre procesos y llamadas al sistema, integrando todo lo aprendido en las prácticas anteriores.

Restricciones:

- **No** se permiten esperas activas en ningún caso.
- Los programas **no** pueden usar variables globales.
- Seis programas en ficheros independientes que interactúan entre sí.
- Se valoran especialmente la concisión, claridad, simplicidad y eficiencia del código.

## Estructuras de datos

```c
typedef struct {
    long tipo;
    int  pid;
    char texto[100];
} mensaje_t;

typedef struct {
    int contador, caudal;
} fluido_t;
```

## Componentes

El simulador representa el flujo de fluido a lo largo de un sistema hidráulico. Línea continua = flujo de fluido por tuberías (`pipe`); línea discontinua = alertas, señales y gestión de recursos.

<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 920 560" font-family="sans-serif" font-size="11" role="img" aria-label="Arquitectura del simulador hidraulico: flujo de fluido y mecanismos de control">
  <defs>
    <marker id="arrowP9" markerWidth="8" markerHeight="8" refX="4" refY="4" orient="auto">
      <path d="M0,0 L8,4 L0,8 z" fill="#333"/>
    </marker>
  </defs>
  <text x="10" y="16" font-size="10" fill="#777">linea gruesa = flujo de fluido (pipe) · linea discontinua = control (semaforos, mem. compartida, cola de alertas, señales)</text>
  <rect x="20" y="30" width="150" height="55" fill="#f2f2f2" stroke="#333"/>
  <text x="95" y="53" text-anchor="middle">gestor</text>
  <text x="95" y="69" text-anchor="middle" font-size="9">crea / destruye toda la infraestructura</text>
  <rect x="560" y="30" width="170" height="55" fill="#eef2ff" stroke="#333"/>
  <text x="645" y="53" text-anchor="middle">cola de mensajes</text>
  <text x="645" y="69" text-anchor="middle" font-size="9">alertas (tipo 1 y tipo 2)</text>
  <rect x="770" y="20" width="130" height="180" fill="none" stroke="#999" stroke-dasharray="3,3"/>
  <text x="835" y="14" text-anchor="middle" font-size="10" fill="#777">monitor</text>
  <rect x="780" y="35" width="110" height="65" fill="#e8f3ff" stroke="#333"/>
  <text x="835" y="60" text-anchor="middle">padre</text>
  <text x="835" y="76" text-anchor="middle" font-size="9">imprime capacidad</text>
  <rect x="780" y="115" width="110" height="65" fill="#e8f3ff" stroke="#333"/>
  <text x="835" y="140" text-anchor="middle">hijo</text>
  <text x="835" y="156" text-anchor="middle" font-size="9">imprime alertas</text>
  <rect x="20" y="235" width="120" height="55" fill="#f2f2f2" stroke="#333"/>
  <text x="80" y="266" text-anchor="middle">llena_deposito</text>
  <rect x="190" y="235" width="130" height="55" fill="#fdeef2" stroke="#333"/>
  <text x="255" y="258" text-anchor="middle">deposito proveedor</text>
  <text x="255" y="274" text-anchor="middle" font-size="9">· semaforo 1 ·</text>
  <rect x="360" y="235" width="110" height="55" fill="#e3f5e1" stroke="#333"/>
  <text x="415" y="267" text-anchor="middle">surtidor</text>
  <rect x="520" y="235" width="110" height="55" fill="#fdf3d0" stroke="#333"/>
  <text x="575" y="267" text-anchor="middle">caudalimetro</text>
  <rect x="680" y="235" width="110" height="55" fill="#fbe0e0" stroke="#333"/>
  <text x="735" y="267" text-anchor="middle">sumidero</text>
  <rect x="430" y="410" width="200" height="60" fill="#fdeef2" stroke="#333"/>
  <text x="530" y="434" text-anchor="middle">capacidad surtidor</text>
  <text x="530" y="450" text-anchor="middle" font-size="9">memoria compartida</text>
  <text x="530" y="463" text-anchor="middle" font-size="9">· semaforo 2 ·</text>
  <!-- flujo principal: fluido -->
  <path d="M140,262 L188,262" stroke="#333" stroke-dasharray="4,3" fill="none" marker-end="url(#arrowP9)"/>
  <text x="150" y="255" font-size="9" fill="#555">sem1 (+vol)</text>
  <path d="M320,262 L358,262" stroke="#333" stroke-dasharray="4,3" fill="none" marker-end="url(#arrowP9)"/>
  <text x="322" y="255" font-size="9" fill="#555">sem1 (-vol)</text>
  <path d="M470,262 L518,262" stroke="#333" stroke-width="3" fill="none" marker-end="url(#arrowP9)"/>
  <text x="475" y="252" font-size="9" fill="#555">pipe · fluido_t</text>
  <path d="M630,262 L678,262" stroke="#333" stroke-width="3" fill="none" marker-end="url(#arrowP9)"/>
  <text x="635" y="252" font-size="9" fill="#555">pipe · fluido_t</text>
  <!-- gestor management -->
  <path d="M95,85 L95,230" stroke="#999" stroke-dasharray="2,3" fill="none" marker-end="url(#arrowP9)"/>
  <path d="M170,60 C400,60 500,60 640,50" stroke="#999" stroke-dasharray="2,3" fill="none" marker-end="url(#arrowP9)"/>
  <path d="M120,85 C200,200 350,380 428,438" stroke="#999" stroke-dasharray="2,3" fill="none" marker-end="url(#arrowP9)"/>
  <text x="130" y="150" font-size="9" fill="#888">crea / destruye</text>
  <!-- surtidor / sumidero <-> capacidad (sem2) -->
  <path d="M400,290 L500,408" stroke="#333" stroke-dasharray="4,3" fill="none" marker-end="url(#arrowP9)"/>
  <text x="395" y="350" font-size="9" fill="#555">sem2 (+)</text>
  <path d="M720,290 L590,408" stroke="#333" stroke-dasharray="4,3" fill="none" marker-end="url(#arrowP9)"/>
  <text x="660" y="350" font-size="9" fill="#555">sem2 (-)</text>
  <!-- alertas -->
  <path d="M440,235 C480,150 540,90 558,63" stroke="#333" stroke-dasharray="4,3" fill="none" marker-end="url(#arrowP9)"/>
  <text x="450" y="150" font-size="9" fill="#555">alerta tipo 1</text>
  <path d="M590,235 C595,170 610,110 615,63" stroke="#333" stroke-dasharray="4,3" fill="none" marker-end="url(#arrowP9)"/>
  <text x="600" y="150" font-size="9" fill="#555">alerta tipo 2</text>
  <path d="M730,55 L778,140" stroke="#333" fill="none" marker-end="url(#arrowP9)"/>
  <text x="700" y="100" font-size="9" fill="#555">prioridad: tipo1 &gt; tipo2</text>
  <!-- SIGUSR1 y lectura de capacidad -->
  <path d="M780,255 C820,200 835,130 835,102" stroke="#333" stroke-dasharray="4,3" fill="none" marker-end="url(#arrowP9)"/>
  <text x="790" y="180" font-size="9" fill="#555">SIGUSR1</text>
  <path d="M630,435 C700,420 760,400 778,80" stroke="#333" stroke-dasharray="4,3" fill="none" marker-end="url(#arrowP9)"/>
  <text x="655" y="420" font-size="9" fill="#555">lee capacidad</text>
</svg>

### `gestor clave periodo volumen umbral`

Crea y destruye toda la infraestructura. Crea: una **cola de mensajes** para las alertas, una **memoria compartida** con la capacidad del surtidor (un entero de litros, inicializado a 0) y un **grupo semafórico con dos semáforos** (semáforo 1: volumen disponible en el depósito del proveedor; semáforo 2: exclusión mutua para la memoria compartida). Crea los procesos `surtidor` (args `clave periodo volumen`), `caudalimetro` (`clave umbral`), `monitor` (`clave`) y `sumidero` (`clave pid_monitor`), interconectando con **dos tuberías** las E/S estándar `surtidor → caudalimetro → sumidero`. Luego espera (sin espera activa) la señal `SIGINT` (`Ctrl-C`); al recibirla elimina ordenadamente la cola de mensajes, la memoria compartida, el grupo semafórico y los cuatro procesos.

### `llena_deposito clave tiempo volumen`

Espera `tiempo` segundos, accede al semáforo 1 (nivel del depósito del proveedor) y lo incrementa en `volumen` litros. Luego termina.

### `surtidor clave periodo volumen`

Cada `periodo` segundos: espera; si es posible, descuenta `volumen` litros del semáforo 1 (depósito del proveedor); escribe en su salida estándar un `fluido_t` con el caudal traspasado y el número de descarga (`contador`, que se incrementa con cada traspaso). A continuación bloquea el semáforo 2, incrementa en `volumen` la memoria compartida (capacidad del surtidor) y libera el semáforo 2. Si no puede suministrar el volumen indicado, envía a la cola de alertas un `mensaje_t` con `tipo = 1`, su `pid` y el texto `"Problema de suministro en el surtidor PID, caudal insuficiente"`.

### `caudalimetro clave umbral`

Lee continuamente de la entrada estándar `fluido_t`, espera un segundo, imprime por pantalla el caudal que circula, reenvía el `fluido_t` por su salida estándar y comprueba si el caudal supera `umbral`. Si lo supera, encola en la cola de alertas un `mensaje_t` con `tipo = 2` y el texto `"Problema de caudal excesivo en PID"`.

### `sumidero clave pid_monitor`

Lee continuamente de la entrada estándar `fluido_t`. Por cada lectura: espera dos segundos, bloquea el semáforo 2, decrementa la memoria compartida (capacidad del surtidor) en el caudal del `fluido_t`, libera el semáforo 2, envía `SIGUSR1` al proceso `pid_monitor` y continúa esperando nuevos mensajes.

### `monitor clave`

Realiza dos tareas en paralelo (crea un proceso hijo):

- **Padre**: espera continuamente `SIGUSR1`; al recibirla bloquea el semáforo 2, imprime `"Presentes XXX litros en el surtidor"`, libera el semáforo 2 y vuelve a esperar.
- **Hijo**: lee e imprime continuamente los mensajes de la cola de alertas atendiendo a su prioridad (los de `tipo 1` son más prioritarios que los de `tipo 2`).

## Llamadas al sistema útiles

De prácticas anteriores: `fork`, `execvp`, `wait` ([P2](../02-procesos-e-hilos/)); `pipe`, `dup2` ([P3](../03-pipes-y-fifos/)); `kill`, `sigaction`, `pause` ([P4](../04-senales/)); `shmget`/`shmat`/`shmdt`/`shmctl`, `semget`/`semctl`/`semop` ([P6](../06-memoria-compartida-y-semaforos/)); `msgget`/`msgsnd`/`msgrcv`/`msgctl` ([P7](../07-colas-de-mensajes/)).

## Entrega

El comprimido debe incluir los seis programas, el `Makefile` que compila todos ellos, y todos los `.c` y `.h` necesarios. `make` sin argumentos debe crear los seis ejecutables. Para la corrección se borran los ejecutables, se hace `touch` a los fuentes y se recompila con el `Makefile`.

## Consulta

- Teoría: [`TEORIA/05`](../../TEORIA/05-concurrencia-y-sincronizacion/) y temas de IPC [`TEORIA/07`](../../TEORIA/07-ipc-pipes-y-fifos/)–[`TEORIA/10`](../../TEORIA/10-memoria-compartida-y-mutex/).
