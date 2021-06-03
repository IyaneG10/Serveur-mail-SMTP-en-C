/** fichier serveur.h **/
#include "libcom.h"

/***************************************************/
/* Structures necessaires pour la station meteo    */
/***************************************************/



/** Constantes **/
#define NB_SUFFIXES 5
#define TAILLE 100



/** Variables globales  **/

char maildir[TAILLE] ="/var/spool/mail/"; //valeur par défaut
char suffixe[NB_SUFFIXES][TAILLE];


/** Fonctions  **/

char* check_arguments(int argc, char **argv);

void *fnct_sock_locale(void* arg);

void *fnct_sock_internet(void *machine);
