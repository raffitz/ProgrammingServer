/* Cabeçalho */

#include "cgi_handler.h"

void cgi_run(int fdtarget, char* file){
	close(1);
	dup(fdtarget);
	execl(file,file,NULL);
}

void cgi_run_args(int fdtarget, char* file, char* arg){
	close(1);
	dup(fdtarget);
	execl(file,file,arg,NULL);
}
