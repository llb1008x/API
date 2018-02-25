all:tcp_echo_pthread tcp_echo_fork
TARGET = tcp_echo_pthread 
CFLAGS = -Wall -lpthread
CC = gcc

tcp_echo_pthread:tcp_echo_pthread.c
	$(CC) tcp_echo_pthread.c -o tcp_echo_pthread $(CFLAGS)
	
tcp_echo_fork:tcp_echo_fork.c
	$(CC) tcp_echo_fork.c -o tcp_echo_fork $(CFLAGS)
	
clean:
	rm -fr $(TARGET)
	