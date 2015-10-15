#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFSIZE 1024

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

int ext(const char *endereco,char*exten, char *** cts, int maxcts){
	int i;
	char extensao[7];
	i = strlen(endereco);
	extensao[0] = '\0';
	while((endereco[i] != '.') && i>0){
		i = i-1;
	}
	strncpy(extensao,&endereco[i],7);
	extensao[7] = '\0';
	strcpy(exten,extensao);
	for(i=0;i<maxcts;++i){
		if(strcmp(extensao,cts[i][0])==0){
			return i;
		}
	}
	return -1;
}

