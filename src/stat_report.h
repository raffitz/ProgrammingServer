/*******************************************************************************
 *	PROJECTO PSis 2ºSEMESTRE 2013-2014
 *		Servidor Web
 *	
 *	Rafael Gonçalves 73786
 *	António Bacelar 73425
 *
 *	Ficheiro: stat_report.h
 *	Descriçao: Includes e Protótipos de stat_report.c
 ******************************************************************************/

#include "stats.h"
#include <time.h>
#include <stdio.h>

typedef struct report_args{
	int req_pipe;
	req_base * req_list;
}report_args;

report_args* set_report_args(int req_pipe, req_base* req_list);
void stat_print(int fd,req_queue * base);
void stat_clear(int fd, char* file, req_queue ** base, req_queue** top);
void * report_handler(void* arg);
