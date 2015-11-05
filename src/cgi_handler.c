/* Cabeçalho */

#include "cgi_handler.h"

void cgi_run(int fdtarget, char* file){
	dup2(1,fdtarget);
	execl(file,file,NULL);
}
