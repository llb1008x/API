OBJ += main.o
OBJ += ../pthread/arp_pthread.o
OBJ += ../pthread/udp_pthread.o
OBJ += ../source/ip_link.o
OBJ += ../source/ip_filter.o
OBJ += ../pthread/key_pthread.o
OBJ += ../source/get_interface.o
OBJ += ../source/pos/console.o


CC = gcc
cflags = -lpthread 
router:$(OBJ)  
	$(CC) $(OBJ) -o $@  $(cflags)
%.o:%.c
	$(CC) -c $^ -o $@ 
.PHONY:clean
clean:
	rm router *.o -rfv	
