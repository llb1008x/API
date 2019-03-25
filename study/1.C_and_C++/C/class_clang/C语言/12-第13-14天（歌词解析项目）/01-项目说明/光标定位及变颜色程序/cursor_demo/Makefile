CC=gcc
obj=console.o test.o
target=test
CFLAGS=-Wall 

$(target):$(obj)
	@$(CC) $^ -o $@ $(CFLAGS)
test.o:test.c 
	@$(CC) -c $< -o $@ $(CFLAGS)
console.o:console.c 
	@$(CC) -c $< -o $@ $(CFLAGS)


clean:
	@rm $(obj) $(target)
