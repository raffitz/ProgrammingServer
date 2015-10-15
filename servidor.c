/*Entrega Intermédia do projecto de Programação

	Ano Lectivo 2011/2012 - 2º Semestre
	
	Rafael Augusto Maleno Charrama Gonçalves, nº 73786

*/
#define _BSD_SOURCE
#define _XOPEN_SOURCE 500
#define _POSIX_C_SOURCE 1
#define _POSIX_SOURCE
#define _GNU_SOURCE
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
/*----------------------------------------------------------------------------*/



/*Procedimento para enviar ficheiros*/
/*Trivializa o envio de dados para o browser*/
void envia(FILE ** fonte, FILE ** destino){
	char buffer[BUFFSIZE];
	int n;
	do{
    	n = fread(buffer,sizeof(char),BUFFSIZE,*fonte);
    	fwrite(buffer,sizeof(char),n,*destino);
    }while(n!=0);
	
}

/*Função para reconhecer e classificar o endereço do ficheiro a abrir*/
int endereco(const char *fonte,char *destino){
	int r;
	char buffer[BUFFSIZE],fim[BUFFSIZE],t[10];
	if(sscanf(fonte,"%*s %s %s %s",buffer,fim,t) == 2){
		if(buffer[strlen(buffer)-1]=='/'){
			strncat(buffer,"index.html",BUFFSIZE+1-strlen(buffer));
		}
		destino[0] = '\0';
		strncat(destino,buffer,BUFFSIZE);
		if(buffer[0] != '/'){
			r = -1;
		}else if(strcmp(fim,"HTTP/1.1")==0){
			r = 1;
		}else if(strcmp(fim,"HTTP/1.0")==0){
			r = 0;
		}else{
			r = -1;
		}
	}else{
		r = -1;
	}
	return r;
}

/*Função para reconhecer a extensão do ficheiro pedido.*/
/*São usados seis caracteres para diminuir a ocorrência de mal-entendidos*/
/*Isto é, para o programa não confundir .htmlo com .html, por exemplo*/
int ext(const char *endereco){
	int r,i,j,k;
	char extensao[6];
	i = j = strlen(endereco);
	while(endereco[i] != '.'){
		i = i-1;
	}
	++i;
	k = i;
	while((k<j)&&(k-i<6)){
		extensao[k-i] = endereco[k];
		extensao[k-i+1] = '\0';
		++k;
	}
	extensao[6] = '\0';
	if((strcmp(extensao,"htm")==0) || (strcmp(extensao,"html")==0)){
		r = 0;
	}else if(strcmp(extensao,"png")==0){
		r = 1;
	}else{
		r = -1;
	}
	return r;
}

int main(int argc, char * argv[]){
  FILE * fp_read, *fp_write;
  int serversock;
  char endereco_cliente[100];
  char buffer[BUFFSIZE];
  char sbrowser[3][10] = {"Outro","Firefox","Chrome"}; 			/*Array de strings dos browsers*/
  char sso[4][10] = {"Outro","Windows","Linux","Macintosh"};	/*Array de strings dos sistemas operativos*/
  char stype[2][10] = {"text/html","image/png"}; 				/*Array de strings dos content-types*/
  int browser,so,type,linenr;									/*Variáveis para browser, so, tipo de ficheiro e contador de linhas*/
  char primeira[20];											/*String para a primeira palavra de uma linha do pedido*/
  int i;
  char address[BUFFSIZE];										/*Endereço do ficheiro a abrir*/
  char printaddress[BUFFSIZE];									/*Endereço a ser mostrado no terminal (sem http_docs)*/
  FILE *docbuffer;												/*Variável para onde vai o ficheiro pedido*/
  int code;														/*Variável para o código de resposta*/
  docbuffer = NULL;
  serversock = cria_socket();
  
  /*Início do ciclo infinito:*/
  
  while (1) {
    espera_pedido(serversock, & fp_read, & fp_write, endereco_cliente);
    browser = so = code = linenr = 0;
    printaddress[0] = '\0';
    address[0] = '\0';
    fgets(buffer,BUFFSIZE,fp_read);
    /*Ciclo de reconhecimento de dados:*/
    while(strcmp(buffer,"\r\n")!=0){
    	++linenr;
    	if(sscanf(buffer,"%s",primeira) == 1){
    		if(strcmp(primeira,"User-Agent:")==0){
    			if(strcasestr(buffer,"macintosh")!=NULL){
    				so = 3;
    			}else if(strcasestr(buffer,"linux")!=NULL){
    				so = 2;
    			}else if(strcasestr(buffer,"windows")!=NULL){
    				so = 1;
    			}
    			if (strcasestr(buffer,"chrome")!=NULL){
    				browser = 2;
    			}else if(strcasestr(buffer,"firefox")!=NULL){
    				browser = 1;
    			}
    		}else if(strcmp(primeira,"GET") == 0){
    			if (linenr == 1){
					i=endereco(buffer,printaddress);
					if(i!=-1){
						strncat(address,"http_docs",9);
						strncat(address,printaddress,BUFFSIZE + 1 - strlen(address));
						type = ext(address);
						if(type == -1){
							code = 415;
						}else{
							docbuffer = fopen(address,"r");
							if(docbuffer == NULL){
								code = 404;
							}else{
								code = 200;
							}
						}
					} else {
						code = 400;
					}
				}else{
					code = 400;
				}
    		}
    	}
   	    fgets(buffer,BUFFSIZE,fp_read);
    }
    /*Escrita dos dados:*/
    printf("%-30s %-3d %-10s %-10s %-15s\n",printaddress,code,sbrowser[browser],sso[so],endereco_cliente);
    /*Envio de resposta*/
    switch(code){
    	case 200:
    		fprintf(fp_write,"HTTP/1.%d 200 OK\nContent-Type:%s\n\r\n",i,stype[type]);
    		envia(&docbuffer,&fp_write);
    		fclose(docbuffer);
    		break;
    	case 404:
    		fprintf(fp_write,"HTTP/1.%d 404 Not Found\nContent-Type:text/html\n\r\n<html><head><title>Erro 404</title></head>",i);
    		fprintf(fp_write,"<body>O ficheiro <a href=\"%s\">%s</a> nao foi encontrado.</body></html>",printaddress,printaddress);
    		break;
    	case 415:
    		fprintf(fp_write,"HTTP/1.%d 415 Unsupported Media Type\nContent-Type:text/html\n\r\n<html><head><title>Erro 415</title></head>",i);
    		fprintf(fp_write,"<body>O tipo de ficheiro especificado nao e suportado pelo servidor.</body></html>");
    		break;
    	default:
    		fprintf(fp_write,"HTTP/1.1 400 Bad Request\n\r\n");
    		break;
    }
    fclose(fp_write);
    fclose(fp_read);
  }
  return 0;
}
