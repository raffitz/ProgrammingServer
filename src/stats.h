/* Cabeçalho */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

typedef struct req_data{
	char* name;
	uint16_t code;
	uint8_t ip[4];
	time_t date;
	uint16_t length;
} req_data;

typedef struct req_queue{
	req_data data;
	struct req_queue * next;
} req_queue;

req_data newrequest(char*name,uint16_t code,uint8_t ip[4],uint64_t date, uint16_t length);
void req_push(req_data data,req_queue** base,req_queue** top);
void req_freeall(req_queue** base,req_queue** top);
uint16_t req_read(int fd,req_queue** base,req_queue** top);
void req_load(char* file,req_queue** base,req_queue** top);
void req_save(char* file,req_queue* base);
