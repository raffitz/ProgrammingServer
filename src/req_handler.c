/* Cabeçalho */

#include "req_handler.h"

#define _RH_BUFFSIZE 16

request * new_request(int new_socket, struct sockaddr_in client_info, socklen_t client_length){
	request * aux;
	aux = malloc(sizeof(request));
	(*aux).socket_fd = new_socket;
	(*aux).client_info = client_info;
	(*aux).client_length = client_length;
	return aux;
}

void * handle_request(void* arg){
	request * req = (request *) arg;
	char * buffer = NULL;
	int read_nr;
	int tries = 1;
	
	
	printf("%d Recebeu pedido de %s\n",getpid(),inet_ntoa((*req).client_info.sin_addr));
	/*
	do{
		buffer = realloc(tries * _RH_BUFFSIZE * sizeof(char));
		read_nr = read((*req).socket_fd,buffer,tries * _RH_BUFFSIZE - 1);
	}
	*/
	
	
	dprintf((*req).socket_fd,"HTTP/1.1 200 OK\r\nContent-Type:text/html; charset=utf-8\n\n<html><body>\n");
	dprintf((*req).socket_fd,"Desta vez funcionas ou não, moço? <h1> Acorda para a vida!</h1>\nAddress:%s<br/>Done, I guess...</body></html>\n\r\n",inet_ntoa((*req).client_info.sin_addr));
	close((*req).socket_fd);
	free(req);
	return NULL;
}
