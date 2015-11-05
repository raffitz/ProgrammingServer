#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lista1.h"


/*Função para inserir pedido na lista dinâmica*/
pedido * insere_pedido(pedido*base,const char * end,const char * tipo, int code, const char * browser, const char * so, const char * cliente){
	pedido * aux;
	aux = base;
	if(aux == NULL){
		aux = malloc(sizeof(pedido));
		strncpy((*aux).endereco,end,BUFFSIZE);
		strncpy((*aux).tipo,tipo,6);
		(*aux).code = code;
		strncpy((*aux).browser,browser,8);
		strncpy((*aux).so,so,10);
		strncpy((*aux).cliente,cliente,100);
		(*aux).nxt = NULL;
		base = aux;
	}else{
		while((*aux).nxt !=NULL){
			aux = (*aux).nxt;
		}
		(*aux).nxt = malloc(sizeof(pedido));
		aux = (*aux).nxt;
		strncpy((*aux).endereco,end,BUFFSIZE);
		strncpy((*aux).tipo,tipo,6);
		(*aux).code = code;
		strncpy((*aux).browser,browser,8);
		strncpy((*aux).so,so,10);
		strncpy((*aux).cliente,cliente,100);
		(*aux).nxt = NULL;
	}
	return base;
}

/*Procedimento para imprimir a lista de pedidos*/
void imprime_600(pedido *base, FILE ** destino){
	pedido * aux;
	/*Não vale a pena verificar se a base é NULL já que na lista dinâmica existe pelo menos o pedido "/estatisticas/Pedidos"*/
	if((*base).nxt!=NULL){
		fprintf(*destino,"<html><head><title>Estatisticas - Pedidos</title></head><body><h2>Lista de Pedidos</h2><br/><table><tr><th>Ficheiro pedido</th><th>Extensao</th><th>Codigo</th><th>Browser</th><th>Sistema Operativo</th><th>Endereco IP do cliente</th></tr>");
		aux = base;
		while((*aux).nxt != NULL){
			fprintf(*destino,"<tr><td><a href=\"%s\">%-30s</a></td><td>%-6s</td><td>%-3d</td><center><td>%-10s</td><td>%-10s</td><td>%-15s</td></center></tr>\n",(*aux).endereco,(*aux).endereco,(*aux).tipo,(*aux).code,(*aux).browser,(*aux).so,(*aux).cliente);
			aux = (*aux).nxt;
		}
		fprintf(*destino,"</table>\n</body></html>");
	}else{
		fprintf(*destino,"<html><head><title>Estatisticas - Pedidos</title></head><body><h2>Lista de Pedidos</h2><br/>Nao ha registo de pedidos efectuados.</body></html>");
	}
}

/*Procedimento para imprimir a lista de pedidos relacionados com o último ficheiro pedido*/
void imprime_601(pedido *base, FILE ** destino){
	pedido *aux;
	aux = base;
	char ultimo[BUFFSIZE];
	
	/*Não vale a pena verificar se a base é NULL já que na lista dinâmica existe pelo menos o pedido "/estatisticas/Ultimo/Ficheiro"*/
	if((*aux).nxt == NULL){
		fprintf(*destino,"<html><head><title>Estatisticas - Ultimo Ficheiro</title></head><body><h2>Ultimo Ficheiro</h2><br/>Nao ha registo de pedidos efectuados.<br/></body></html>");
	}else{
		while((*(*aux).nxt).nxt !=NULL){
			aux = (*aux).nxt;
		}
		strcpy(ultimo,(*aux).endereco);	
		
		fprintf(*destino,"<html><head><title>Estatisticas - Ultimo Ficheiro</title></head><body><h2>Ultimo Ficheiro: %s</h2><br/><table><tr><th>Ficheiro pedido</th><th>Extensao</th><th>Codigo</th><th>Browser</th><th>Sistema Operativo</th><th>Endereco IP do cliente</th></tr>",ultimo);
		aux = base;
		while(aux != NULL){
			if(strcmp(ultimo,(*aux).endereco)==0){
				fprintf(*destino,"<tr><td><a href=\"%s\">%-30s</a></td><td>%-6s</td><td>%-3d</td><center><td>%-10s</td><td>%-10s</td><td>%-15s</td></center></tr>\n",(*aux).endereco,(*aux).endereco,(*aux).tipo,(*aux).code,(*aux).browser,(*aux).so,(*aux).cliente);
			}
			aux = (*aux).nxt;
		}
		fprintf(*destino,"</table>\n</body></html>");
	}
}

/*Função que percorre um ficheiro e dele extrai os pedidos nele armazenados*/
pedido * le_backup(FILE ** origem,pedido*base,FILE ** reportaddress){
	char linha[BUFFSIZE];
	char endereco[BUFFSIZE];
	char tipo[6];
	int code;
	char browser[8];
	char so[10];
	char cliente[100];
	int i = 0;
	while(fgets(linha,BUFFSIZE,*origem)!=NULL){
		++i;
		if(sscanf(linha,"%s %s %d %s %s %s",endereco,tipo,&code,browser,so,cliente)==6){
			base = insere_pedido(base,endereco,tipo,code,browser,so,cliente);
		}else{
			fprintf(*reportaddress,"%4d: Erro na leitura do ficheiro de salvaguarda.\n",i);
		}
	}
	return base;
}

/*Função que apaga uma lista dinâmica*/
pedido * clearlist(pedido*base){
	pedido *aux;
	aux = base;
	if(base!=NULL){
		while((*base).nxt!=NULL){
			while((*(*aux).nxt).nxt !=NULL){
				aux = (*aux).nxt;
			}
			free((*aux).nxt);
			(*aux).nxt = NULL;
			aux = base;
		}
		free(base);
	}
	return NULL;
}

/*Função que elimina todos os pedidos não relacionados com estatística*/
pedido * clrfich(pedido*base){
	pedido *basenova,*aux;
	basenova = NULL;
	aux = base;
	while(aux!=NULL){
		if(strcmp((*aux).endereco,"/estatisticas/Pedidos")==0){
			basenova = insere_pedido(basenova,(*aux).endereco,(*aux).tipo,(*aux).code,(*aux).browser,(*aux).so,(*aux).cliente);
		}else if(strcmp((*aux).endereco,"/estatisticas/UltimoFicheiro")==0){
			basenova = insere_pedido(basenova,(*aux).endereco,(*aux).tipo,(*aux).code,(*aux).browser,(*aux).so,(*aux).cliente);
		}else if(strcmp((*aux).endereco,"/estatisticas/Clear")==0){
			basenova = insere_pedido(basenova,(*aux).endereco,(*aux).tipo,(*aux).code,(*aux).browser,(*aux).so,(*aux).cliente);
		}else if(strcmp((*aux).endereco,"/estatisticas/Browser")==0){
			basenova = insere_pedido(basenova,(*aux).endereco,(*aux).tipo,(*aux).code,(*aux).browser,(*aux).so,(*aux).cliente);
		}else if(strcmp((*aux).endereco,"/estatisticas/Ficheiros")==0){
			basenova = insere_pedido(basenova,(*aux).endereco,(*aux).tipo,(*aux).code,(*aux).browser,(*aux).so,(*aux).cliente);
		}else if(strcmp((*aux).endereco,"/estatisticas/Tipo")==0){
			basenova = insere_pedido(basenova,(*aux).endereco,(*aux).tipo,(*aux).code,(*aux).browser,(*aux).so,(*aux).cliente);
		}
		aux = (*aux).nxt;
	}
	base = clearlist(base);
	return basenova;
}

/*Procedimento que escreve os dados duma lista num ficheiro*/
void listdump(pedido *base, FILE ** destino){
	pedido * aux;
	if(base!=NULL){
		aux = base;
		while(aux != NULL){
			fprintf(*destino,"%-30s %-6s %-3d %-10s %-10s %-15s\n",(*aux).endereco,(*aux).tipo,(*aux).code,(*aux).browser,(*aux).so,(*aux).cliente);
			aux = (*aux).nxt;
		}
	}
}
