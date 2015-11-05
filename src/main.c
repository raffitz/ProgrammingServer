/* Cabeçalho */

#define _XOPEN_SOURCE 700
#include "pid_list.h"
#include "comm_man.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/socket.h>


pid_t parent;

/* Declaration of descendant list: */
pid_list* pid_base;
pid_list* pid_top;

int main(int argc,char** argv){
	/* Declaration of request list: */
	/*req_queue* req_base;
	req_queue* req_top;*/
	
	/* Declaration of server socket: */
	int socket_fd;
	
	int news;
	struct sockaddr_in client_info;
	socklen_t client_isize;
	char cenas[100];
	int cenasc;
	
	/* Declaration of structural threads: */
	/*pthread_t pid_thread;*/
	
	/* Declaration of pid register pipe: */
	/*int pid_register[2];*/
	
		
	/* Initialization of pipes: */
	/*if(pipe(pid_register)!=0){
		perror("pipes");
		exit(-1);
	}*/
	
	/* Initialization of socket. Bind & listen: */
	socket_fd = create_socket();
	
	/* Initialization of parent id container: */
	parent = getpid();
	
	/* Initialization of structural threads: *//*
	if(pthread_create(&pidthread,NULL,pid_handler,(void*)pid_register)!=0){
		perror("pthread(pid)");
		exit(-1);
	}
	*/
	while(1){
		news = accept(socket_fd,(struct sockaddr*)&client_info,&client_isize);
		printf("Recebeu pedido.\n");
		dprintf(news,"HTTP/1.1 200 OK\r\nContent-Type:text/html; charset=utf-8\n\n<html><body>\n");
		cenasc = read(news,cenas,100);
		dprintf(news,"Desta vez funcionas ou não, moço? <h1> Acorda para a vida!</h1>\n</body></html>\n\r\n");
		do{
			cenasc = read(news,cenas,100);
		}while(cenasc>0);
		close(news);
	}
	
	
	
	
	
	/* Resto do código com os forks e os threads etc e tal */
	
	
}
