/* Cabeçalho */

#include "comm_man.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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

void socket_hub(int socketfd){
	struct sockaddr_in client_info;
	socklen_t client_length = sizeof(struct sockaddr_in);
	int news;
	int length;
	char buffer[100];
	char jic;
	int bufferc;
	
	
	while(1){
		news = accept(socketfd,(struct sockaddr*)&client_info,&client_length);
		printf("Recebeu pedido.\n");
		jic = '\0';
		/*do{
			bufferc = read(news,buffer,99);
			buffer[99]=0;
			length = strlen(buffer);
			if (length==1 && jic=='\r') break;
			if(buffer[length-1]=='\n' && buffer[length-2]=='\r') break;
			jic = buffer[length-1];
		}while(bufferc>0);*/
		
		dprintf(news,"HTTP/1.1 200 OK\r\nContent-Type:text/html; charset=utf-8\n\n<html><body>\n");
		dprintf(news,"Desta vez funcionas ou não, moço? <h1> Acorda para a vida!</h1>\nAddress:%s<br/>Done, I guess...</body></html>\n\r\n",inet_ntoa(client_info.sin_addr));
		close(news);
	}
	
}
