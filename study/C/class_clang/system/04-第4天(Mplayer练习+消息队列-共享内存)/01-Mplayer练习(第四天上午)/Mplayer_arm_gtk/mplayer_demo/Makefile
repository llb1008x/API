src += demo.c

target = demo

cc  = arm-linux-gcc 
cflags = -lpthread -Wall -g -lgthread-2.0 `pkg-config --cflags --libs gtk+-2.0` `pkg-config --cflags --libs libglade-2.0`
prefix = /opt/gtkdfb
ldflags = -L$(prefix)/lib -Wl,-rpath,$(prefix)/lib 
cppflags = -I$(prefix)/include/gtk-2.0/ 

export PKG_CONFIG_PATH = /opt/gtkdfb/lib/pkgconfig

obj = $(src:%.c=%.o)

$(target):$(obj)
	$(cc) $^ -o $@ $(cflags) $(ldflags)
%.o:%.c
	$(cc) -c $< -o $@ $(cflags) $(cppflags)
	
.phony:clean
clean:
	rm *.o $(obj) $(target) -fr

