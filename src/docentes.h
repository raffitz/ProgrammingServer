#ifndef DOCENTES
#define DOCENTES

int cria_socket();

void  espera_pedido(int serversock, FILE** fp_read, FILE ** fp_write, char *remote_address);

#endif
