/* cabeçalho */

#include "stats.h"

/* Creates request structure from discrete variables */
req_data newrequest(char*name,uint16_t code,uint8_t ip[4],time_t date, uint16_t length){
	req_data req_return;
	uint8_t i;
	req_return.name = name;
	req_return.code = code;
	req_return.date = date;
	req_return.length = length;
	for(i=0;i<4;i++){
		req_return.ip[i] = ip[i];
	}
	return req_return;
}

/* Pushes the passed request structure to the top of the statistics list */
void req_push(req_data data,req_queue** base,req_queue** top){
	req_queue* aux;
	if((*base) == NULL){
		(*base) = malloc(sizeof(req_queue));
		aux = (*base);
		(*top)=(*base);
	}else{
		(*(*top)).next = malloc(sizeof(req_queue));
		aux=(*(*top)).next;
	}
	(*aux).next = NULL;
	(*aux).data = data;
	(*top) = aux;
	return;
}

/* Frees the entirety of the statistics list */
void req_freeall(req_queue** base,req_queue** top){
	req_queue* aux;
	while((*base)!=NULL){
		aux = (*base);
		(*base) = (*aux).next;
		free((*aux).data.name);
		free(aux);
	}
	*top = NULL;
	return;
}

/* Reads from a file descriptor request structures, and pushes them to the list */
uint16_t req_read(int fd,req_queue** base,req_queue** top){
	uint16_t size;
	char* aux;
	req_data buffer;
	if(read(fd,&size,sizeof(uint16_t))<=0){
		return 0;
	}
	aux = malloc(size * sizeof(char));
	if(read(fd,aux,size)!=size){
		printf("Error reading request statistics.\n");
		free(aux);
		req_freeall(base,top);
		return 0;
	}
	buffer.name = aux;
	read(fd,&(buffer.code),sizeof(uint16_t));
	read(fd,buffer.ip,4*sizeof(uint8_t));
	read(fd,&(buffer.date),sizeof(time_t));
	read(fd,&(buffer.length),sizeof(uint16_t));
	req_push(buffer,base,top);
	return 1;
}

/* Writes to a file descriptor request structures */
void req_write(int fd,req_data data){
	uint16_t size;
	size = strlen(data.name) + 1;
	write(fd,&size,sizeof(uint16_t));
	write(fd,data.name,size);
	write(fd,&(data.code),sizeof(uint16_t));
	write(fd,data.ip,4*sizeof(uint8_t));
	write(fd,&(data.date),sizeof(time_t));
	write(fd,&(data.length),sizeof(uint16_t));
	free(data.name);
}

/* UNUSED - Loads from a file a statistics safeguard */
void req_load(char* file,req_queue** base,req_queue** top){
	int fd;
	int counter = 0;
	fd = open(file,O_RDONLY);

	if(fd<0){
		printf("Safeguard request statistics not found.\n");
		return;
	}
	while(req_read(fd,base,top)){
		counter++;
	}
	printf("%d requests read from Safeguard.\n",counter);
	return;
}

/* UNUSED - Creates a statistics safeguard file */
void req_save(char* file,req_queue* base){
	int fd;
	req_queue* aux = base;
	
	fd = creat(file,0 | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	while(aux!=NULL){
		req_write(fd,(*aux).data);
		aux = (*aux).next;
	}
	close(fd);
	return;
}

/* Reads from a pipe request structures and pushes them to the stats list */
void* stat_handler(void* arg){
	req_base* base = (req_base *) arg;
	
	while(1){
		req_read((*base).req_pipe[0],&((*base).base),&((*base).top));
	}
	return NULL;
}
