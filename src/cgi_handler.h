/*******************************************************************************
 *	PROJECTO PSis 2ºSEMESTRE 2013-2014
 *		Servidor Web
 *	
 *	Rafael Gonçalves 73786
 *	António Bacelar 73425
 *
 *	Ficheiro: cgi_handler.h
 *	Descriçao: Includes e Protótipos de cgi_handler.c
 ******************************************************************************/

#include <unistd.h>

void cgi_run(int fdtarget, char* file);
void cgi_run_args(int fdtarget, char* file, char* arg1,char* arg2);
