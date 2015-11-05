/* Cabeçalho */


#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv){
	pid_t aux;
	if(argc<2) exit(0);
	if(!sscanf(argv[1],"%lld",(long long int*) &aux)) exit(0);
	sleep(5);
	kill(aux,SIGKILL);
	exit(0);
}
