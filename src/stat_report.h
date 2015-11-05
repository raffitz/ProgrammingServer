/* Cabeçalho */

#include "stats.h"
#include <stdio.h>
#define _XOPEN_SOURCE 700

void stat_print(int fd,req_queue * base);
req_queue* stat_clear(int fd, char* file, req_queue * base);
