/* Cabeçalho */

#include "comm_man.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

int create_socket(){
	int socketfd;
	struct addrinfo* server_address;
	
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketfd<0){
		perror("socket");
		exit(-1);
	}
	if(getaddrinfo("0.0.0.0","8080",NULL,&server_address)!=0){
		perror("getaddrinfo");
		exit(-1);
	}
	if(bind(socketfd,(*server_address).ai_addr,(*server_address).ai_addrlen)<0){
		perror("bind");
		exit(-1);
	}
	
	if(listen(socketfd,127)<0){
		perror("listen");
		exit(-1);
	}
	freeaddrinfo(server_address);
	return socketfd;
}
