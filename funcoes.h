#ifndef FUNCOES
#define FUNCOES
#include <stdlib.h>

void envia(FILE ** fonte, FILE ** destino);

int endereco(const char *fonte,char *destino);

int ext(const char *endereco,char*exten, char *** cts, int maxcts);

#endif
