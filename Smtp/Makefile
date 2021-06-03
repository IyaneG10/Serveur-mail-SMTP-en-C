#
# Makefile pour generer la bibliotheque de fonctions de liées au SMTP
#

SOURCES = $(wildcard *.c)
OBJETS = $(SOURCES:.c=.o)
BIBLIOTHEQUE = libsmtp.a
AR = ar r

LOCAL_LIBS  =    -L ../Communication  -L  ../Threads -L ../Calcul
LOCAL_LIBS +=  -lcom   -lthrd -lcalcul -lresolv
CFLAGS     +=  -g -Wall   -I ../Communication    -I ../Threads  -I ../Calcul 



all: $(BIBLIOTHEQUE)

$(BIBLIOTHEQUE): $(OBJETS)
	$(AR) $@ $^

$(OBJETS): libsmtp.h

clean:
	rm -rf *.o *.a core
