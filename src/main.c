/* Cabeçalho */

#define _XOPEN_SOURCE 700
#define _BSD_SOURCE
#include "pid_list.h"
#include "comm_man.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/socket.h>


pid_t parent;

/* Declaration of descendant list: */
pid_list* pid_base;
pid_list* pid_top;

char* pathname;

int main(int argc,char** argv){
	/* Declaration of auxiliary primitives: */
	int i;
	pid_t child;
	
	/* Declaration of request list: */
	/*req_queue* req_base;
	req_queue* req_top;*/
	
	/* Declaration of server socket: */
	int socket_fd;
	
	/* Declaration of structural threads: */
	pthread_t pid_thread;
	
	/* Declaration of pid register pipe: */
	int pid_register[2];
	
		
	/* Initialization of pipes: */
	if(pipe(pid_register)!=0){
		perror("pipes");
		exit(-1);
	}
	
	/* Initialization of socket. Bind & listen: */
	socket_fd = create_socket();
	
	/* Initialization of parent id container: */
	parent = getpid();
	
	/* Initialization of structural threads: */
	if(pthread_create(&pid_thread,NULL,pid_handler,(void*)pid_register)!=0){
		perror("pthread(pid)");
		exit(-1);
	}
	
	/* Declaration of the path name: */
	pathname = malloc(10*sizeof(char));
	sprintf(pathname,"http_docs");
	
	/* Initialization of starting accept processes: */
	for(i=0;i<8;i++){
		switch(fork()){
			case 0:
				child = getpid();
				write(pid_register[1],&child,sizeof(pid_t));
				socket_hub(socket_fd);
				break;
			default:
				break;
		}
	}
	while(1);
	
	
	/* Resto do código com os forks e os threads etc e tal */
	
	exit(0);
}
