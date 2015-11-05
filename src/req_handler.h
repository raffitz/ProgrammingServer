/* Cabeçalho */

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

typedef struct request{
	int socket_fd;
	struct sockaddr_in client_info;
	socklen_t client_length;
	int stat_fd;
	int report_fd;
} request;

request * new_request(int new_socket, struct sockaddr_in client_info, socklen_t client_length,int stat_fd,int report_fd);
void * handle_request(void* arg);
