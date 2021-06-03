#
# Makefile pour generer le serveur 
#

CC=gcc
OBJS = serveur.o
LIBS = -lpthread
LOCAL_LIBS  =    -L ../Communication   -L ../Smtp -L  ../Threads -L ../Calcul
LOCAL_LIBS +=  -lcom   -lsmtp -lthrd -lcalcul
CFLAGS     +=    -I ../Communication  -I ../Smtp  -I ../Threads  -I ../Calcul


$(OBJETS): serveur.h 

#
# Cible generale
#

all: serveur

#
# La cible de nettoyage
#

clean: 
	rm -f core *.o 

#
# Les cibles pour l'executable
#

serveur: $(OBJS)
	$(CC) $(CFLAGS) -g -o serveur $(OBJS) $(LOCAL_LIBS) $(LIBS) -lresolv

serveur.o: serveur.c serveur.h

DEBUG: $(OBJS)
	$(CC) $(CFLAGS) -g -o serveur $(OBJS) $(LOCAL_LIBS) $(LIBS) -DDEBUG
