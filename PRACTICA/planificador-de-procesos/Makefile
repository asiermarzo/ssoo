# Cada .c de la carpeta es un programa: make los compila todos (procsched.c y encolador.c incluidos)
CFLAGS = -Wall -O2
LDLIBS = -lX11 -lm
PROGRAMAS = $(basename $(wildcard *.c))

all: $(PROGRAMAS)

clean:
	rm -f $(PROGRAMAS) *.ppm
