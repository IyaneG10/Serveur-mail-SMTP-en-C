/** fichier libsmtp.h **/
#ifndef H_LIBSMTP
#define H_LIBSMTP
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
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <dirent.h>
#include <resolv.h>


#include "libcalcul.h"
#include "libthrd.h"
#include "libcom.h"



/******************************************************************/
/** Ce fichier decrit les structures et les constantes utilisees **/
/** par les fonctions SMTP                   **/
/******************************************************************/

/** Constantes **/
#define MAX_LIGNE 2048
#define N 4096
#define PORTNUM "25"
#define NB_SUFFIXES 5
#define TAMPON_MESSAGE "temp_message.txt"
#define END_MESSAGE ".\r\n"
#define SERVER_GREETING "220 localhost SMTP server \r\n"
#define UNKNOWN_CMD "502 Not implemented command\r\n"
#define QUIT_CMD "221 2.0.0 \r\n"
#define STATUS_TO_FWD "BEING TRANSFERED TO HOST MACHINE"
#define STATUS_FWD "TRANSFERED TO HOST MACHINE"
#define STATUS_SAVED "SENT"
#define RSP_END_DATA "250 2.0.0 Ok\r\n"
#define RSP_DATA "354 End data with <CR><LF>,<CR><LF>\r\n"
#define USER_NOT_LOCAL "251  User not local\r\n"
#define RSP_MAIL "250 2.1.0 Ok\r\n"
#define RSP_NOOP "220 Ok NOOP\r\n"
#define CMD_DIGIT 4




/**** Variables globales *****/
extern char maildir[100]; // chemin du fichier de stockage des messages (option de main)
extern char suffixe[NB_SUFFIXES][100]; //  suffixes des adresses locales (option de main)





/**** Fonctions ****/

struct Information
{
	int relay_or_not;
	int debut_message;
	char from [1024];
	char subject[1024];
	char to [1024];
	char mailServer[1024];
	FILE *fd_status;
	FILE *fd_message;
	FILE *fd_tampon;
	int fd_mailbox;
	char racine[100]; // chemin du fichier de stockage des messages (aprés concaténation)
	char racine2[100] ; // chemin du fichier .status (aprés concaténation)
	int socket_typ;
};


void* gestionClient(int socket [2]);

int multi_gestionClient(int socket [2] );


struct Information relayMail(struct Information Infos, char* filename, char* portNum) ;

struct Information commandeHELO(char *buffer, int s, struct Information Infos);

struct Information commandeMAIL(char *buffer, int s, struct Information Infos);

struct Information commandeRCPT(char *buffer, int s,struct Information Infos);

struct Information  commandeDATA(char *buffer,  int s, struct Information Infos);

struct Information commande_traiter_DATA(char *buffer,  int s,struct Information Infos);


struct Information commande_fin_DATA(char *buffer,  int s,struct Information Infos);

void commandeQUIT(char *buffer,  int s);

void commandeNOOP(char *buffer, int s);

void commandeRSET(char *buffer, int s);

struct Information redirection_Commandes(char *buffer, int s,struct Information Infos);

































#endif
