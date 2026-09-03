# Makefile general del repositorio
#
#   make         compila los ejemplos de todas las practicas
#                (y de teoria en cuanto tengan codigo)
#   make clean   borra los binarios de todas ellas
#   make list    muestra las carpetas que se recorren
#
# No compila nada por si mismo: delega en el Makefile de cada carpeta.

SUBDIRS := $(sort $(dir $(wildcard PRACTICA/*/Makefile) $(wildcard TEORIA/*/Makefile)))

all:
	@for d in $(SUBDIRS); do \
	    echo "==> $$d"; \
	    $(MAKE) --no-print-directory -C $$d all || exit 1; \
	done

clean:
	@for d in $(SUBDIRS); do \
	    echo "==> $$d (clean)"; \
	    $(MAKE) --no-print-directory -C $$d clean || exit 1; \
	done

list:
	@printf '%s\n' $(SUBDIRS)

.PHONY: all clean list
