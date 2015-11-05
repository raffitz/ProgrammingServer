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
#include <signal.h>


pid_t parent;

/* Declaration of descendant list: */
pid_list* pid_base;
pid_list* pid_top;

/* Declaration of main thread blocking semaphore: */
sem_t main_sem;

/* Declaration of folder name variable */
char* pathname;


/* Declaration of server socket: */
int socket_fd;

/* SIGINT/SIGTERM handling function: */
void sigint_handler(int signal){
	pid_killall(pid_base);
	pid_freeall(&pid_base,&pid_top);
	printf("closing socket\n");
	close(socket_fd);
	exit(0);
	return;
}

/* Main function: */
int main(int argc,char** argv){
	/* Declaration of auxiliary primitives: */
	int i;
	pid_t child;
	
	/* Declaration of request list: */
	req_base stat_base;
	
	
	/* Declaration of structural threads: */
	pthread_t pid_thread;
	pthread_t stat_thread;
	pthread_t report_thread;
	
	/* Declaration of pid register pipe: */
	int pid_register[2];
	
	/* Declaration of report-related pipes: */
	int report_requests[2];
	
	/* Initialization of main thread blocking semaphore: */
	if(sem_init(&main_sem,0,0)<0){
		perror("sem_init(main)");
		exit(-1);
	}
	
	/* Initialization of statistics list & pipe: */
	stat_base.base = NULL;
	stat_base.top = NULL;
	if(pipe(stat_base.req_pipe)!=0){
		perror("pipes(stat)");
		exit(-1);
	}
	
	/* Initialization of report-related pipes: */
	if(pipe(report_requests)!=0){
		perror("pipes(report)");
		exit(-1);
	}
	
	/* Initialization of pid register pipes: */
	if(pipe(pid_register)!=0){
		perror("pipes(pid)");
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
	if(pthread_create(&stat_thread,NULL,stat_handler,(void*)&stat_base)!=0){
		perror("pthread(stat)");
		exit(-1);
	}
	if(pthread_create(&report_thread,NULL,report_handler,(void*)set_report_args(report_requests[0], &stat_base))!=0){
		perror("pthread(report)");
		exit(-1);
	}
	
	/* Declaration of the path name: */
	pathname = malloc(10*sizeof(char));
	sprintf(pathname,"http_docs");
	
	/* Enacting Signal Handling Functions: */
	
	signal(SIGINT,sigint_handler);
	signal(SIGTERM,sigint_handler);
	
	/* Initialization of starting accept processes: */
	for(i=0;i<4;i++){
		switch(fork()){
			case 0:
				child = getpid();
				write(pid_register[1],&child,sizeof(pid_t));
				close(stat_base.req_pipe[0]);
				socket_hub(socket_fd,stat_base.req_pipe[1],report_requests[1]);
				break;
			default:
				break;
		}
	}
	
	
	/* Blocking main thread: */
	sem_wait(&main_sem);
	
	exit(0);
}
