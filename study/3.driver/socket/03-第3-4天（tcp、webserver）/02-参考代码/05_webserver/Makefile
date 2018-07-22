all:webserver_t webserver_f
TARGET = webserver_t webserver_f
CFLAGS = -Wall -lpthread
CC = gcc

webserver_t:webserver_t.c
	$(CC) webserver_t.c -o webserver_t $(CFLAGS)

webserver_f:webserver_f.c
	$(CC) webserver_f.c -o webserver_f $(CFLAGS)
	
clean:
	rm -rf $(TARGET)