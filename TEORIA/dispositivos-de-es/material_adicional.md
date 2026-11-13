# Dispositivos de Entrada/Salida: material adicional

## Modelo de tiempos: sondeo frente a interrupciones

| Componente | Significado |
|-----------|-------------|
| `tiempo_cómputo` | tiempo invertido en el cómputo |
| `tiempo_dispositivo` | tiempo invertido en las operaciones de E/S |
| `tiempo_sobrecarga` | tiempo que el proceso invierte en determinar cuándo se completa cada operación de E/S |
| `tiempo_sondeo` | tiempo que el proceso invierte en determinar la finalización de la E/S **con sondeo** |
| `tiempo_manejador` | tiempo acumulado en ejecutar el manejador de interrupciones y las rutinas de manejo del dispositivo |
| `tiempo_listo` | tiempo que el proceso acumula esperando para usar la CPU al concluir la E/S |

```text
tiempo_total = tiempo_cómputo + tiempo_dispositivo + tiempo_sobrecarga

E/S con sondeo:          tiempo_sobrecarga = tiempo_sondeo
E/S con interrupciones:  tiempo_sobrecarga = tiempo_manejador + tiempo_listo
```

Con dispositivos lentos respecto al coste de una interrupción, `tiempo_sondeo > tiempo_manejador + tiempo_listo` y las interrupciones salen a cuenta. Con dispositivos muy rápidos (SSD NVMe, red de alta velocidad) puede ocurrir lo contrario, y por eso se vuelve a sondear.

## Discos híbridos (SSHD)

Un **SSHD** es un disco duro con algo de memoria flash para los datos de acceso más frecuente; sus prestaciones dependen de la cantidad de flash.

## Geometría del disco

- Un disco magnético tiene varios **platos**, cada uno con una o dos **superficies** de almacenamiento.
- Cada superficie se divide en **pistas** (anillos concéntricos) y en **sectores** (porciones angulares del círculo). La intersección de una pista con un sector es el **sector de pista**, la unidad mínima que se lee o escribe.
- Hay un cabezal por superficie; todos van unidos al **mismo brazo** y se mueven a la vez (un único motor). Las pistas que quedan bajo los cabezales en una posición del brazo forman un **cilindro**: se accede a ellas **sin mover el brazo**.
- Para leer o escribir, el disco debe estar **girando**, el brazo debe **llevar el cabezal a la pista** y hay que **esperar a que pase el sector**.
- La capacidad del disco es superficies × pistas × sectores por pista × bytes por sector.

<img src="img/geometria-disco.svg" width="420" alt="Superficie de un disco dividida en pistas concéntricas y sectores angulares">

Los discos actuales meten más sectores en las pistas exteriores, que son más largas (grabación por zonas), así que la geometría real ya no coincide con este modelo. El controlador la oculta y presenta el disco como una lista lineal de bloques numerados (LBA, *Logical Block Addressing*).

## Varias colas en NVMe

NVMe admite miles de colas de peticiones. Linux crea una por núcleo, así cada núcleo envía sus peticiones sin competir con los demás por un cerrojo. Son las líneas `nvme0q1`…`nvme0q4` de `/proc/interrupts`, cada una atendida por una CPU.
