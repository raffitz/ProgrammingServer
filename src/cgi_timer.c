/* Cabeçalho */


#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv){
	pid_t aux;
	/* If it was not passed a PID, it closes, as it has no purpose whatsoever. */
	if(argc<2) exit(0);
	/* If the PID is illegible, it closes, as it has no purpose whatsoever. */
	if(!sscanf(argv[1],"%lld",(long long int*) &aux)) exit(0);
	/* Waits for 5 seconds. It can be terminated by the main server during this time. */
	sleep(5);
	/* It kills the CGI process after 5 seconds, as it was intended. */
	kill(aux,SIGKILL);
	exit(0);
}
