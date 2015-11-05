/* Cabeçalho */


#include "pid_list.h"
#include "comm_man.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>


pid_t parent;

/*Declaration of pid semaphore: */
sem_t* pid_semaphore;

/* Declaration of descendant list: */
pid_list* pid_base;
pid_list* pid_top;

int main(int argc,char** argv){
	/* Declaration of request list: */
	req_queue* req_base;
	req_queue* req_top;
	
	/* Declaration of server socket: */
	int socket_fd;
	struct addrinfo* server_address;
	
	/* Declaration of structural threads: */
	pthread_t pid_thread;
	
	/* Declaration of pid register pipe: */
	int pid_register[2];
	
		
	/* Initialization of pipes: */
	if(pipe(pid_register)!=0){
		perror("pipes");
		exit(-1);
	}
	
	/* Initialization of pid semaphore: */
	pid_semaphore = sem_open("/pid_semaphore",O_CREAT || O_TRUNC || O_RDWR,S_IRUSR || S_IWUSR || S_IRGRP || S_IWGRP || S_IROTH,0);
	
	/* Initialization of parent id container: */
	parent = getpid();
	
	/* Initialization of structural threads: */
	if(pthread_create(&pidthread,NULL,pid_handler,(void*)pid_register)!=0){
		perror("pthread(pid)");
		exit(-1);
	}
	
	
	
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketfd<0){
		perror("socket");
		exit(-1);
	}
	if(getaddrinfo("0.0.0.0","8080",NULL,&server_address)!=0){
		perror("getaddrinfo");
		exit(-1);
	}
	if(bind(socketfd,server_address[0].ai_addr,server_address[0].ai_addrlen)<0){
		perror("bind");
		exit(-1);
	}
	
	if(listen(socketfd,127)<0){
		perror("listen");
		exit(-1);
	}
	
	
	/* Resto do código com os forks e os threads etc e tal */
	
	
}
