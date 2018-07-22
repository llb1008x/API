DRIVERS-y 	:=
DRIVERS-y 	+= main.o
DRIVERS-y 	+= uart.o

OBJS 	:= $(DRIVERS-y)

CROSSTOOLDIR 	:= /usr/local/arm/4.3.2
CROSS_COMPILE   = $(CROSSTOOLDIR)/bin/arm-linux-
CC          	= $(CROSS_COMPILE)gcc
LD          	= $(CROSS_COMPILE)ld
OBJCOPY     	= $(CROSS_COMPILE)objcopy
OBJDUMP     	= $(CROSS_COMPILE)objdump
NM		     	= $(CROSS_COMPILE)nm

LFLAGS 	= -Tapp.lds -Bstatic

CLIBSDIR	= $(CROSSTOOLDIR)/arm-none-linux-gnueabi/libc/armv4t/usr/lib
GCCLIBSDIR 	= $(CROSSTOOLDIR)/lib/gcc/arm-none-linux-gnueabi/4.3.2/armv4t
ARMLIBS 	= -L$(CLIBSDIR) -lc -L$(GCCLIBSDIR) -lgcc

export CC LD OBJCOPY OBJDUMP

app.bin :$(OBJS)
	$(LD) -v $(LFLAGS) -o app_elf $(OBJS) $(ARMLIBS)
	$(OBJCOPY) -O binary -S app_elf $@
	$(OBJDUMP) -D -m arm  app_elf > app.dis
	$(NM) -v -l app_elf > app.map
	
%.o:%.c
	$(CC) -Wall -c -O2 -o $@ $< -fno-builtin-printf
%.o:%.S
	$(CC) -Wall -c -O2 -o $@ $< -fno-builtin-printf
	
clean: 
	find -name '*.o' | xargs rm -f
	rm -f app.dis app.bin app_elf app.map 
