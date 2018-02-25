OBJ += main.o
OBJ += recv_net_data.o
OBJ += arp_link.o
OBJ += ip_deal.o
OBJ += get_interface.o
OBJ += ip_link.o
OBJ += firewall_pthread.o
OBJ += tftp_deal.o
OBJ += thread_pool.o

FLAGS = -Wall
CC = gcc

router:$(OBJ)  
	$(CC) $(OBJ) -o $@ $(FLAGS) -lpthread
%.o:%.c
	$(CC) -c $^ -o $@ $(FLAGS)
.PHONY:clean
clean:
	rm router *.o -rfv	
