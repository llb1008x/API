src += demo.c
src += sungtk_interface.c
target = demo 

cc = gcc 
obj = $(src:%.c=%.o)

#使用glade需要加上 参数libglade-2.0
cflags = `pkg-config --cflags --libs gtk+-2.0` `pkg-config --cflags --libs libglade-2.0` 
option = -Wall
$(target):$(obj)
	$(cc) $^ -o $@ $(cflags) $(option)
%.o:%.c
	$(cc) -c $< -o $@  $(cflags) $(option)
	
clean:
	rm *.o $(targer) -rf
