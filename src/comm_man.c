/* Cabeçalho */

#include "comm_man.h"

pid_t parent;

/* Creates socket and binds it to all local addresses (0.0.0.0 aka INADDR_ANY) */
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


/* Runs accept, and tries to create threads to handle received requests.
   If unable to do so, handles request itself, and signals the main thread
   of the original process to spawn another accept process. */
void socket_hub(int socketfd,int statfd,int report_fd){
	struct sockaddr_in client_info;
	socklen_t client_length = sizeof(struct sockaddr_in);
	int new_socket;
	
	int debug_feature;
	
	request* aux;
	pthread_attr_t attributes;
	pthread_t* handlers;
	
	pthread_attr_init(&attributes);
	pthread_attr_setdetachstate(&attributes,PTHREAD_CREATE_DETACHED);
	
	while(1){
		new_socket = accept(socketfd,(struct sockaddr*)&client_info,&client_length);
		aux = new_request(new_socket,client_info,client_length,statfd,report_fd);
		handlers = malloc(sizeof(pthread_t));
		if((debug_feature = pthread_create(handlers,&attributes,handle_request,(void*)aux))!=0){
			printf("%d\n",debug_feature);
			if(kill(parent,SIGUSR2)!=0){
				perror("kill");
			}
			handle_request(aux);
		}
		free(handlers);
	}
	
}
