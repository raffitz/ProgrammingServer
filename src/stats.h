/* Cabeçalho */

#include <stdint.h>
#include <string.h>

typedef struct req_data{
	char* name;
	uint16_t code;
	uint8_t ip[4];
	uint64_t date;
	uint16_t length;
} req_data;

typedef struct req_queue{
	req_data request;
	struct req_queue * next;
} req_queue;

req_data newrequest(char*name,uint16_t code,uint8_t ip[4],uint64_t date, uint16_t length);
req_queue* req_push(req_data data,req_queue* base);
void req_freeall(req_queue* base);
req_queue* req_load(char* file,req_queue* base);
req_queue* req_save(char* file,req_queue* base);
