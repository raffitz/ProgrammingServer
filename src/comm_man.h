/* Cabeçalho */


#include "req_handler.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>


int create_socket();
void socket_hub(int socketfd,int statfd,int report_fd);
