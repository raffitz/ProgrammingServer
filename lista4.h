#ifndef LISTA4
#define LISTA4
#define BUFFSIZE 1024
#include "lista1.h"

/*Tipo de dados para os nomes de ficheiro */

typedef struct fich{
	char ficheiro[BUFFSIZE];
	int num;
	struct fich * nxt;
} fich;

fich * insere_fich(fich*base,const char * nome);

fich * limpafich(fich*base);

void fazfich(pedido * baseped, FILE ** destino);

#endif
