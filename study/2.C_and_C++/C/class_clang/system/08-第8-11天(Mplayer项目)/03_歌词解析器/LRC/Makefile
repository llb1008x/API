OBJ += lyrics_process.o
OBJ += demo.o
CC = gcc
FLAG = -Wall
EXEC_NAME = test

play:$(OBJ)
	@echo make ...
	@$(CC) $^ -o $(EXEC_NAME) $(FLAG)
	@echo make over
	@echo Execute target is $(EXEC_NAME)
%.o:%.c
	@$(CC) -c -o $@ $< $(FLAG)

.PHONY:clean
clean:
	@echo clean ...
	rm $(EXEC_NAME) *.o -rf
	@echo clean over
