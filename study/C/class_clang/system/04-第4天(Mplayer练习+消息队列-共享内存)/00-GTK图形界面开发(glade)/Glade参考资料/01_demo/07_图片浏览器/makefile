CC = gcc  
MAINC = glade.c
EXEC = pc_demo

CFLAGS = `pkg-config --cflags --libs gtk+-2.0 libglade-2.0` -lpthread -ldl -lgthread-2.0 -rdynamic

main:  
	$(CC)  $(MAINC)  -o $(EXEC) $(CFLAGS)
clean:
	rm $(EXEC) -rf