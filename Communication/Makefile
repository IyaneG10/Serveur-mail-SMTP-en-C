#
# Makefile pour generer la bibliotheque de fonctions de communication
#

SOURCES = $(wildcard *.c)
OBJETS = $(SOURCES:.c=.o)
BIBLIOTHEQUE = libcom.a
AR = ar r
CFLAGS += -g -Wall -lresolv


all: $(BIBLIOTHEQUE)

$(BIBLIOTHEQUE): $(OBJETS)
	$(AR) $@ $^

$(OBJETS): libcom.h 

clean:
	rm -rf *.o *.a core
