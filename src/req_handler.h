/* Cabeçalho */

#include "stat_report.h"
#include "cgi_handler.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

typedef struct request{
	int socket_fd;
	struct sockaddr_in client_info;
	socklen_t client_length;
} request;

request * new_request(int new_socket, struct sockaddr_in client_info, socklen_t client_length);
void * handle_request(void* arg);
