/* Cabeçalho */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

typedef struct pid_list{
	pid_t pid;
	struct pid_list * next;
} pid_list;

void pid_add(pid_t pid,pid_list** base,pid_list** top);
void pid_freeall(pid_list** base,pid_list** top);
void pid_killall(pid_list* base);
void* pid_handler(void* arg);
