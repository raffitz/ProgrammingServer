# Makefile Projecto PSis
# Compliant with V1 of the specifications

CC=gcc
CFLAGS=-Wall -pedantic -c -std=gnu11 -g

all: server_73425_73786

server_73425_73786: main.o comm_man.o req_handler.o stats.o stat_report.o pid_list.o cgi_handler.o
	$(CC) main.o comm_man.o req_handler.o stats.o stat_report.o cgi_handler.o pid_list.o -o server_73425_73786 -lpthread -g

main.o: main.c comm_man.h pid_list.h req_handler.h stat_report.h cgi_handler.h stats.h
	$(CC) $(CFLAGS) main.c -o main.o

comm_man.o: comm_man.c comm_man.h req_handler.h stat_report.h cgi_handler.h stats.h
	$(CC) $(CFLAGS) comm_man.c -o comm_man.o

req_handler.o: req_handler.c req_handler.h cgi_handler.h stat_report.h cgi_handler.h stats.h
	$(CC) $(CFLAGS) req_handler.c -o req_handler.o

stats.o: stats.c stats.h
	$(CC) $(CFLAGS) stats.c -o stats.o

stat_report.o: stat_report.c stat_report.h stats.h
	$(CC) $(CFLAGS) stat_report.c -o stat_report.o

cgi_handler.o: cgi_handler.c cgi_handler.h
	$(CC) $(CFLAGS) cgi_handler.c -o cgi_handler.o
	
pid_list.o: pid_list.c pid_list.h
	$(CC) $(CFLAGS) pid_list.c -o pid_list.o

clear:
	rm *.o *~
