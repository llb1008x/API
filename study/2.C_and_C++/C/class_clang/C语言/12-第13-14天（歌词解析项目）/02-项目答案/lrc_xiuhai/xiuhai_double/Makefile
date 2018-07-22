CC=gcc
obj=main.o lrc.o start_mplayer.o ./pos/console.o
target=main
CFLAGS=-Wall 

$(target):$(obj)
	@$(CC) $^ -o $@ $(CFLAGS)
main.o:main.c 
	@$(CC) -c $< -o $@ $(CFLAGS)
lrc.o:lrc.c lrc.h
	@$(CC) -c $< -o $@ $(CFLAGS)
start_mplayer.o:start_mplayer.c
	@$(CC) -c $< -o $@ $(CFLAGS)
./pos/console.o:./pos/console.c ./pos/console.h
	@$(CC) -c $< -o $@ $(CFLAGS)

clean:
	@rm $(obj) $(target) -rf
