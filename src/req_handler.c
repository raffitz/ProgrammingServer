/* Cabeçalho */

#include "req_handler.h"

#define _RH_BUFFSIZE 16

char* pathname; /* DOCUMENT_ROOT */
char* cgipath; /* CGI_ROOT */

sem_t* req_wr_sem;
sem_t* stat_sem;

request * new_request(int new_socket, struct sockaddr_in client_info, socklen_t client_length,int stat_fd,int report_fd){
	request * aux;
	aux = malloc(sizeof(request));
	(*aux).socket_fd = new_socket;
	(*aux).client_info = client_info;
	(*aux).client_length = client_length;
	(*aux).stat_fd = stat_fd;
	(*aux).report_fd = report_fd;
	return aux;
}

void * handle_request(void* arg){
	request * req = (request *) arg;
	char* buffer = NULL;
	char* name = NULL;
	char* c_name = NULL;
	char* breakplace = NULL;
	char* extension;
	char report_name[64];
	char firstc;
	struct stat filestat;
	int read_nr;
	int tries = 1;
	int file_fd;
	pid_t cgi_aux;
	pid_t cgi_tim;
	uint16_t code = 0;
	uint8_t i;
	uint8_t address[4];
	uint8_t protocol;
	uint8_t type;
	uint8_t report_type;
	uint8_t report_ns;
	uint8_t aborted = 0;
	time_t start;
	clockid_t counter;
	struct timespec dstart,dend;
	
	start = time(NULL);
	clock_getcpuclockid(getpid(),&counter);
	if(clock_gettime(counter,&dstart)!=0){
		perror("clock_gettime");
	}
	
	sscanf(inet_ntoa((*req).client_info.sin_addr),"%"SCNu8".%"SCNu8".%"SCNu8".%"SCNu8"",&address[0],&address[1],&address[2],&address[3]);
	
	while(1){
		/* Bugtesting purposes only: */
		if(tries>16){
			printf("\n%d:«««%s»»»\n",tries,buffer);
		}
		
		/* end */
		buffer = realloc(buffer,sizeof(char)*(1 + tries * _RH_BUFFSIZE));
		if(buffer==NULL){
			perror("realloc");
			printf("Error: Out of memory!\n");
			break;
		}
		read_nr = read((*req).socket_fd,buffer + (tries-1) * _RH_BUFFSIZE * sizeof(char),_RH_BUFFSIZE);
		buffer[(read_nr) + (tries-1)*_RH_BUFFSIZE] = '\0';
		if(strlen(buffer)==0){
			code = 400;
			break;
		}else if((breakplace = strchr(buffer,'\n'))!=NULL){
			if (*(breakplace - sizeof(char)) != '\r'){
				code = 400;
			}else{
				breakplace[0]='\0';
				name = malloc(strlen(buffer)*sizeof(char));
				if(sscanf(strrchr(buffer,'H'),"HTTP/1.%"SCNu8,&protocol)!=1 || strstr(buffer,"GET ")==NULL){
					code = 400;
				}else{
					for(i=0;i<strlen(buffer);i++){
						if(buffer[i]!=' ' && &(buffer[i]) < strstr(buffer,"GET ")){
							code = 400;
							break;
						}
					}
					if(code==400) break;
					breakplace = strrchr(buffer,'H') - sizeof(char);
					breakplace[0] = '\0';
					strcpy(name,strstr(buffer,"GET ")+4*sizeof(char));
					for(i=0;i<strlen(name);i++){
						if(name[i]!=' '){
							firstc = name[i];
							break;
						}
					}
					if(firstc!='/'){
						code = 400;
					}else{
						if(protocol>1){
							code = 400;
						}else{
							c_name = malloc((strlen(pathname)+strlen(name)+1)*sizeof(char));
							strcpy(c_name,pathname);
							strcat(c_name,name);
							if(strcmp(name,"/estatisticas/Pedidos")==0){
								/* Statistics module: */
								code = 200;
								type = 8;
							}else if(strcmp(name,"/estatisticas/ClearAll")==0){
								code = 200;
								type = 6;
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
											if(cgipath[0]!='\0' && strstr(name,cgipath)==NULL){
												code = 403;
											}else{
												code = 200;
												type = 5;
											}
											close(file_fd);
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
												close(file_fd);
												code = 415;
											}
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
	do{
		if(read((*req).socket_fd,buffer,sizeof(char))==0){
			aborted = 1;
		}else if(buffer[0]=='\n'){
			read((*req).socket_fd,buffer,sizeof(char));
			if(buffer[0]=='\r'){
				read((*req).socket_fd,buffer,sizeof(char));
				if(buffer[0]=='\n') break;
			}
		}
	}while(1);
	
	if(!aborted){
		/* Sends answer: */
		switch(code){
			case 200:
				/* Sends adequate header: */
				dprintf((*req).socket_fd,"HTTP/1.%"SCNu8" 200 OK\r\n",protocol);
				/* If content is html-formatted text: */
				if(type%2 == 0){
					dprintf((*req).socket_fd,"Content-Type:text/html; charset=utf-8\n\n");
				}
				/* If content is a png image: */
				if(type==1){
					dprintf((*req).socket_fd,"Content-Type:image/png\n\n");
				}
			
				/* If content is a CGI: */
				if(type==5){
					cgi_aux = 0;
					if((cgi_aux = fork())==0){
						cgi_run((*req).socket_fd,c_name);
					}else if((cgi_tim = fork())==0){
						/* Given that it is a sub-process, "buffer" can be
						carelessly re-utilized here: */
						free(buffer);
						buffer = malloc(512*sizeof(char));
						sprintf(buffer,"%lld",(long long int)cgi_aux);
						execl("cgi_timer","cgi_timer",buffer,NULL);
					}else{
						waitpid(cgi_aux,NULL,0);
						if(!waitpid(cgi_tim,NULL,WNOHANG)){
							kill(cgi_tim,SIGKILL);
							waitpid(cgi_tim,NULL,0);
						}
					}
				}
			
				/* If we are interfacing with the statistics module: */
				if(type>5){
					if(sem_wait(stat_sem)!=0) perror("sem_wait");
					if(type==6){
						report_type=0;
					}else{
						report_type=1;
					}
					write((*req).report_fd,&report_type,sizeof(uint8_t));
					sprintf(report_name,"%x_%x",getpid(),(unsigned int) pthread_self());
					report_ns = strlen(report_name) + 1;
					write((*req).report_fd,&report_ns,sizeof(uint8_t));
					if(mkfifo(report_name,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)!=0){
						perror("mkfifo");
						break;
					}
					write((*req).report_fd,report_name,report_ns*sizeof(char));
					file_fd = open(report_name,O_RDONLY);
					if(file_fd<0){
						perror("open(fifo rh)");
						unlink(report_name);
						if (sem_post(stat_sem)!=0) perror("sem_post");
						break;
					}
				}
			
				/* If content is a file (content-type has been already sent): */
				if(type<2 || type>5){
					do{
						read_nr = read(file_fd,buffer,tries * _RH_BUFFSIZE * sizeof(char));
						if(write((*req).socket_fd,buffer,read_nr)<0){
							break;
						}
					}while(read_nr > 0);
					close(file_fd);
				}
			
				/* Continuation of the statistics section: */
				if(type>5){
					unlink(report_name);
					sem_post(stat_sem);
				}
			
				/* If we are listing contents of a directory: */
				if(type==3){
					cgi_aux = 0;
					if((cgi_aux = fork())==0){
						cgi_run_args((*req).socket_fd,"listfiles",c_name,name);
					}else{
						waitpid(cgi_aux,NULL,0);
					}
				}
				break;
			case 415:
				dprintf((*req).socket_fd,"HTTP/1.%"SCNu8" 415 Unsupported Media Type\r\n\n",protocol);
				break;
			case 404:
				dprintf((*req).socket_fd,"HTTP/1.%"SCNu8" 404 Not Found\r\n\n<html><head><title>File Not Found</title></head><body><h1>Error 404</h1> The file \"%s\" was not found.</body></html>",protocol,name);
				break;
			case 403:
				dprintf((*req).socket_fd,"HTTP/1.%"SCNu8" 403 Forbidden\r\n\n",protocol);
				break;
			case 400:
			default:
				dprintf((*req).socket_fd,"HTTP/1.%"SCNu8" 400 Bad Request\r\n\n",protocol);
				break;
		}
	}
	
	close((*req).socket_fd);
	if(clock_gettime(counter,&dend)!=0){
		perror("clock_gettime");
	}
	if(sem_wait(req_wr_sem)!=0) perror("sem_wait");
	req_write((*req).stat_fd,newrequest(name,code,address,start,(dend.tv_sec - dstart.tv_sec)*1000000 + (dend.tv_nsec - dstart.tv_nsec) / 1000));
	if(sem_post(req_wr_sem)!=0) perror("sem_post");
	
	/*
	printf("%d Recebeu pedido de %u.%u.%u.%u e demorou %ld\n",getpid(),address[0],address[1],address[2],address[3],(dend.tv_sec - dstart.tv_sec)*1000000 + (dend.tv_nsec - dstart.tv_nsec) / 1000);*/
	
	if(buffer!=NULL) free(buffer);
	if(c_name!=NULL) free(c_name);
	free(req);
	return NULL;
}
