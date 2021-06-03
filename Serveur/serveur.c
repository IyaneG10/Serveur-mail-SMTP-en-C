
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <fcntl.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <resolv.h> 
#include <getopt.h>
#include <pthread.h>
#include <stdbool.h>

#include "serveur.h"

#include "libsmtp.h"
#include "libcom.h"
#include "libthrd.h"





char* check_arguments(int argc, char **argv)
{
	int c;
	int  num_suffix=0;
	char * arg_oblig;

	while (1)
	{
		static struct option long_options[] =
		{
			{"Mail Directory",  required_argument, 0, 'R'},
			{"Suffix",    required_argument, 0, 's'},
			{0, 0, 0, 0}
		};
		int option_index = 0;
		c = getopt_long (argc, argv, "R:s:",
				long_options, &option_index);

		if (c == -1)
			break;
		switch (c)
		{
			case 0:
				if (long_options[option_index].flag != 0)
					break;
				printf ("option %s", long_options[option_index].name);
				if (optarg)
					printf (" with arg %s", optarg);
				printf ("\n");
				break;
			case 'R':
				{
					printf ("option -R with value '%s'\n", optarg);
					strncpy(maildir,optarg,strlen(maildir));
				}
				break;
			case 's':
				{
					printf ("option -s with value '%s'\n", optarg);
					//suffixe[TAILLE][num_suffix]='\0';
					strncpy(suffixe[num_suffix],optarg,50);
					printf("SUFFIXE %d: %s\n", num_suffix,suffixe[num_suffix]);
					num_suffix++;
				}
				break;
			case '?':
			
				break;
			default:
				abort ();
		}
	}
	if (optind < argc)
	{
		printf ("non-option ARGV-elements: ");
		while (optind < argc)
{
arg_oblig=argv[optind++];			
printf ("%s ", arg_oblig);
//return arg_oblig;
}
		putchar ('\n');


	}
	return arg_oblig;
	exit (0);

}



void *fnct_sock_locale(void* arg)
{
	int socket_locale=0;
	/* Initialisation du serveur */	
	int s_locale=initialisationServeur("localhost",socket_locale);
	/* Lancement de la boucle d'ecoute */
	boucleServeur(s_locale,multi_gestionClient,socket_locale);
	/* On termine la connexion */
	shutdown(s_locale,SHUT_RDWR);
	
	pthread_exit(NULL);
}



void *fnct_sock_internet(void* machine)
{
	int socket_locale=1;
	/* Initialisation du serveur */	
	int s_internet=initialisationServeur(machine,socket_locale);
	/* Lancement de la boucle d'ecoute */
	boucleServeur(s_internet,multi_gestionClient,socket_locale);
	/* On termine la connexion */
	shutdown(s_internet,SHUT_RDWR);
}





int main(int argc,char *argv[])
{	
    strcpy(suffixe[0],"myserv.fr");// suffixe par défaut
    char* machine_sock_internet= check_arguments( argc, argv);
    pthread_t tid_loc ,tid_int;

    pthread_create(&tid_loc, NULL, fnct_sock_locale, NULL);
    pthread_create(&tid_int, NULL, fnct_sock_internet, (char*) machine_sock_internet);

    pthread_join(tid_loc, NULL);
    pthread_join(tid_int, NULL);

    return 0;
}
