/* Cabeçalho */

#include "comm_man.h"


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
	int new_socket;
	
	while(1){
		new_socket = accept(socketfd,(struct sockaddr*)&client_info,&client_length);
		handle_request(new_request(new_socket,client_info,client_length));
	}
	
}
