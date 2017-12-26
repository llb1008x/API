#SRC := #
SRC += g_thread_create.c

#OBJ := $(subst .c,.o,$(SRC))
OBJ = $(SRC:%.c=%.o)

CFLAGS=`pkg-config --cflags --libs gmodule-export-2.0 gtk+-2.0`

CC = gcc
FLAGS = $(CFLAGS) -Wall 
OPTION = -lpthread -ldl -lgthread-2.0
EXEC_PATH = ./
EXEC_NAME = demo

.PHONY:clean demo

demo:$(OBJ)
	@echo make ...
	$(CC) $^ -o $(EXEC_PATH)/$(EXEC_NAME) $(FLAGS) $(OPTION)
	@echo make over
	@echo Execute target is $(EXEC_PATH)/$(EXEC_NAME)
$(OBJ):%.o:%.c
	$(CC) -c -o $@ $< $(FLAGS)
clean:
	@echo clean ...
	rm $(EXEC_PATH)/$(EXEC_NAME) *.o -rf
	@echo clean over
