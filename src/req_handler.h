/*******************************************************************************
 *	PROJECTO PSis 2ºSEMESTRE 2013-2014
 *		Servidor Web
 *	
 *	Rafael Gonçalves 73786
 *	António Bacelar 73425
 *
 *	Ficheiro: req_handler.h
 *	Descriçao: Includes e Protótipos de req_handler.c
 ******************************************************************************/

#include "stat_report.h"
#include "cgi_handler.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

typedef struct request{
	int socket_fd;
	struct sockaddr_in client_info;
	socklen_t client_length;
	int stat_fd;
	int report_fd;
} request;

request * new_request(int new_socket, struct sockaddr_in client_info, socklen_t client_length,int stat_fd,int report_fd);
void * handle_request(void* arg);
