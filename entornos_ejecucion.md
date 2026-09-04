# Entornos de ejecución

Las prácticas se desarrollan sobre **Linux**. Estas son las diferentes opciones que tienes:

| Entorno | Requisitos |
|---------|-----------|
| Ordenadores UPNA en local | Estar físicamente en un aula/laboratorio |
| VDI UPNA | Navegador |
| Máquinas virtuales UPNA | Navegador |
| GitHub Codespaces | Cuenta de GitHub + navegador |
| Ordenador propio · Linux nativo | Instalación local |
| Ordenador propio · WSL2 (Windows) | Windows 10 2004+ o Windows 11 |
| Ordenador propio · Docker (Windows o Mac) | Docker Desktop |

---

## Ordenadores UPNA en local

Los PC de las aulas pueden arrancan con la imagen de Linux, que ya trae `gcc`, `make`, `gdb`, `strace`, `tmux` y el resto de herramientas del curso.

Elije Xubuntu cuando está arrancando. Tus archivos desaparecerán al reiniciar el ordenador por lo que deberás copiarlos a un usb, drive o git.

## Cualquier ordenador con internet

### VDI UPNA

Escritorio virtual con Linux:

- Acceso: [https://vdibroker.unavarra.es/](https://vdibroker.unavarra.es/), introducir las credenciales de la UPNA y elegir el escritorio **Xubuntu 2024**.

Los archivos desaparecerán cuando desconectes. Puedes copiar y pegar archivos entre la máquina virtual y tu ordenador.

### Máquinas virtuales de la UPNA

Imagen de disco descargable para ejecutar el Linux del laboratorio en local con **VirtualBox** o **VMware**, sin depender de la conexión.

- Descarga de la imagen: *(añadir enlace del portal de la UPNA / Campus Virtual)*.
- Recursos sugeridos: 2 CPU, 4 GB de RAM, 20 GB de disco.
- Instala las *Guest Additions* para portapapeles compartido y carpeta compartida con el host.
- El home es **local** a la VM: haz copias de tu trabajo (git, carpeta compartida, USB).

### GitHub Codespaces

Contenedor Linux que se ejecuta en la nube **servidores de GitHub** (Microsoft Azure). Cada codespace es una máquina virtual dedicada (2 vCPU / 8 GB RAM en el plan gratuito).

El repo trae un **dev container** (`.devcontainer/`) que específica que la máquina arranca con `gcc`, `gdb`, `strace`, `ltrace`, `tmux` y `valgrind` listos.

#### Requisitos

- **Cuenta de GitHub** y sesión iniciada ([github.com/signup](https://github.com/signup)). Sin estar logeado no aparece la pestaña *Codespaces*.
- El plan gratuito incluye 120 núcleo-hora y 15 GB de almacenamiento al mes (suficiente para el curso).
- Con **GitHub Education** (verificando que eres estudiante de la UPNA) el límite sube y tienes otras ventajas: [education.github.com](https://education.github.com/) → *Get benefits* / [GitHub Student Developer Pack](https://education.github.com/pack).

#### Crear el codespace

1. Inicia sesión en GitHub. En la página del repositorio: botón verde **Code → pestaña Codespaces → Create codespace on main**.
2. La primera vez tarda 1–2 min en construir la imagen (las siguientes arrancan en segundos).
3. Cuando termine, comprueba que está todo:

   ```bash
   gcc --version && gdb --version | head -1 && strace --version && tmux -V && valgrind --version
   ```

#### Programar desde el navegador

Al crear el codespace, GitHub **abre solo** una pestaña nueva con VS Code para la Web (URL `https://<nombre>.github.dev`) ya conectado a la máquina.

Para volver más tarde: [github.com/codespaces](https://github.com/codespaces) → clic en el codespace (o **Code → Codespaces** en la página del repo → clic en el que aparece en la lista).

```bash
cd PRACTICA/00-shell-y-herramientas
gcc -Wall -Wextra -o hola hola.c
./hola
```

También se puede programar desde tu visual code (con el plugin github codespaces) o por terminal, pero no lo cubriremos en la asigantura.

#### Detener y borrar

- La cuota gratuita es limitada. **Detén el codespace al terminar**: en <https://github.com/codespaces>, `···` → **Stop codespace** (o `gh codespace stop`).
- Un codespace parado sigue ocupando almacenamiento: si no lo vas a usar ya más puedes borrarlo con `···` → **Delete** o `gh codespace delete`.

## Ordenador propio

### Linux (dual-boot o instalación nativa)

Cualquier distribución reciente sirve, pero se **recomienda [Ubuntu LTS](https://ubuntu.com/download/desktop)** (o Xubuntu, la misma base con un escritorio más ligero): es la que usan las aulas y la VDI.

#### Instalación más sencilla: USB de arranque

1. **Descarga la ISO** de Ubuntu LTS desde [ubuntu.com/download/desktop](https://ubuntu.com/download/desktop) (~5 GB).
2. **Graba la ISO en un USB** de al menos 8 GB con una herramienta gráfica:
   - Windows/Mac/Linux: [balenaEtcher](https://etcher.balena.io/) — abrir, elegir ISO, elegir USB, *Flash*.
   - Solo Windows: [Rufus](https://rufus.ie/).
3. **Arranca desde el USB**: reinicia y pulsa la tecla del menú de arranque (suele ser `F12`, `F9`, `Esc` o `Supr` según el fabricante); elige el USB.
4. En el menú de Ubuntu puedes elegir **"Probar Ubuntu"** (sesión *live*, sin tocar el disco, útil para comprobar que el hardware funciona) o **"Instalar Ubuntu"**.
5. Durante la instalación, la opción **"Instalar junto a Windows"** crea el *dual-boot* automáticamente (redimensiona la partición de Windows y añade un menú para elegir sistema al arrancar). Si vas a dedicar el equipo entero a Linux, elige "Borrar disco e instalar Ubuntu".

> Antes de un dual-boot: haz copia de tus datos y, en portátiles con Windows, desactiva *Fast Startup* y *BitLocker*. El *Secure Boot* puede quedarse activado, Ubuntu lo soporta.

#### Instalar las herramientas del curso

```bash
# Debian / Ubuntu
sudo apt-get update
sudo apt-get install -y build-essential gdb strace ltrace tmux valgrind git
```

### Windows → WSL2

WSL2 ejecuta un kernel Linux real dentro de una máquina windows; las llamadas al sistema (`fork`, `pipe`, señales, memoria compartida, colas de mensajes…) funcionan como en Linux nativo.

En una terminal como administrador (PowerShell o Símbolo del sistema; requiere Windows 10 2004+ o Windows 11):

```
wsl --install -d Ubuntu
```

- Reinicia, crea el usuario de Linux y dentro de la shell de Ubuntu instala `build-essential gdb strace ltrace tmux` como arriba.
- Trabaja **dentro del sistema de ficheros de Linux** (`~/…`, no `/mnt/c/…`): es mucho más rápido y evita problemas de permisos.
- Editor: VS Code con la extensión *WSL* abre el proyecto directamente en el entorno Linux.

### Mac / Windows → Docker

Contenedor Linux, útil si WSL2 no está disponible (Mac con Apple Silicon, Windows sin virtualización, etc.).

**Requisito previo:** instala [Docker Desktop](https://www.docker.com/products/docker-desktop/) (Windows o Mac) y ábrelo una vez para que el servicio quede en marcha. Comprueba con `docker run hello-world`. En Windows, Docker Desktop usa WSL2 por debajo, así que necesita virtualización activada.

No hace falta descargar la imagen a mano: la primera vez que ejecutes `docker run … debian:stable`, Docker la baja de Docker Hub automáticamente (~120 MB) y la deja en caché para las siguientes.

```bash
docker run -it --rm -v "$PWD":/ssoo -w /ssoo debian:stable bash
# dentro del contenedor:
apt-get update && apt-get install -y build-essential gdb strace tmux
```

- `-v "$PWD":/ssoo` monta el repositorio dentro del contenedor; los cambios se guardan en el host.
- En macOS con Apple Silicon algunas herramientas de trazado (`strace`) funcionan mejor forzando arquitectura: `docker run --platform linux/amd64 …` (más lento).
- Para `gdb` dentro del contenedor puede hacer falta `--cap-add=SYS_PTRACE --security-opt seccomp=unconfined`.