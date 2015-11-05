# Makefile Projecto PSis
# Compliant with V1 of the specifications

CC=gcc
CFLAGS=-Wall -ansi -pedantic -c

all: server_73425_73786

server_73425_73786: main.o comm_man.o req_handler.o stats.o stat_report.o cgi_handler.o
	$(CC) main.o comm_man.o req_handler.o stats.o stat_report.o cgi_handler.o -o server_73425_73786

main.o: main.c comm_man.h stats.h
	$(CC) $(CFLAGS) main.c -lpthread

comm_man.o: comm_man.c comm_man.h req_handler.h
	$(CC) $(CFLAGS) comm_man.c -lpthread

req_handler.o: req_handler.c req_handler.h cgi_handler.h stats.h stat_report.h
	$(CC) $(CFLAGS) req_handler.c -lpthread

stats.o: stats.c stats.h
	$(CC) $(CFLAGS) stats.c

stat_report.o: stat_report.c stat_report.h stats.h
	$(CC) $(CFLAGS) stat_report.c

cgi_handler.o: cgi_handler.c cgi_handler.h
	$(CC) $(CFLAGS) cgi_handler.c

clear:
	rm *.o *~
