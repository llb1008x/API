all:server client
TARGET = server client
CFLAGS = -Wall
CC = gcc

server:server.c
	gcc server.c -o server $(CFLAGS)
client:client.c
	gcc client.c -o client $(CFLAGS)

clean:
	rm -rf *.o  $(TARGET)
