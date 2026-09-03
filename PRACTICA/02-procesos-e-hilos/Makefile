# P2 — ejercicios extra
# make        compila extra_01 .. extra_05
# make clean  borra los binarios

CC     = gcc
CFLAGS = -Wall -Wextra -O2

SRCS = $(wildcard extra_*.c)
BINS = $(SRCS:.c=)

all: $(BINS)

# el ejercicio 5 usa hilos POSIX
extra_05: CFLAGS += -pthread

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

clean:
	rm -f $(BINS)

.PHONY: all clean
