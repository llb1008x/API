SRC := #
SRC += gtk_button_key_led.c
SRC += gpio_led.c

#OBJ := $(subst .c,.o,$(SRC))
OBJ = $(SRC:%.c=%.o)

PREFIX = /usr/local/arm/gtk_dfb/
export PKG_CONFIG_PATH=${PREFIX}/lib/pkgconfig
LDFLAGS=-L${PREFIX}/lib -Wl,-rpath,${PREFIX}/lib 
CFLAGS=-I${PREFIX}/include/gtk-2.0/

CC = gcc
FLAGS = $(CFLAGS) `pkg-config --cflags --libs gtk+-2.0 gmodule-export-2.0` -Wall
#FLAGS = $(CFLAGS) `pkg-config --cflags --libs gtk+-2.0` -export-dynamic -Wall
OPTION = $(LDFLAGS) -lpthread -ldl -lgthread-2.0
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
	
