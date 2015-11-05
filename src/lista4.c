#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define BUFFSIZE 1024
#include "lista1.h"
#include "lista4.h"

/*Função para inserir pedido na lista dinâmica*/
fich * insere_fich(fich*base,const char * nome){
	fich * aux;
	aux = base;
	if(aux == NULL){
		aux = malloc(sizeof(fich));
		strncpy((*aux).ficheiro,nome,BUFFSIZE);
		(*aux).num = 1;
		(*aux).nxt = NULL;
		base = aux;
	}else{
		while((*aux).nxt !=NULL){
			aux = (*aux).nxt;
		}
		(*aux).nxt = malloc(sizeof(fich));
		aux = (*aux).nxt;
		strncpy((*aux).ficheiro,nome,BUFFSIZE);
		(*aux).num = 1;
		(*aux).nxt = NULL;
	}
	return base;
}

/*Função que apaga uma lista dinâmica*/
fich * limpafich(fich*base){
	fich *aux;
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


void fazfich(pedido * baseped, FILE ** destino){
	fich * basefich;
	int teste = 0;
	int total = 0;
	pedido *aux;
	fich *auxfich;
	basefich = NULL;
	/*Podemos referir-nos ao apontado por baseped à vontade porque de certeza que existe pelo menos um pedido na lista (o "/estatisticas/Ficheiros")*/
	if((*baseped).nxt!=NULL){
		aux = baseped;
		while((*aux).nxt != NULL){
			teste = 0;
			if(basefich == NULL){
				basefich = insere_fich(basefich,(*aux).endereco);
			}else{
				auxfich = basefich;
				while(auxfich != NULL){
					if(strcmp((*aux).endereco,(*auxfich).ficheiro)==0){
						(*auxfich).num = (*auxfich).num + 1;
						teste = 1;
					}
					auxfich = (*auxfich).nxt;
				}
				if(teste == 0){
					basefich = insere_fich(basefich,(*aux).endereco);
				}
			}
			aux = (*aux).nxt;
		}
		
		/*-------*/
		auxfich = basefich;
		while(auxfich != NULL){
			total = total + (*auxfich).num;
			auxfich = (*auxfich).nxt;
		}
		fprintf(*destino,"<html><head><title>Estatisticas - Ficheiros</title></head><body><h2>Ficheiros pedidos</h2><br/><table><tr><th>Ficheiro</th><th>Acessos</th></tr>");
		auxfich = basefich;
		while(auxfich != NULL){
			fprintf(*destino,"<tr><td>%s</td><td>%d</td></tr>",(*auxfich).ficheiro,(*auxfich).num);
			auxfich = (*auxfich).nxt;
		}
		fprintf(*destino,"</table>\n</body></html>");		
		basefich = limpafich(basefich);
	}else{
		fprintf(*destino,"<html><head><title>Estatisticas - Ficheiros</title></head><body><h2>Ficheiros pedidos</h2><br/>Nao ha registo de pedidos efectuados.</body></html>");
	}
}
