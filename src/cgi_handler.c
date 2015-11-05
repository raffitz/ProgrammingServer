/* Cabeçalho */

#include "cgi_handler.h"

/* Dups the appropriate file descriptors and runs the cgi script specified. */
void cgi_run(int fdtarget, char* file){
	close(1);
	dup(fdtarget);
	execl(file,file,NULL);
}

/* Runs a CGI-like script, passing two arguments.
   As of the current implementation, it is used exclusively for
   directory listing. */
void cgi_run_args(int fdtarget, char* file, char* arg1,char* arg2){
	close(1);
	dup(fdtarget);
	execl(file,file,arg1,arg2,NULL);
}
