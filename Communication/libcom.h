/** fichier libcom.h **/
#ifndef H_LIBCOM
#define H_LIBCOM
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <dirent.h>
#include <resolv.h>
#include <stdbool.h>



/******************************************************************/
/** Ce fichier decrit les structures et les constantes utilisees **/
/** par les fonctions de gestion des sockets                     **/
/******************************************************************/




/**** Fonctions ****/


int initialisationServeur(char *node, int socket_typ);

int boucleServeur(int s,int (*traitement)(int[2]),int socket_typ);  


char *resolv_addr(char *domaine) ;

FILE *connServDist(char *mailServer, const char *port) ;







#endif
