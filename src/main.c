/* Cabeçalho */


#include "pid_list.h"
#include "comm_man.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/types.h>


pid_t parent;

/* Declaration of descendant list: */
pid_list* pid_base;
pid_list* pid_top;

int main(int argc,char** argv){
	/* Declaration of request list: */
	req_queue* req_base;
	req_queue* req_top;
	
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
	socketfd = create_socket();
	
	/* Initialization of parent id container: */
	parent = getpid();
	
	/* Initialization of structural threads: */
	if(pthread_create(&pidthread,NULL,pid_handler,(void*)pid_register)!=0){
		perror("pthread(pid)");
		exit(-1);
	}
	
	
	
	
	
	
	/* Resto do código com os forks e os threads etc e tal */
	
	
}
