/* Cabeçalho */

#include "stats.h"
#include <time.h>
#include <stdio.h>


void stat_print(int fd,req_queue * base);
void stat_clear(int fd, char* file, req_queue ** base, req_queue** top);
