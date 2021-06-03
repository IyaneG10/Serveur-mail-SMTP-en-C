#
# Makefile pour generer la bibliotheque de fonctions de calculs
#

SOURCES = $(wildcard *.c)
OBJETS = $(SOURCES:.c=.o)
BIBLIOTHEQUE = libcalcul.a
AR = ar r
CFLAGS += -g -Wall


all: $(BIBLIOTHEQUE)

$(BIBLIOTHEQUE): $(OBJETS)
	$(AR) $@ $^

$(OBJETS): libcalcul.h

clean:
	rm -rf *.o *.a core
