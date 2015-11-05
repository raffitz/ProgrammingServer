/* Cabeçalho */

#include "req_handler.h"

#define _RH_BUFFSIZE 16

char* pathname;

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
	char* buffer = NULL;
	char* name = NULL;
	char* c_name = NULL;
	char* breakplace = NULL;
	char* extension;
	struct stat filestat;
	int read_nr;
	int tries = 1;
	int file_fd;
	pid_t cgi_aux;
	uint16_t code;
	uint8_t address[4];
	uint8_t protocol;
	uint8_t type;
	
	sscanf(inet_ntoa((*req).client_info.sin_addr),"%"SCNu8".%"SCNu8".%"SCNu8".%"SCNu8"",&address[0],&address[1],&address[2],&address[3]);
	
	printf("%d Recebeu pedido de %u.%u.%u.%u\n",getpid(),address[0],address[1],address[2],address[3]);
	
	while(1){
		buffer = realloc(buffer,tries * _RH_BUFFSIZE * sizeof(char) + 1);
		read_nr = read((*req).socket_fd,buffer + (tries-1) * _RH_BUFFSIZE * sizeof(char),_RH_BUFFSIZE);
		buffer[read_nr + (tries-1)*_RH_BUFFSIZE] = '\0';
		if((breakplace = strchr(buffer,'\n'))!=NULL){
			/*printf("\n«a«%s»a»\n\n«b«%s»b»\n",buffer,breakplace);*/
			if (*(breakplace - sizeof(char)) != '\r'){
				code = 400;
			}else{
				breakplace='\0';
				name = malloc(strlen(buffer)*sizeof(char));
				if(sscanf(buffer,"GET %s HTTP/1.%"SCNu8,name,&protocol)!=2){
					code = 400;
				}else{
					if(protocol>1){
						code = 400;
					}else{
						c_name = malloc((strlen(pathname)+strlen(name)+1)*sizeof(char));
						strcpy(c_name,pathname);
						strcat(c_name,name);
						if(strcmp(name,"/estatisticas/Pedidos")==0){
							code = 200;
							type = 2;
						}else if(strcmp(name,"/estatisticas/ClearAll")==0){
							code = 200;
							type = 4;
						}else{
							if(name[strlen(name)-1]=='/'){
								/* Directory */
								code = 200;
								type = 3;
							}else{
								file_fd = open(c_name,O_RDONLY);
								if(file_fd < 0){
									code = 404;
								}else{
									if(fstat(file_fd,&filestat)==0 && filestat.st_mode & S_IXUSR){
										/* CGI */
										code = 200;
										type = 5;
									}else{
										/* File */
										extension = strrchr(name,'.');
										if(strcmp(extension,".html")==0){
											code = 200;
											type = 0;
										}else if(strcmp(extension,".png")==0){
											code = 200;
											type = 1;
										}else{
											code = 415;
										}
									}
								}
							}
						}
					}
				}
			}
			break;
		}
		tries++;
	}
	/* Reads rest of request, so browsers don't reject answer: */
	read_nr = 0;
	do{
		read_nr = read((*req).socket_fd,buffer,tries * _RH_BUFFSIZE * sizeof(char));
		buffer[read_nr + (tries-1)*_RH_BUFFSIZE] = '\0';
	}while(read_nr==tries * _RH_BUFFSIZE * sizeof(char));
	
	/* Sends answer: */
	switch(code){
		case 200:
			dprintf((*req).socket_fd,"HTTP/1.%"SCNu8" 200 OK\r\n",protocol);
			if(type%2 == 0){
				dprintf((*req).socket_fd,"Content-Type:text/html; charset=utf-8\n\n");
			}else if(type==1){
				dprintf((*req).socket_fd,"Content-Type:image/png\n\n");
			}else if(type==5){
				/*CGI*/
				cgi_aux = 0;
				if((cgi_aux = fork())==0){
					cgi_run((*req).socket_fd,c_name);
				}else{
					waitpid(cgi_aux,NULL,0);
				}
			}
			if(type<2){
				do{
					read_nr = read(file_fd,buffer,tries * _RH_BUFFSIZE * sizeof(char));
					write((*req).socket_fd,buffer,read_nr);
				}while(read_nr > 0);
				dprintf((*req).socket_fd,"\n");
			}else if(type==3){
				cgi_aux = 0;
				if((cgi_aux = fork())==0){
					cgi_run_args((*req).socket_fd,"listfiles",c_name);
				}else{
					waitpid(cgi_aux,NULL,0);
				}
			}
			break;
		case 415:
			dprintf((*req).socket_fd,"HTTP/1.%"SCNu8" 415 Unsupported Media Type\r\n\n",protocol);
			break;
		case 404:
			dprintf((*req).socket_fd,"HTTP/1.%"SCNu8" 404 Not Found\r\n\n<html><head><title>File Not Found</title></head><body><h1>Error 404</h1> This file was not found.</body></html>",protocol);
			break;
		case 400:
		default:
			dprintf((*req).socket_fd,"HTTP/1.%"SCNu8" 400 Bad Request\r\n\n",protocol);
			break;
	}
	close((*req).socket_fd);
	if(buffer!=NULL) free(buffer);
	if(name!=NULL) free(name);
	if(c_name!=NULL) free(c_name);
	free(req);
	return NULL;
}
