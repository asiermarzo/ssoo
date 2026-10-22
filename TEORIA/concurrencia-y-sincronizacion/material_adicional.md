# Sincronización de procesos: material adicional

## Formas de garantizar la concurrencia

- **Multiprogramación**: varios procesos en un sistema monoprocesador.
- **Multiprocesamiento**: varios procesos en un sistema multiprocesador.
- **Procesamiento distribuido**: varios procesos en sistemas de computadores múltiples y distribuidos (ejemplo: *clusters*).

La concurrencia comprende: comunicación entre procesos, compartición y competencia por los recursos, sincronización de la ejecución de varios procesos y asignación del tiempo de procesador.

## Contextos en que aparece la concurrencia

- **Múltiples aplicaciones**: compartición dinámica del procesador entre varias aplicaciones.
- **Aplicaciones estructuradas**: una aplicación se diseña como un conjunto de procesos concurrentes.
- **Estructura del sistema operativo**: el propio SO se implementa como un conjunto de procesos o hilos.

## Tareas del SO referentes a la concurrencia

Seguimiento de los procesos activos; asignación y liberación de recursos (tiempo de procesador, memoria, archivos, dispositivos de E/S); protección de datos y recursos de cada proceso frente a injerencias de otros; garantía de la correcta ejecución de un proceso independientemente de la velocidad de los demás.

## Definición operacional de los semáforos

Con un contador con signo `e(s)` y una fila de espera `f(s)`:

```text
acquire(s)                       release(s)
  e(s) = e(s) - 1;                 e(s) = e(s) + 1;
  si e(s) < 0 entonces             si e(s) <= 0 entonces
      estado(p) = bloqueado;           sale(q, f(s));
      entra(p, f(s));                  estado(q) = elegible;
                                       entra(q, f(elegibles));
```

Elementos: un entero `e(s)`, una fila de espera `f(s)` y las dos operaciones `acquire(s)` y `release(s)`.

## Filósofos comensales: solución sin interbloqueo

Estado por filósofo + `mutex` + un semáforo `s[i]` por filósofo:

```text
Filósofo i:
    Pensar();
    tomar_cubierto(i);
    Comer();
    dejar_cubierto(i);

tomar_cubierto(i):
    acquire(mutex);
    estado[i] = HAMBRE;
    test(i);
    release(mutex);
    acquire(s[i]);

dejar_cubierto(i):
    acquire(mutex);
    estado[i] = PIENSA;
    test(IZQUIERDA);
    test(DERECHA);
    release(mutex);

test(i):
    si (estado[i] == HAMBRE && estado[IZQUIERDA] != COMER && estado[DERECHA] != COMER) entonces
        estado[i] = COMER;
        release(s[i]);
```

## Productor-consumidor: variantes

El productor y el consumidor son dos procesos **cíclicos**:

```text
Productor                        Consumidor
    ...                              ...
    producir(mensajeP);              retirar(casilla, mensajeC);
    depositar(casilla, mensajeP);    consumir(mensajeC);
    ...                              ...
```

Problemas: depositar un mensaje cuando el consumidor no ha retirado el anterior; retirar un mensaje cuando el productor no ha depositado nada.

**Solución para una sola casilla** (semáforos `lleno` y `vacío` inicializados a 0 y 1):

```text
Productor                        Consumidor
    producir(mensajeP);              acquire(lleno);
    acquire(vacío);                  retirar(casilla, mensajeC);
    depositar(casilla, mensajeP);    release(vacío);
    release(lleno);                  consumir(mensajeC);
```

## Monitores

- El concepto de **monitor** fue definido por **Hoare (1974)**. Java emplea monitores para garantizar la exclusión mutua.
- Un monitor es una herramienta evolucionada de sincronización: un **tipo abstracto de datos** pensado para usarse con seguridad por varios procesos/hilos. Sus procedimientos se ejecutan en **exclusión mutua**: en cada instante solo un proceso/hilo ejecuta alguno de ellos.
- Con semáforos, las llamadas a las funciones necesarias quedan repartidas por el código, lo que dificulta corregir errores y asegurar el buen funcionamiento; los monitores evitan estos inconvenientes.

Un monitor está formado por: **variables de estado**, **procedimientos internos**, **procedimientos externos** (puntos de entrada), **condiciones** y **primitivas de sincronización**. Las variables de estado solo son accesibles desde los procedimientos del monitor (**encapsulación**).

**Funcionamiento**: un proceso entra en el monitor invocando uno de sus procedimientos externos; si ya hay otro dentro, queda suspendido hasta que quede libre (salvo que el de dentro ejecute la primitiva `esperar`, que lo saca del monitor).

**Monitor `incr_dism`** (cada `procedure` se ejecuta en exclusión mutua):

```text
incr_dism : monitor;
    var i : entero;

    procedure incrementa;
    inicio
        i := i + 1;
    fin;

    procedure disminuye;
    inicio
        i := i - 1;
    fin;

inicio
    i := 0;
fin incr_dism.
```

**Monitor de reunión (*rendez‑vous*)** — cada proceso que llega espera hasta que han llegado los `N`:

```text
Reunión : monitor;
    var n : entero;
        llegaron_todos : condición;

    procedure llegar;
    inicio
        n := n + 1;
        si n < N entonces llegaron_todos.esperar;
        llegaron_todos.marcar;
    fin;

inicio
    n := 0;
fin reunión.
```

**Lectores‑escritores con monitor:**

```text
Lector_escritor : monitor;
    var escr : booleano;
        nl   : entero;
        c_escr, c_lect : condición;

    procedure empezar_lectura;
    inicio
        si escr entonces c_lect.esperar;
        nl := nl + 1;
        c_lect.marcar;
    fin;

    procedure fin_lectura;
    inicio
        nl := nl - 1;
        si nl = 0 entonces c_escr.marcar;
    fin;

    procedure empezar_escritura;
    inicio
        si escr o nl > 0 entonces c_escr.esperar;
        escr := true;
    fin;

    procedure fin_escritura;
    inicio
        escr := false;
        si nl > 0 entonces c_lect.marcar;
        si no c_escr.marcar;
    fin;

inicio
    nl := 0; escr := false;
fin lector_escritor.
```

## Evitación del interbloqueo

### Modelo de sistema

- Conjunto de procesos `P₁, P₂, …, Pₙ`.
- Conjunto de recursos (físicos o lógicos) `R₁, R₂, …, Rₘ`; de cada recurso puede haber **una o más instancias**.
- Consumo de recursos por parte de los procesos: **petición** (si no está disponible, el proceso queda suspendido hasta que lo esté) → **uso** → **liberación**.

### Estado seguro

Los bloqueos mutuos pueden evitarse si se conoce cierta información sobre los procesos antes de asignar recursos. Para cada petición, el sistema comprueba si, satisfaciéndola, entraría en un **estado inseguro** (donde puede producirse un bloqueo); solo satisface la petición si asegura quedar en un **estado seguro**. Se necesita conocer el número y tipo de todos los recursos existentes, disponibles y requeridos.

**Algoritmos para evitar interbloqueos**: algoritmo del banquero (Dijkstra) · algoritmo del grafo de asignación de recursos · algoritmo de seguridad · algoritmo de solicitud de recursos.

### Algoritmo del banquero

Analogía con un banco: los **clientes** son los procesos (con un crédito límite), el **dinero** son los recursos y el **banquero** es el SO. El banco confía en que no todos los clientes usarán todo su crédito a la vez, y asume que si un cliente maximiza su crédito podrá terminar sus negocios y devolver el dinero, permitiendo servir a otros.

- Un sistema está en **estado seguro** si existe una **secuencia segura**: una sucesión de procesos `<P₁, …, Pₙ>` tal que, para cada `Pᵢ`, su petición de recursos puede satisfacerse con los recursos disponibles más los que están usando los `Pⱼ` con `j < i` (que liberarán al terminar).
- Si no hay suficientes recursos para `Pᵢ`, este espera hasta que algún `Pⱼ` termine y libere sus recursos; entonces `Pᵢ` toma los necesarios, los usa y termina, y así sucesivamente. Si no existe tal secuencia, el sistema está en **estado inseguro**.
- **Restricciones**: se debe conocer a priori la demanda máxima de recursos; los procesos deben ser independientes (ejecutables en cualquier orden, sin sincronización forzada); debe haber un número fijo y conocido de recursos y de procesos; los procesos no pueden finalizar mientras retengan recursos.
