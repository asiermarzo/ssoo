# Imagen base de dev containers (Ubuntu 24.04) con git, curl, sudo y usuario "vscode" ya configurados.
FROM mcr.microsoft.com/devcontainers/base:ubuntu-24.04

# Herramientas del curso preinstaladas: nadie tiene que hacer apt-get install.
RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
        build-essential \
        gcc \
        gdb \
        make \
        strace \
        ltrace \
        tmux \
        valgrind \
        man-db \
        manpages-dev \
        manpages-posix \
        manpages-posix-dev \
    && rm -rf /var/lib/apt/lists/*
