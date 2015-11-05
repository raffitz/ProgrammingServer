/* Cabeçalho */

#include "stat_report.h"

void stat_print(int fd,req_queue * base){
	req_queue* aux =base;
	dprintf(fd,"<html><head><title>Estatísticas - Pedidos</title>\n");
	dprintf(fd,"<style>\ntable,th,td\n{\nborder:1px solid black;\n}\n");
	dprintf(fd,"\nth\n{\nbackground-color:#666666\ncolor:#ffffff\n");
	dprintf(fd,"text-align:center;\nfont-weight:bold\n}\n");
	dprintf(fd,"</style></head><body>\n");
	dprintf(fd,"<h1>Estatísticas - Pedidos</h1>\n");
	dprintf(fd,"<table><tr><th>Data</th><th>Nome</th><th>Código</th>");
	dprintf(fd,"<th>IP do Cliente</th><th>Tempo</th></tr>\n");
	while(aux!=NULL){
		dprintf(fd,"<tr>");
		dprintf(fd,"<td>%s</td><td>%s</td><td>%d</td><td>%d.%d.%d.%d</td><td>%d",
			ctime(&((*aux).data.date)),(*aux).data.name,(*aux).data.code,
			(*aux).data.ip[0],(*aux).data.ip[1],(*aux).data.ip[2],(*aux).data.ip[3],
			(*aux).data.length);
		dprintf(fd,"</td></tr>");
		aux = (*aux).next;
	}
	dprintf(fd,"</table></body></html>");
}

void stat_clear(int fd, char* file, req_queue ** base, req_queue** top){
	int sg_fd;
	req_freeall(base,top);
	dprintf(fd,"<html><head><title>Estatísticas - Pedidos</title>\n");
	dprintf(fd,"</head><body>\n");
	dprintf(fd,"<h1>Estatísticas - Clear All</h1>\n");
	dprintf(fd,"Todos os dados armazenados foram eliminados.\n");
	dprintf(fd,"</body></html>\n");
	sg_fd = creat(file,0 || S_IRUSR || S_IWUSR || S_IRGRP || S_IWGRP || S_IROTH);
	close(sg_fd);
	return;
}
