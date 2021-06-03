/** fichier libcom.c **/

/*****************************************************************/
/** Ce fichier contient des fonctions utilisees par le serveur  **/
/** SMTP   et concernant les sockets.                           **/
/*****************************************************************/

/**** Fichiers d'inclusion ****/

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
#include "libcom.h"


/**** Variables globales *****/
struct sockaddr_in adresse;
socklen_t taille=sizeof adresse;
char *service="4444"; // port de mon serveur

extern char mailServer[1024];



/** Constantes **/
#define MAX_LIGNE 2048
#define N 4096
#define MAX_CONNEXIONS 8 

/**** Fonctions de gestion des sockets ****/




int initialisationServeur(char *node, int socket_typ){
	struct addrinfo precisions,*resultat,*origine; 
	int statut; 
	int s; 

	/* Construction de la structure adresse */
	memset(&precisions,0,sizeof precisions);
	precisions.ai_family=AF_UNSPEC; // aucune précision, donc prend aussi en compte les clients IPv4 et IPv6
	precisions.ai_socktype=SOCK_STREAM; // Type de socket: TCP (mode flux, connecté) => SOCK_STREAM 
	precisions.ai_flags=AI_PASSIVE; // On veut un serveur (ne fait qu'écouter) => AI_PASSIVE (AI_ACTIVE pour client)
	statut=getaddrinfo(node,service,&precisions,&origine);
	if(statut<0){ perror("initialisationServeur.getaddrinfo"); exit(EXIT_FAILURE); }
	struct addrinfo *p;
	for(p=origine,resultat=origine;p!=NULL;p=p->ai_next)
	{
		//if(p->ai_family==AF_INET6){ resultat=p; break; } // chercher des adresses jusqu'à IP v6 
	}  
	/* Creation d'une socket */
	s=socket(resultat->ai_family,resultat->ai_socktype,resultat->ai_protocol);
	if(s<0){ perror("initialisationServeur.socket"); exit(EXIT_FAILURE); }

	if (socket_typ==0)
		printf("Socket locale crée avec succes\n"); 
	if (socket_typ==1)
		printf("Socket internet crée avec succes\n");

	/* Options utiles */
	int vrai=1;
	if(setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&vrai,sizeof(vrai))<0){
		perror("initialisationServeur.setsockopt (REUSEADDR)");
		exit(EXIT_FAILURE);
	}
	if(setsockopt(s,IPPROTO_TCP,TCP_NODELAY,&vrai,sizeof(vrai))<0){
		perror("initialisationServeur.setsockopt (NODELAY)");
		exit(EXIT_FAILURE);
	}

	/* Specification de l'adresse de la socket */
	statut=bind(s,resultat->ai_addr,resultat->ai_addrlen); 
	if(statut<0) return -1;
	
	if (socket_typ==0)
		printf("Socket locale \"bindé\" avec succes\n");  
	if (socket_typ==1)
		printf("Socket internet \"bindé\" avec succes\n"); 


	/* Liberation de la structure d'informations */
	freeaddrinfo(origine); 

	/* Taille de la queue d'attente */
	statut=listen(s,MAX_CONNEXIONS); 

	if (socket_typ==0)
			printf("Socket locale en attente de connexion avec les clients\n");  
	if (socket_typ==1)
			printf("Socket internet en attente de connexion avec les clients\n");
	if(statut<0) return -1;

	return s;
}

int boucleServeur(int s,int (*multi_gestionClient)(int[2]),int socket_typ)
{
	int dialogue; 
	while(1){
		/* Attente d'une connexion */
		if((dialogue=accept(s,(struct sockaddr *)&adresse,&taille))<0) return -1;
	if (socket_typ==0)
			printf("client connecté à la socket locale\n");  
	if (socket_typ==1)
			printf("client connecté à la socket internet\n");

		int socket[2] = {dialogue, socket_typ};
		/* Passage de la socket de dialogue a la fonction de traitement */
		if(multi_gestionClient((int*)socket)<0){ shutdown(socket[0],SHUT_RDWR); return 0;}
	}
	return s;
}




FILE *connServDist(char *mailServer, const char *port) {
	FILE *f = NULL;
	int s;
	struct addrinfo precisions_adresse,*resultat_adresse,*origine_adresse;
	memset(&precisions_adresse,0,sizeof precisions_adresse);
	precisions_adresse.ai_family=AF_INET;
	precisions_adresse.ai_socktype=SOCK_STREAM;
	precisions_adresse.ai_flags = 0;
	precisions_adresse.ai_protocol = 0; 
	char MX[254];
	strcpy((char *)MX,resolv_addr(mailServer));

	s=getaddrinfo(MX,port,&precisions_adresse,&origine_adresse); // utilisation de res_query dans resolv_addr
	if (s != 0) {
		fprintf(stderr, "getaddrinfo(): failed: %s.\n", gai_strerror(s));
	}

	for (resultat_adresse = origine_adresse; resultat_adresse != NULL; resultat_adresse = resultat_adresse->ai_next) {
		if ((s = socket(AF_INET, SOCK_STREAM, 0))== -1) {
			perror("socket() failed: ");
			continue; 
		}

		if (connect(s, resultat_adresse->ai_addr, resultat_adresse->ai_addrlen) != -1) {
			break; 
		}
		else {
			perror("connect(): ");
		}
		close(s); 
	}

	freeaddrinfo(origine_adresse);

	if (resultat_adresse == NULL) {
		fprintf(stderr, "Could not connect.\n");
	}
	else {

		if (!(f = fdopen(s, "r+"))) {
			perror("fdopen() failed: ");
		}
	}

	return f;
}

char *resolv_addr(char *domaine) 
{
	char * adresse;    
	u_char nsbuf[N];
	char dispbuf[N];
	ns_msg msg;
	ns_rr rr;
	int i, l;

	l = res_query(domaine, ns_s_an, ns_t_mx, nsbuf, sizeof(nsbuf));
	if (l < 0)
	{
		perror(domaine);
	}
	else
	{
		ns_initparse(nsbuf, l, &msg);
		l = ns_msg_count(msg, ns_s_an);
		for (i = 0; i < l; i++)
		{
			ns_parserr(&msg, ns_s_an, i, &rr);
			ns_sprintrr(&msg, &rr, NULL, NULL, dispbuf, sizeof(dispbuf));
		}
		// pour récupérer le premier MX (plus tard: essayer par ordre de priorité)
		adresse = strtok (dispbuf," ");
		adresse = strtok (NULL," ");
		adresse = strtok (NULL," ");
		adresse = strtok (NULL," ");
	}        
	return adresse;
}
