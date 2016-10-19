#include "sender.h"

const char* file_name;
const char* host_name;
int port; //soit le numéro du port sur lequel envoyer, soit -1 pour envoyer sur la même machine)

int main(int argc, const char* argv[])
{	
	int i;
	for(i = 1;i<argc;i++)
	{
		if(strcmp("-f",argv[i])==0)
		{
			if(i < argc-1)
			{
				i++;
				file_name = argv[i];
			}
			else 
			{
				fprintf(stderr, "N'oubliez pas de donner l'emplacement du fichier !\n");
				return 1;
			}
		}
		else if (strcmp("::1",argv[i])==0)
		{
			port = -1;
		}
		else
		{
			if(atoi(argv[i])!=0) port = atoi(argv[i]);
			else
			{
				host_name = argv[i]; 
			}
		}
	}
	if (port==0)
	{
		fprintf(stderr,"Numéro du port inexistant ou invalide !\n");
		return 1;
	}
	if (host_name==NULL)
	{
		fprintf(stderr,"Nom du host inexistant ou invalide !\n");
		return 1;
	}
}
