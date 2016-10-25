#include <err.h>
#include "reciever.h"

const char* file_name;
char* host_name;
int port; //soit le numéro du port sur lequel écouter, soit -1 pour écouter sur toutes les interfaces
int fd_out; //file descriptor, 0 si STDOUT, ou un nouveau file descriptor si un fichier a été spécifié

void init()
{
	host_name = malloc(40*(sizeof(char)));
	strcpy(host_name,"::");
	file_name = NULL;
	port = 0;
}

int receive_datagram(int socket,struct sockaddr_in6 *serv_addr){
    char buffer[600];
    socklen_t src_addr_len = sizeof(serv_addr);
    ssize_t count=recvfrom(socket,buffer,sizeof(buffer),0,(struct sockaddr*)&serv_addr,&src_addr_len);
    if (count==-1) {
        fprintf(stderr,"Erreur lors de la reception du paquet\n");
        return -1;
    } else if (count==sizeof(buffer)) {
        warn("datagram too large for buffer: truncated");
    } else {
        printf(buffer);
    }
}

int main(int argc, const char* argv[])
{	
	init();
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
		else if (strcmp("::1",argv[i])==0 || strcmp("::",argv[i])==0)
		{
			port = -1;
		}
		else
		{
			if(atoi(argv[i])!=0) port = atoi(argv[i]);
			else
			{
				strcpy(host_name,argv[i]); 
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

	struct sockaddr_in6 *serv_addr;
	serv_addr = malloc(sizeof(*serv_addr));
	const char *addr = real_address(host_name, serv_addr);
	if(addr != NULL) fprintf(stderr,"Erreur en convertissant le nom du host en une adresse IPV6 réelle\n");

    int backup;
	int socket = create_socket(serv_addr, port, NULL,0);
	if(socket == -1) return 1;
	int fd;
	if(file_name != NULL)
	{
		chmod(file_name,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
		fd = open(file_name,O_RDWR|O_CREAT, NULL);
		backup = dup(fileno(stdout));
		fd_out = dup2(fd, fileno(stdout));
	}

    printf("Waiting for data...");
    while(1 == 1) {
        receive_datagram(socket, serv_addr);
    }

}

