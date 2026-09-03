# P1 — ejercicios extra
# make        compila extra_01 .. extra_05
# make clean  borra los binarios

CC     = gcc
CFLAGS = -Wall -Wextra -O2

SRCS = $(wildcard extra_*.c)
BINS = $(SRCS:.c=)

all: $(BINS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

clean:
	rm -f $(BINS)

.PHONY: all clean
