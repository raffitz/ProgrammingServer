#ifndef LISTA1
#define LISTA1
#define BUFFSIZE 1024

/*Tipo de dados para a lista*/

typedef struct pedido{
	char endereco[BUFFSIZE];
	char tipo[6];
	int code;
	char browser[8];
	char so[10];
	char cliente[100];
	struct pedido * nxt;
}pedido;

pedido * insere_pedido(pedido*base,const char * end,const char * tipo, int code, const char * browser, const char * so, const char * cliente);

void imprime_600(pedido *base, FILE ** destino);

void imprime_601(pedido *base, FILE ** destino);

pedido * le_backup(FILE ** origem,pedido*base,FILE ** reportaddress);

pedido * clearlist(pedido*base);

pedido * clrfich(pedido*base);

void listdump(pedido *base, FILE ** destino);

#endif
