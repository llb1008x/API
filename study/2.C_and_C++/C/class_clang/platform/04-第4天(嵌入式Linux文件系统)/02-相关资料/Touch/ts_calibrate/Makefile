#This is a simple Makefile
CC=/usr/local/arm/4.3.2/bin/arm-linux-gcc

SRC=main.c
 
ts_test:$(SRC) 
	$(CC) $(SRC) ./lcd_lib.a -lpthread -o ts_test

clean:
	rm $(object)  ts_test -rf
