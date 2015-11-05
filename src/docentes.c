#define _BSD_SOURCE
#define _XOPEN_SOURCE 500
#define _POSIX_C_SOURCE 1
#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define MAXPENDING 10    /* Max connection requests */
#define BUFFSIZE 1024

int cria_socket(){
	int serversock;
	struct sockaddr_in echoserver;
	char host[1024];
	struct hostent *host_entry;
	char * localIP;
	int port_number;


	port_number = 5000;
	/* Create the TCP socket */
	if ((serversock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("impossivel criar socket. O servidor vai sair.\n");
		exit(1);
	}

	/* Construct the server sockaddr_in structure */
	memset(&echoserver, 0, sizeof(echoserver));       /* Clear struct */
	echoserver.sin_family = AF_INET;                  /* Internet/IP */
	echoserver.sin_addr.s_addr = htonl(INADDR_ANY);   /* Incoming addr */
	port_number--;
	do{
		port_number++;
		echoserver.sin_port = htons(port_number);       /* server port */

		/* Bind the server socket */
	}while(bind(serversock, (struct sockaddr *) &echoserver, sizeof(echoserver)) < 0);

	/*obter o nome da máquina */
	gethostname(host, 1024);
	host_entry=gethostbyname(host);
	localIP = inet_ntoa (*(struct in_addr *)*host_entry->h_addr_list);
	printf("\n\n\tuse um dos seguintes enderecos no navegador: \n");
	printf("\t\thttp://%s:%d/\n", host, port_number);
	printf("\tou\n");
	printf("\t\tServidor em: http://%s:%d/\n", localIP, port_number);
	printf("\n\n\tSe necessária de autorizacao na Firewal.\n\n"); 

	/* Escuta pedidos */
	if (listen(serversock, MAXPENDING) < 0) {
		perror("Impossível criar o socket. O servidor vai sair.\n");
		exit(1);
	}
	return serversock;
}


void  espera_pedido(int serversock, FILE** fp_read, FILE ** fp_write, char *remote_address){
	int clientsock;
	struct sockaddr_in  echoclient;

	unsigned int clientlen = sizeof(echoclient);
	/* Wait for client connection */
	if ((clientsock = accept(serversock, (struct sockaddr *) &echoclient, &clientlen)) < 0) {
		perror("Impossivel estabelecer ligacao ao cliente. O servidor vai sair.\n");
		exit(1);
	}
	sprintf(remote_address, "%s", inet_ntoa(echoclient.sin_addr));

	*fp_read = fdopen(clientsock, "r");
	*fp_write = fdopen(clientsock, "w");

}
