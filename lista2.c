#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lista1.h"
#include "lista2.h"

/*Função para inserir pedido na lista dinâmica*/
par * insere_par(par*base,const char * browser, const char * so, int n){
	par * aux;
	aux = base;
	if(aux == NULL){
		aux = malloc(sizeof(par));
		strncpy((*aux).nav,browser,8);
		strncpy((*aux).so,so,10);
		(*aux).num = n;
		(*aux).nxt = NULL;
		base = aux;
	}else{
		while((*aux).nxt !=NULL){
			aux = (*aux).nxt;
		}
		(*aux).nxt = malloc(sizeof(par));
		aux = (*aux).nxt;
		strncpy((*aux).nav,browser,8);
		strncpy((*aux).so,so,10);
		(*aux).num = n;
		(*aux).nxt = NULL;
	}
	return base;
}

/*Função que apaga uma lista dinâmica*/
par * limpapar(par*base){
	par *aux;
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


void fazpar(pedido * baseped, FILE ** destino){
	par * basepar;
	int teste = 0;
	int total = 0;
	float racio,a,b;
	pedido *aux;
	par *parmax;
	par *auxpar;
	parmax = NULL;
	basepar = NULL;
	/*Uma vez mais, não faz sentido verificar se baseped existe pois na lista dinâmica existe pelo menos o pedido "/estatisticas/Browser"*/
	if((*baseped).nxt!=NULL){
		aux = baseped;
		while((*aux).nxt != NULL){
			teste = 0;
			if(basepar == NULL){
				basepar = insere_par(basepar,(*aux).browser,(*aux).so,1);
			}else{
				auxpar = basepar;
				while(auxpar != NULL){
					if((strcmp((*aux).browser,(*auxpar).nav)==0)&&(strcmp((*aux).so,(*auxpar).so)==0)){
						(*auxpar).num = (*auxpar).num + 1;
						teste = 1;
					}
					auxpar = (*auxpar).nxt;
				}
				if(teste == 0){
					basepar = insere_par(basepar,(*aux).browser,(*aux).so,1);
				}
			}
			aux = (*aux).nxt;
		}
		
		/*-------*/
		auxpar = basepar;
		while(auxpar != NULL){
			total = total + (*auxpar).num;
			auxpar = (*auxpar).nxt;
		}
		fprintf(*destino,"<html><head><title>Estatisticas - Browser</title></head><body><h2>Navegadores e Sistemas Operativos</h2><br/><table><tr><th>Browser</th><th>Sistema Operativo</th><th>Acessos</th><th>Percentagem</th></tr>");
		auxpar = basepar;
		while(auxpar != NULL){
			a = (*auxpar).num;
			b = total;
			racio = a / b;
			racio = racio * 100;
			fprintf(*destino,"<tr><td>%s</td><td>%s</td><td>%d</td><td>%.0f%%</td></tr>",(*auxpar).nav,(*auxpar).so,(*auxpar).num,racio);
			if(parmax == NULL){
				parmax = insere_par(parmax,(*auxpar).nav,(*auxpar).so,(*auxpar).num);
			}else if((*auxpar).num == (*parmax).num){
				parmax = insere_par(parmax,(*auxpar).nav,(*auxpar).so,(*auxpar).num);
			}else if((*auxpar).num > (*parmax).num){
				parmax = limpapar(parmax);
				parmax = insere_par(parmax,(*auxpar).nav,(*auxpar).so,(*auxpar).num);
			}
			auxpar = (*auxpar).nxt;
		}
		fprintf(*destino,"</table><br/><br/><h3>Navegador(es) e Sistema(s) Operativo(s) mais frequente(s):</h3><br/>\n");
		auxpar = parmax;
		while(auxpar !=NULL){
			fprintf(*destino,"%s / %s - %d acesso(s)<br/>\n",(*auxpar).nav,(*auxpar).so,(*auxpar).num);
			auxpar = (*auxpar).nxt;
		}
		fprintf(*destino,"</body></html>");
		parmax = limpapar(parmax);
		basepar = limpapar(basepar);
	}else{
		fprintf(*destino,"<html><head><title>Estatisticas - Browser</title></head><body><h2>Navegadores e Sistemas Operativos</h2><br/>Nao ha registo de pedidos efectuados.</body></html>");
	}
}
