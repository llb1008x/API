#ifndef MIAN_H
#define MIAN_H

int raw_sock_fd;

#define RECV_SIZE 2048
typedef struct recv_data{
	ssize_t data_len ;
	unsigned char data[RECV_SIZE];
}RECV_DATA;
#endif