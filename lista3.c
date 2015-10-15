#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lista1.h"
#include "lista3.h"

/*Função para inserir pedido na lista dinâmica*/
exten * insere_exten(exten*base,const char * tipo){
	exten * aux;
	aux = base;
	if(aux == NULL){
		aux = malloc(sizeof(exten));
		strncpy((*aux).tipo,tipo,6);
		(*aux).num = 1;
		(*aux).nxt = NULL;
		base = aux;
	}else{
		while((*aux).nxt !=NULL){
			aux = (*aux).nxt;
		}
		(*aux).nxt = malloc(sizeof(exten));
		aux = (*aux).nxt;
		strncpy((*aux).tipo,tipo,6);
		(*aux).num = 1;
		(*aux).nxt = NULL;
	}
	return base;
}

/*Função que apaga uma lista dinâmica*/
exten * limpaexten(exten*base){
	exten *aux;
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


void fazexten(pedido * baseped, FILE ** destino){
	exten * basex;
	int teste = 0;
	int total = 0;
	pedido *aux;
	exten *auxexten;
	basex = NULL;
	/*Podemos referir-nos ao apontado por baseped à vontade porque de certeza que existe pelo menos um pedido na lista (o "/estatisticas/Tipo")*/
	if((*baseped).nxt!=NULL){
		aux = baseped;
		while((*aux).nxt != NULL){
			teste = 0;
			if(basex == NULL){
				basex = insere_exten(basex,(*aux).tipo);
			}else{
				auxexten = basex;
				while(auxexten != NULL){
					if(strcmp((*aux).tipo,(*auxexten).tipo)==0){
						(*auxexten).num = (*auxexten).num + 1;
						teste = 1;
					}
					auxexten = (*auxexten).nxt;
				}
				if(teste == 0){
					basex = insere_exten(basex,(*aux).tipo);
				}
			}
			aux = (*aux).nxt;
		}
		
		/*-------*/
		auxexten = basex;
		while(auxexten != NULL){
			total = total + (*auxexten).num;
			auxexten = (*auxexten).nxt;
		}
		fprintf(*destino,"<html><head><title>Estatisticas - Tipo</title></head><body><h2>Tipos de Ficheiro pedidos</h2><br/><table><tr><th>Tipo</th><th>Acessos</th></tr>");
		auxexten = basex;
		while(auxexten != NULL){
			fprintf(*destino,"<tr><td>%s</td><td>%d</td></tr>",(*auxexten).tipo,(*auxexten).num);
			auxexten = (*auxexten).nxt;
		}
		fprintf(*destino,"</table>\n</body></html>");
		
		basex = limpaexten(basex);
	}else{
		fprintf(*destino,"<html><head><title>Estatisticas - Tipo</title></head><body><h2>Tipos de Ficheiro pedidos</h2><br/>Nao ha registo de pedidos efectuados.</body></html>");
	}
}
