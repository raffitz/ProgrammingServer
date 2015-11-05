/*******************************************************************************
 *	PROJECTO PSis 2ºSEMESTRE 2013-2014
 *		Servidor Web
 *	
 *	Rafael Gonçalves 73786
 *	António Bacelar 73425
 *
 *	Ficheiro: comm_man.h
 *	Descriçao: Includes e Protótipos de comm_man.c
 ******************************************************************************/


#include "req_handler.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <pthread.h>


int create_socket();
void socket_hub(int socketfd,int statfd,int report_fd);
