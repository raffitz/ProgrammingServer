#ifndef LISTA2
#define LISTA2
#include "lista1.h"

/*Tipo de dados para os pares browser/so */

typedef struct par{
	char nav[8];
	char so[10];
	int num;
	struct par * nxt;
} par;

par * insere_par(par*base,const char * browser, const char * so, int n);

par * limpapar(par*base);

void fazpar(pedido * baseped, FILE ** destino);

#endif
