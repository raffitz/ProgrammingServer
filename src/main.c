/*******************************************************************************
 *	PROJECTO PSis 2ºSEMESTRE 2013-2014
 *		Servidor Web
 *	
 *	Rafael Gonçalves 73786
 *	António Bacelar 73425
 *
 *	Ficheiro: main.c
 *	Descriçao: Rotina principal do projecto
 ******************************************************************************/

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
#include <unistd.h>
#include <limits.h>


#define _CF_BUFFER 2048

pid_t parent;

/* Declaration of descendant list: */
pid_list* pid_base;
pid_list* pid_top;

/* Declaration of folder name variables: */
char* pathname;
char* cgipath;

/* Declaration of main thread semaphore: */
sem_t main_sem;

/* Declaration of request list: */
req_base stat_base;

/* Declaration of server socket: */
int socket_fd;

/* Declaration of pid register pipe: */
int pid_register[2];

/* Declaration of report-related pipes: */
int report_requests[2];

/* Declaration of request-writing semaphore */
sem_t* req_wr_sem;

/* Declaration of statistics request semaphore */
sem_t* stat_sem;

/* Debugging purposes-only signal handler:*/
void debug_handler(int sig){
	printf("%d",sig);
	exit(-1);
}

/* SIGINT/SIGTERM handling function: */
void sigint_handler(int sig){
	pid_t local;
	int i;
	
	sem_close(req_wr_sem);
	sem_close(stat_sem);
	
	if((local=getpid())==parent){
		pid_killall(pid_base);
		pid_freeall(&pid_base,&pid_top);
		sem_unlink("/reqwrsem\0");
		sem_unlink("/statsem\0");
	}
	
	printf("%d: Closing All open file descriptors\n",local);
	close(socket_fd);
	for(i=0;i<sysconf(_SC_OPEN_MAX);i++){
		close(i);
	}
	
	exit(0);
	return;
}

/* SIGUSR2 handling function: */
void new_process(int sig){
	pid_t child;
	if(fork()==0){
		signal(SIGUSR2,SIG_DFL);
		child = getpid();
		write(pid_register[1],&child,sizeof(pid_t));
		close(stat_base.req_pipe[0]);
		socket_hub(socket_fd,stat_base.req_pipe[1],report_requests[1]);
	}else{
		return;
	}
}

/* SIGUSR1 handling function: */
void load_config(int sig){
	int fd;
	int i;
	
	char buffer[_CF_BUFFER];
	char* auxbuf;
	
	fd = open("www.config",O_RDONLY);
	if(fd<0){
		pathname = malloc(10*sizeof(char));
		sprintf(pathname,"http_docs");
		cgipath = malloc(1*sizeof(char));
		cgipath[0] = '\0';
	}else{
		i = read(fd,buffer,_CF_BUFFER);
		buffer[i] = '\0';
		auxbuf = strchr(buffer,'\n');
		if(auxbuf!=NULL){
			auxbuf[0]='\0';
			auxbuf = auxbuf + 1;
			if(auxbuf[0]!='/'){
				cgipath = malloc((strlen(auxbuf)+2)*sizeof(char));
				sprintf(cgipath,"/%s",auxbuf);
			}else{
				cgipath = malloc((strlen(auxbuf)+1)*sizeof(char));
				strcpy(cgipath,auxbuf);
			}
			while(cgipath[strlen(cgipath)-1]=='\n' || cgipath[strlen(cgipath)-1]=='\r'){
				cgipath[strlen(cgipath)-1]='\0';
			}
		}
		pathname = malloc((strlen(buffer)+1)*sizeof(char));
		strcpy(pathname,buffer);
	}
	return;
}

/* Main function: */
int main(int argc,char** argv){
	/* Declaration of auxiliary primitives: */
	int i;
	pid_t child;
	
	/* Declaration of structural threads: */
	pthread_t pid_thread;
	pthread_t stat_thread;
	pthread_t report_thread;
	
	/* Initialization of main process semaphore: */
	if(sem_init(&main_sem,0,0)!=0){
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
	
	/* Initialization of parent id container: */
	parent = getpid();
	
	/* Initialization of socket. Bind & listen: */
	socket_fd = create_socket();
	
	/* Initialization of semaphores: */
	req_wr_sem = sem_open("/reqwrsem\0",O_CREAT,0 | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH,1);
	if(req_wr_sem == SEM_FAILED){
		perror("semaphore(req_wr)");
		exit(-1);
	}
	stat_sem = sem_open("/statsem\0",O_CREAT,0 | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH,1);
	if(req_wr_sem == SEM_FAILED){
		perror("semaphore(stat)");
		exit(-1);
	}
	
	/* Enacting Signal Handling Functions: */
	
	signal(SIGINT,sigint_handler);
	signal(SIGTERM,sigint_handler);
	signal(SIGKILL,sigint_handler);
	signal(SIGUSR1,load_config);
	signal(SIGPIPE,SIG_IGN);
	
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
	
	/* Importation/Declaration of the path name: */
	/*  Since it is already neatly handled by the signal function,
	let's just call it instead of duplicating code: */
	load_config(0);
	
	/* Initialization of starting accept processes: */
	
	for(i=0;i<4;i++){
		switch(fork()){
			case 0:
				child = getpid();
				write(pid_register[1],&child,sizeof(pid_t));
				close(stat_base.req_pipe[0]);
				socket_hub(socket_fd,stat_base.req_pipe[1],report_requests[1]);
				printf("Warning! You're not where you ought to be!	\n");
				break;
			default:
				break;
		}
	}
	
	/* Activating the SIGUSR2 handler: */
	signal(SIGUSR2,new_process);
	
	sem_wait(&main_sem);
	
	exit(0);
}
