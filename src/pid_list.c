/* cabeçalho */

#include "pid_list.h"

/* Declaration of descendant list: */
pid_list* pid_base;
pid_list* pid_top;

/* Adds a process number to the process list */
void pid_add(pid_t pid,pid_list** base,pid_list** top){
	pid_list* aux;
	if((*base) == NULL){
		(*base) = malloc(sizeof(pid_list));
		aux = (*base);
		(*top)=(*base);
	}else{
		(*(*top)).next = malloc(sizeof(pid_list));
		aux=(*(*top)).next;
	}
	(*aux).next = NULL;
	(*aux).pid = pid;
	(*top) = aux;
	return;
}

/* Frees the process list */
void pid_freeall(pid_list** base,pid_list** top){
	pid_list* aux;
	while((*base)!=NULL){
		aux = (*base);
		(*base) = (*aux).next;
		free(aux);
	}
	*top = NULL;
	return;
}

/* Sends a SIGINT signal to all processes on the list */
void pid_killall(pid_list* base){
	pid_list* aux = base;
	
	while(aux!=NULL){
		kill((*aux).pid,SIGINT);
		waitpid((*aux).pid,NULL,0);
		aux = (*aux).next;
	}
	
	return;
}

/* Receives, from a pipe, process numbers to store in the list. */
void* pid_handler(void* arg){
	int* pipe = (int*)arg;
	pid_t aux;
	while(1){
		read(pipe[0],&aux,sizeof(pid_t));
		pid_add(aux,&pid_base,&pid_top);
	}
}
