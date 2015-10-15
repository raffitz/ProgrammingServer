#ifndef LISTA3
#define LISTA3
#include "lista1.h"

/*Tipo de dados para os tipos de ficheiro */

typedef struct exten{
	char tipo[6];
	int num;
	struct exten * nxt;
} exten;

exten * insere_exten(exten*base,const char * tipo);

exten * limpaexten(exten*base);

void fazexten(pedido * baseped, FILE ** destino);

#endif
