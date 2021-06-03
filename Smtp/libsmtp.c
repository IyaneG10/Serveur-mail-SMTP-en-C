/** fichier libsmtp **/

/*****************************************************************/
/** Ce fichier contient des fonctions utilisees par le serveur  **/
/**                           SMTP                              **/
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
#include <pthread.h>

#include "libsmtp.h"

#include "libcom.h"
#include "libcalcul.h"
#include "libthrd.h"




/**** Fonctions de gestion des sockets ****/



struct Information relayMail(struct Information Infos, char* filename, char* portNum) 
{
	printf("\n----------------------------------------------------");
	printf("\n                    SERVEUR RELAIS                  \n");
	printf("----------------------------------------------------\n");
	FILE *f;
	if ((f = connServDist(Infos.mailServer, portNum))) {
		int cmd; // pour respecter les étapes EHLO, MAIL, RCPT, DATA, QUIT
		for(cmd = 1; cmd < 7; cmd++) 
		{
			char bufferin[1024];
			fgets(bufferin, sizeof(bufferin), f);
			puts(bufferin);
			if(cmd == 1) {
				fprintf(f, "HELO [127.0.0.1]\r\n");
			}
			else if(cmd == 2 && (strncmp(bufferin,"250",3)==0 ) ) {
				fprintf(f,"MAIL FROM: <%s>\r\n", Infos.from);
			}
			else if (cmd == 3 &&  (strncmp(bufferin,"250",3)==0 )){
				fprintf(f,"RCPT TO: <%s>\r\n", Infos.to);
			}
			else if (cmd == 4 &&  (strncmp(bufferin,"250",3)==0 )){
				fprintf(f,"DATA\r\n");
			}
			else if (cmd == 5 &&  (strncmp(bufferin,"354",3)==0 )){
				char* message="";
				FILE * messageFile = fopen(filename, "r");
				fseek(messageFile, 0, SEEK_END);
				long length = ftell (messageFile);
				fseek(messageFile, 0, SEEK_SET);
				message = malloc(length);
				fread (message, 1, length, messageFile);
				fclose(messageFile);
				fprintf(f,"%s", Infos.subject);
				fprintf(f,"From: <%s>\r\n", Infos.from);
				fprintf(f,"To: %s\r\n\r\n",  Infos.to);
				fprintf(f,"%s\r\n\r\n.\r\n",  message);
				free(message);
			}
			else if ( (cmd == 6 &&  (strncmp(bufferin,"250",3)==0 ) )     ||     ( strncmp(bufferin,"5",1)==0 ) )    {
				fprintf(f,"QUIT\r\n");
			}
		}
		fclose(f);
	}
	return Infos;
}




struct Information commandeHELO(char *buffer, int s, struct Information Infos)
{
	char buffer_sortie[1024];
	sprintf(buffer_sortie, "250 Ok\r\n");
	send(s, buffer_sortie, strlen(buffer_sortie), 0);
	printf("message du serveur:  %s", buffer_sortie);
	sprintf(Infos.racine, "%s",maildir);
	mkdir(Infos.racine,0777);
	return Infos;
}

struct Information commandeMAIL(char *buffer, int s, struct Information Infos)
{

	char buffer_sortie[1024];
	char addr_expediteur[256+11];
	char user[64+5];
	char domain[64+5];
	char *acc_ouvr,*acc_ferm,*dom=domain;
	int portee;

	memset(addr_expediteur,0,231);
	acc_ouvr=strchr(buffer,'<');
	acc_ferm=strchr(buffer,'>');
	acc_ouvr++;
	portee=acc_ferm-acc_ouvr;
	strncpy(addr_expediteur,acc_ouvr,portee);
	dom=strchr(addr_expediteur,'@');
	dom=dom+1;
	memset(user,0,sizeof(user));
	strncpy(user,addr_expediteur,strlen(addr_expediteur)-strlen(dom)-1);

	sprintf(Infos.from, "%s@%s",user,suffixe[0]);// choisir le premier suffixe pour les adresses locales (pour l'instant)
	printf("LE SUFFIXE PAR DEFAUT EST: %s\n", suffixe[0]);
	sprintf(buffer_sortie, RSP_MAIL);
	send(s, buffer_sortie, strlen(buffer_sortie), 0);
	printf("message du serveur:  %s", buffer_sortie);

	return Infos;
}

struct Information commandeRCPT(char *buffer, int s,struct Information Infos)
{
	char buffer_sortie[1024];
	char addr[256+11];
	char user[64+5];
	char domain[64+5];
	char *acc_ouvr,*acc_ferm,*dom=domain;
	int portee;
	int rcpt_locale=0;// pour savoir si le destinataire est local
	memset(addr,0,231);
	acc_ouvr=strchr(buffer,'<');
	acc_ferm=strchr(buffer,'>');
	acc_ouvr++;
	portee=acc_ferm-acc_ouvr;
	strncpy(addr,acc_ouvr,portee);
	dom=strchr(addr,'@');
	dom=dom+1;
	memset(user,0,sizeof(user));
	strncpy(user,addr,strlen(addr));


	int nb_files=0;
	char destinataire[100];
	sprintf(destinataire, "%s", addr);
	char nb_messages[100];
	sprintf(nb_messages,"%i", nb_files );
	char sl[2]="/";
	sprintf(sl, "/");
	char extension[10];
	sprintf(extension, ".txt");

	strcat(user, sl);
	strcat(Infos.racine,user);
	mkdir(Infos.racine,0777);

	int a= count_files(Infos.racine);
	sprintf(nb_messages,"%i", (a/2)+1 );
	strcat(nb_messages,extension);
	strcat(Infos.racine,nb_messages);
	Infos.fd_message = fopen (Infos.racine,"a+");
	strncpy(Infos.racine2,Infos.racine,strlen(Infos.racine)-3);
	strcat(Infos.racine2,"status");
	Infos.fd_status = fopen (Infos.racine2,"w");

	fclose(Infos.fd_status);

	sprintf(Infos.to, "%s",addr);
	sprintf(Infos.mailServer, "%s",dom);

	
	for(int i=0;i<NB_SUFFIXES;i++)
	{		
		if(strcmp(dom,(const char *)suffixe[i])==0) 
		{
			rcpt_locale=1; // le destinataire est bien local
		}
	}

	if(rcpt_locale==1) // verifier s'il s'agit d'un destinataire en interne suffixe (suffixe locale passée en paramètre)
	{
		sprintf(buffer_sortie, "250 2.1.5 Ok\r\n");
		send(s, buffer_sortie, strlen(buffer_sortie), 0);
		printf("message du serveur:  %s", buffer_sortie);
	}

	else if (Infos.socket_typ==0)
	{
		Infos.relay_or_not=1; // message à transmettre au serveur distant
		sprintf(buffer_sortie, USER_NOT_LOCAL);
		send(s, buffer_sortie, strlen(buffer_sortie), 0);
		printf("message du serveur:  %s", buffer_sortie);
	}
	else
	{
		sprintf(buffer_sortie, "250 Ok\r\n");
		send(s, buffer_sortie, strlen(buffer_sortie), 0);
		printf("message du serveur:  %s", buffer_sortie);		
		printf("SOCKET LIEE A INTERNET DONC SEULS MESSAGES EN INTERNE AUTORISES\n");
	}



		



	return Infos;
}

struct Information  commandeDATA(char *buffer,  int s, struct Information Infos)
{
	char buffer_sortie[1024];	
	Infos.fd_tampon = fopen (TAMPON_MESSAGE,"w+");                                
	sprintf(buffer_sortie, RSP_DATA);
	send(s, buffer_sortie, strlen(buffer_sortie), 0);
	printf("message du serveur:  %s", buffer_sortie);
	return Infos;
}

struct Information commande_traiter_DATA(char *buffer,  int s,struct Information Infos)
{
	if(Infos.debut_message==1 )
	{
		fprintf (Infos.fd_tampon,"%s",buffer);
	}
	if(strncmp(buffer,"Subject: ",9)==0)
	{
		sprintf(Infos.subject, "%s",buffer);
	}
	if(strncmp(buffer,"\r\n",2)==0 )
	{
		fprintf (Infos.fd_message,"\r\n");
		if(Infos.debut_message==0)
		{
			Infos.debut_message=1;
		}
	}
	else
	{
		fprintf (Infos.fd_message,"%s",buffer);
	}
	return Infos;
}

struct Information commande_fin_DATA(char *buffer,  int s,struct Information Infos)
{
	char buffer_sortie[1024];
	fclose(Infos.fd_tampon);	  
	sprintf(buffer_sortie,RSP_END_DATA);
	send(s, buffer_sortie, strlen(buffer_sortie), 0);
	printf("message du serveur:  %s", buffer_sortie);
	if( (Infos.relay_or_not == 1) && (Infos.socket_typ==0) )
	{
		Infos.fd_status = fopen (Infos.racine2,"w");
		fprintf (Infos.fd_status,STATUS_TO_FWD);
		fclose(Infos.fd_status);
		Infos=relayMail(Infos, TAMPON_MESSAGE, PORTNUM);
		Infos.fd_status = fopen (Infos.racine2,"w");
		fprintf (Infos.fd_status,STATUS_FWD);
		fclose(Infos.fd_status);
	}

	else if (Infos.socket_typ==1)

	{	
		Infos.fd_status = fopen (Infos.racine2,"w");
		fprintf (Infos.fd_status,STATUS_SAVED);
		fclose(Infos.fd_status);
		close(s);
	}
	fclose(Infos.fd_message);
	return Infos;
}

void commandeQUIT(char *buffer,  int s)
{
	char buffer_sortie[1024];
	sprintf(buffer_sortie, QUIT_CMD);
	send(s, buffer_sortie, strlen(buffer_sortie), 0);
	printf("message du serveur:  %s", buffer_sortie);
	close(s);
}


void commandeNOOP(char *buffer, int s)
{
	char buffer_sortie[1024];
	sprintf(buffer_sortie, RSP_NOOP);
	send(s, buffer_sortie, strlen(buffer_sortie), 0);
	printf("message du serveur:  %s", buffer_sortie);
}

void commandeRSET(char *buffer, int s)
{
	char buffer_sortie[1024];
	sprintf(buffer_sortie, "250 Ok\r\n");
	send(s, buffer_sortie, strlen(buffer_sortie), 0);
	printf("message du serveur:  %s", buffer_sortie);

}



struct Information redirection_Commandes(char *buffer, int s,struct Information Infos)
{
	char buffer_sortie[1024];
	if(strncmp(buffer,"EHLO",CMD_DIGIT)==0 || strncmp(buffer,"HELO",CMD_DIGIT)==0)
		Infos=commandeHELO(buffer,s,Infos);
	else if(strncmp(buffer,"MAIL",CMD_DIGIT)==0)
		Infos=commandeMAIL(buffer,s,Infos);
	else if(strncmp(buffer,"RCPT",CMD_DIGIT)==0)
	{
		Infos=commandeRCPT(buffer,s,Infos);
	}
	else if(strncmp(buffer,"RSET",CMD_DIGIT)==0)
		commandeNOOP(buffer,s);
	else if(strncmp(buffer,"QUIT",CMD_DIGIT)==0)
		commandeQUIT(buffer,s);
	else 
	{
		sprintf(buffer_sortie, UNKNOWN_CMD);
		send(s, buffer_sortie, strlen(buffer_sortie), 0);
		printf("message du serveur:  %s", buffer_sortie);
	};
	return Infos;
}



void* gestionClient(int socket [2] )
{
	struct Information Infos = {0,0,"","","","",NULL,NULL,NULL,0,"","",0};
	memset (Infos.racine, 0, sizeof (Infos.racine));
	memset (Infos.racine2, 0, sizeof (Infos.racine2));
	int s= socket[0];
	Infos.socket_typ=socket[1];// pour différencier les deux types de sockets
	mkdir(maildir,0777);
	if ((Infos.fd_mailbox = open(maildir, O_DIRECTORY | O_RDONLY)) ==-1)
	{
		printf("error %s\n", strerror(errno));
	}

	// lockf à revoir car n'accepte pas les file descriptors de répertoires
	//if ((lockf(Infos.fd_mailbox, F_LOCK, 0)) == -1) 
	//{
	//perror("lockf()");
	//exit(EXIT_FAILURE);
	//} 

	/* Obtient une structure de fichier */
	FILE *dialogue=fdopen(s,"a+"); 
	if(dialogue==NULL){ perror("gestionClient.fdopen"); exit(EXIT_FAILURE); }

	int dans_message=0;
	char buffer[1024], buffer_sortie[1024];
	sprintf(buffer_sortie, SERVER_GREETING);
	printf("%s", buffer_sortie);
	send(s, buffer_sortie, strlen(buffer_sortie), 0);
	while(fgets(buffer,MAX_LIGNE,dialogue)!=NULL)
	{
		printf("message du client: %s", buffer);
		if(dans_message==0)
		{
			if(strncmp(buffer,"DATA",4)==0)
			{
				Infos=commandeDATA(buffer,s,Infos);
				dans_message=1;
			}
			else
				Infos=redirection_Commandes(buffer,s,Infos);
		}
		if(dans_message==1)
		{
			if(strncmp(buffer,END_MESSAGE,3)==0)
			{
				Infos=commande_fin_DATA(buffer,s, Infos);
				dans_message=0;
				Infos.debut_message=0;
			}
			else
			{
				Infos=commande_traiter_DATA(buffer,s,Infos);
			}
		}
	}
	/* Termine la connexion */
	fclose(dialogue);
	printf("client déconnecté\n");
return NULL;
}






int multi_gestionClient(int socket[2] ) // socket[0]: socket en question; socket[1]: variable pour différencier les deux types de sockets
{
	lanceThread(gestionClient,( int *) socket,0);
	return 0;
}








