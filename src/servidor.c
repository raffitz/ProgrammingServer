/*Entrega Final do projecto de Programação

	Ano Lectivo 2011/2012 - 2º Semestre

	Filipe João Correia Costa, nº 73055
	Rafael Augusto Maleno Charrama Gonçalves, nº 73786

*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFSIZE 1024
#include "docentes.h"
#include "funcoes.h"
#include "lista1.h"
#include "lista2.h"
#include "lista3.h"
#include "lista4.h"


int main(int argc, char * argv[]){
	FILE * fp_read, *fp_write;
	int serversock;
	char endereco_cliente[100];
	char buffer[BUFFSIZE];
	char extensao[7];												/*Extensão do ficheiro*/
	char sbrowser[5][8] = {"Outro","Firefox","Chrome",
						"Wget","Opera"};			 				/*Array de strings dos browsers*/
	char sso[5][10] = {"Outro","Windows","Linux","Macintosh",
					"Android"};										/*Array de strings dos sistemas operativos*/
	int browser,so,linenr,indexnr;									/*Variáveis para browser, so, tipo de ficheiro e contador de linhas*/
	char ***cts;
	char primeira[20];												/*String para a primeira palavra de uma linha do pedido*/
	int i,j;
	int extr;
	char address[BUFFSIZE];											/*Endereço do ficheiro a abrir*/
	char printaddress[BUFFSIZE];									/*Endereço a ser mostrado no terminal (sem http_docs)*/
	FILE *docbuffer;												/*Variável para onde vai o ficheiro pedido*/
	FILE *backupbuf;
	int code;														/*Variável para o código de resposta*/
	docbuffer = NULL;
	serversock = cria_socket();
	pedido *base;
	base = NULL;
	
	/*Leitura dos tipos de ficheiro*/
	
	backupbuf = fopen("content-type.txt","r");
	if(backupbuf != NULL){
		linenr = 0;
		while(fgets(buffer,BUFFSIZE,backupbuf)!=NULL){
			++linenr;
			if(linenr == 1){
				if(sscanf(buffer,"%d",&indexnr)!=1){
					indexnr = 0;
				}
				if(indexnr>0){
					cts = (char ***) calloc(indexnr,sizeof(char**));
					for(i=0;i<indexnr;++i){
						cts[i] = (char **) calloc(2,sizeof(char*));
						for(j=0;j<2;++j){
							cts[i][j] = (char *) calloc(30,sizeof(char));
						}
					}
				}
				i=0;
				j=0;
			}else{
				if(sscanf(buffer,"%s %s",cts[i][0],cts[i][1])==2){
					++i;
				}else{
					fprintf(stdout,"Houve um erro na leitura dos tipos de ficheiros.\n");
				}
			}
		}		
		fclose(backupbuf);
	}else{
		fprintf(stdout,"Houve um erro na leitura dos tipos de ficheiro.\nPor defeito so podera visualizar ficheiros do tipo htm e html.\n");
		cts = (char ***) calloc(2,sizeof(char**));
		for(i=0;i<2;++i){
			cts[i] = (char **) calloc(2,sizeof(char*));
			for(j=0;j<2;++j){
				cts[i][j] = (char *) calloc(30,sizeof(char));
			}
		}
		indexnr = 2;
		sprintf(cts[0][0],".htm");
		sprintf(cts[0][1],"text/html");
		sprintf(cts[1][0],".html");
		sprintf(cts[1][1],"text/html");
	}
	
	
	/*Leitura do ficheiro de salvaguarda*/

	backupbuf = fopen("backup","r");
	if(backupbuf != NULL){
		base = le_backup(&backupbuf,base,&stdout);
		fclose(backupbuf);
	}else{
		printf("Ficheiro de salvaguarda inexistente.\nUm ficheiro de salvaguarda sera criado automaticamente durante esta sessao.\n");
	}
	/*Início do ciclo infinito:*/

	while (1) {
		espera_pedido(serversock, & fp_read, & fp_write, endereco_cliente);
		browser = so = linenr = 0;
		code = 400;
		printaddress[0] = '\0';
		address[0] = '\0';
		extensao[0] = '\0';
		fgets(buffer,BUFFSIZE,fp_read);
		   

		/*Ciclo de reconhecimento de dados:*/
		while(strcmp(buffer,"\r\n")!=0){
			++linenr;
			if(sscanf(buffer,"%s",primeira) == 1){
				if(strcmp(primeira,"User-Agent:")==0){
					if(strcasestr(buffer,"macintosh")!=NULL){
						so = 3;
					}else if(strcasestr(buffer,"linux")!=NULL){
						if(strcasestr(buffer,"android")!=NULL){
							so = 4;
						}else{
							so = 2;
						}
					}else if(strcasestr(buffer,"windows")!=NULL){
						so = 1;
					}
					if (strcasestr(buffer,"chrome")!=NULL){
						browser = 2;
					}else if(strcasestr(buffer,"firefox")!=NULL){
						browser = 1;
					}else if(strcasestr(buffer,"wget")!=NULL){
						browser = 3;
					}else if(strcasestr(buffer,"opera")!=NULL){
						browser = 4;
					}
				}else if(strcmp(primeira,"GET") == 0){
					if (linenr == 1){
						i=endereco(buffer,printaddress);
						if(i!=-1){
							if(strcmp(printaddress,"/estatisticas/Pedidos")==0){
								/*Os códigos HTTP só vão até ao 5xx
								(de qualquer maneira, este código é interno,
								não vai para o browser) por isso decidimos usar 6xx*/
								code = 600;
							}else if(strcmp(printaddress,"/estatisticas/UltimoFicheiro")==0){
								code = 601;
							}else if(strcmp(printaddress,"/estatisticas/ClearAll")==0){
								code = 602;
							}else if(strcmp(printaddress,"/estatisticas/Clear")==0){
								code = 603;
							}else if(strcmp(printaddress,"/estatisticas/Browser")==0){
								code = 604;
							}else if(strcmp(printaddress,"/estatisticas/Tipo")==0){
								code = 605;
							}else if(strcmp(printaddress,"/estatisticas/Ficheiros")==0){
								code = 606;
							}else{
								strncat(address,"http_docs",9);
								strncat(address,printaddress,BUFFSIZE + 1 - strlen(address));
								extr = ext(address,extensao,cts,indexnr);
								if(extr==-1){
									code = 415;
								}else{
									docbuffer = fopen(address,"r");
									if(docbuffer == NULL){
										code = 404;
									}else{
										code = 200;
									}
								}
							}
						}
					}
				}
			}
			fgets(buffer,BUFFSIZE,fp_read);
		}
		/*Condições para casos extraordinários:*/
		if (extensao[0] == '\0'){
			extensao[0] = '-';
			extensao[1] = '\0';
		}
		if (printaddress[0] == '\0'){
			printaddress[0] = '-';
			printaddress[1] = '\0';
		}
		/*Escrita dos dados:*/
		backupbuf = fopen("backup","a");
		if ((code / 100)>5){
			printf("%-30s %-3d %-10s %-10s %-15s\n",printaddress,200,sbrowser[browser],sso[so],endereco_cliente);
			fprintf(backupbuf,"%-30s %-6s %-3d %-10s %-10s %-15s\n",printaddress,"html",200,sbrowser[browser],sso[so],endereco_cliente);
			if(code != 602) base = insere_pedido(base,printaddress,"html",200,sbrowser[browser],sso[so],endereco_cliente);
		}else{
			printf("%-30s %-3d %-10s %-10s %-15s\n",printaddress,code,sbrowser[browser],sso[so],endereco_cliente);
			fprintf(backupbuf,"%-30s %-6s %-3d %-10s %-10s %-15s\n",printaddress,extensao,code,sbrowser[browser],sso[so],endereco_cliente);
			base = insere_pedido(base,printaddress,extensao,code,sbrowser[browser],sso[so],endereco_cliente);
		}
		fclose(backupbuf);
		/*Envio de resposta*/
		switch(code){
			case 200:
				fprintf(fp_write,"HTTP/1.%d 200 OK\nContent-Type:%s\n\r\n",i,cts[extr][1]);
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
			case 600:
				fprintf(fp_write,"HTTP/1.%d 200 OK\nContent-Type:text/html\n\r\n",i);
				imprime_600(base,&fp_write);
				break;
			case 601:
				fprintf(fp_write,"HTTP/1.%d 200 OK\nContent-Type:text/html\n\r\n",i);
				imprime_601(base,&fp_write);
				break;
			case 602:
				backupbuf = fopen("backup","w");
				fclose(backupbuf);
				base = clearlist(base);
				fprintf(fp_write,"HTTP/1.%d 200 OK\nContent-Type:text/html\n\r\n<html><head><title>Estatisticas - Clear All</title></head>",i);
				fprintf(fp_write,"<body><h2>Todos os registos eliminados.</h2></body></html>");
				break;
			case 603:
				base = clrfich(base);
				backupbuf = fopen("backup","w");
				fclose(backupbuf);
				backupbuf = fopen("backup","a");
				listdump(base,&backupbuf);
				fclose(backupbuf);
				fprintf(fp_write,"HTTP/1.%d 200 OK\nContent-Type:text/html\n\r\n<html><head><title>Estatisticas - Clear</title></head>",i);
				fprintf(fp_write,"<body><h2>Registos de acesso a ficheiros eliminados.</h2></body></html>");
				break;
			case 604:
				fprintf(fp_write,"HTTP/1.%d 200 OK\nContent-Type:text/html\n\r\n",i);
				fazpar(base,&fp_write);
				break;
			case 605:
				fprintf(fp_write,"HTTP/1.%d 200 OK\nContent-Type:text/html\n\r\n",i);
				fazexten(base,&fp_write);
				break;
			case 606:
				fprintf(fp_write,"HTTP/1.%d 200 OK\nContent-Type:text/html\n\r\n",i);
				fazfich(base,&fp_write);
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
