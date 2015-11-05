/* cabeçalho */

#include "stats.h"


req_data newrequest(char*name,uint16_t code,uint8_t ip[4],uint64_t date, uint16_t length){
	req_data req_return;
	uint8_t i;
	req_return.name = name;
	req_return.date = date;
	req_return.length = length;
	for(i=0;i<4;i++){
		req_return.ip[i] = ip[i];
	}
	return req_return;
}


req_queue* req_push(req_data data,req_queue* base){
	req_queue* aux = base;
	if(base == NULL){
		base = malloc(sizeof(req_queue));
		aux = base;
	}else{
		while((*aux).next != NULL){
			aux = (*aux).next;
		}
		(*aux).next = malloc(sizeof(req_queue));
		aux = (*aux).next;
	}
	(*aux).next = NULL;
	(*aux).data = data;
	return base;
}


void req_freeall(req_queue* base){
	req_queue* aux;
	while(base!=NULL){
		aux = base;
		base = (*aux).next;
		free((*aux).data.name);
		free(aux);
	}
}


req_queue* req_load(char* file,req_queue* base){
	int fd;
	uint16_t size;
	char* aux;
	req_data buffer;
	fd = open(file,O_RDONLY);
	if(fd<0){
		printf("Safeguard request statistics not found.\n");
		return NULL;
	}
	while(1){
		if(read(fd,&size,sizeof(uint16_t))<=0){
			break;
		}
		aux = malloc(size * sizeof(char));
		if(read(fd,aux,size)!=size){
			printf("Error reading Safeguard request statistics.\n");
			free(aux);
			req_freeall(base);
			return NULL;
		}
		buffer.name = aux;
		if(read(fd,&buffer + sizeof(char*),sizeof(req_data)-sizeof(char*))!=(sizeof(req_data) - sizeof(char*))){
			printf("Error reading Safeguard request statistics.\n");
			free(aux);
			req_freeall(base);
			return NULL;
		}
		base = req_push(buffer,base);
	}
	return base;
}


void req_save(char* file,req_queue* base){
	int fd;
	uint16_t size;
	req_queue* aux = base;
	
	
	fd = creat(file,0 || S_IRUSR || S_IWUSR || S_IRGRP || S_IWGRP || S_IROTH);
	while(aux!=NULL){
		size = strlen((*aux).data.name) + 1;
		write(fd,&size,sizeof(uint16_t));
		write(fd,(*aux).data.name,size);
		write(fd,aux + sizeof(char*),sizeof(req_data)-sizeof(char*));
		aux = (*aux).next;
	}
	close(fd);
	return;
}
