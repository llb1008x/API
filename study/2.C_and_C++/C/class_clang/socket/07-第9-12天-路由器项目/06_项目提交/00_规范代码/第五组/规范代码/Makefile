OBJ += main.o
OBJ += arp_pthread.o
OBJ += arp_link.o
OBJ += ip_link.o
OBJ += key_pthread.o

OBJ += get_interface.o

FLAGS = -Wall
CC = gcc

router:$(OBJ)  
	$(CC) $(OBJ) -o $@ $(FLAGS) -lpthread
%.o:%.c
	$(CC) -c $^ -o $@ $(FLAGS)
.PHONY:clean
clean:
	rm router *.o -rfv	
