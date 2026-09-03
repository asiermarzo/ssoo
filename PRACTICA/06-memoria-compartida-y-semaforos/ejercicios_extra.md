# P6 — Ejercicios extra

Cinco ejercicios adicionales sobre memoria compartida y semáforos **System V IPC** (`shmget`, `shmat`, `shmdt`, `shmctl`, `semget`, `semctl`, `semop`). Cada solución es un `.c` autocontenido. Compilar todos con `make`. Recuerda liberar los recursos (`ipcs`, `ipcrm`).

En estos ejercicios la clave se pasa como un número entero y se usa directamente como `key_t` (sin `ftok`). El tipo `union semun` se define en el propio programa, como indica `man semctl`.

## 1. Crear memoria compartida (`extra_01.c`)

Programa `extra_01 clave n_enteros` que cree un segmento de memoria compartida de `n_enteros` enteros con esa clave, lo inicialice a 0 y permanezca en ejecución hasta recibir `Ctrl-C`, momento en el que libera el segmento (`shmdt` + `shmctl(IPC_RMID)`) y termina.

## 2. Leer y escribir con exclusión mutua (`extra_02.c`)

Programa que acceda al segmento creado por `extra_01` y lea o escriba una posición, protegiendo el acceso con un semáforo binario:

```
./extra_02 <clave> escribir <pos> <valor>
./extra_02 <clave> leer <pos>
```

## 3. Condición de carrera (`extra_03.c`)

Programa que lance 4 procesos que incrementan 100 000 veces un contador en memoria compartida. Sin argumentos no usa semáforo (el resultado final es incorrecto); con el argumento `sem` protege el incremento con un semáforo y el resultado es exacto. Ilustra la necesidad de la sección crítica.

```
./extra_03          # sin proteccion  -> contador < 400000
./extra_03 sem      # con semaforo     -> contador = 400000
```

## 4. Productor / consumidor con búfer circular (`extra_04.c`)

Programa que implemente un búfer circular de 8 huecos en memoria compartida, sincronizado con tres semáforos (mutex, huecos libres, huecos ocupados). Rol por argumento:

```
./extra_04 <clave> productor <n>
./extra_04 <clave> consumidor <n>
./extra_04 <clave> limpiar
```

## 5. Control de aforo con semáforo N-ario (`extra_05.c`)

Programa que simule una sala con aforo `N`: lanza `C` procesos "cliente" que intentan entrar; el semáforo, inicializado a `N`, solo deja dentro a `N` simultáneamente. Cada cliente permanece un tiempo aleatorio y sale.

```
./extra_05 3 8      # aforo 3, 8 clientes
```
