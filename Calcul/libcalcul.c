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
#include <string.h>
#include <dirent.h>


int count_files(char *repertoire) 
{
	int file_count = 0;
	DIR * dir;
	struct dirent * entry;

	dir = opendir(repertoire); 
	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_type == DT_REG) { 
			file_count++;
		}
	}
	closedir(dir);
	return file_count;
}

int find_exped(char *fileName,char *strSearch) 
{
	FILE *fd;
	char c;
	int found=0;
	int pos=0;
	int length;
	fd = fopen(fileName,"r");
	if( fd == NULL )
		return 0;
	length=strlen(strSearch);
	while( fread(&c,1,1,fd)>0 && !found ) {
		if( c==strSearch[pos] ) {
			pos++;
		} else {
			if(pos!=0) {
				fseek(fd,-pos,SEEK_CUR);
				pos=0;
			}
		}
		found = (pos==length);
	}
	fclose(fd);
	return found;
}

