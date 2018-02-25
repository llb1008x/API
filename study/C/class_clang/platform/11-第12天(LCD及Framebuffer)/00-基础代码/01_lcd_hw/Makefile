DRIVERS-y :=
DRIVERS-y += main.o
DRIVERS-y += uart.o
DRIVERS-y += fimd.o
DRIVERS-y += lcd_ops.o

OBJS := $(DRIVERS-y)

CROSSTOOLDIR := /usr/local/arm/4.3.2
CROSS_COMPILE   = $(CROSSTOOLDIR)/bin/arm-linux-
CC          = $(CROSS_COMPILE)gcc
LD          = $(CROSS_COMPILE)ld
OBJCOPY     = $(CROSS_COMPILE)objcopy
OBJDUMP     = $(CROSS_COMPILE)objdump
NM		     = $(CROSS_COMPILE)nm
LFLAGS = -Tapp.lds -Bstatic

ARM_GCC_LIBS	= $(CROSSTOOLDIR)/arm-none-linux-gnueabi/libc/armv4t/usr/lib
CLIBS = -L$(ARM_GCC_LIBS) -lc -L$(CROSSTOOLDIR)/lib/gcc/arm-none-linux-gnueabi/4.3.2/armv4t -lgcc

export CC LD OBJCOPY OBJDUMP

app.bin :$(OBJS)
	$(LD) -v $(LFLAGS) -o app_elf $(OBJS) $(CLIBS)
	$(OBJCOPY) -O binary -S app_elf $@
	$(OBJDUMP) -D -m arm  app_elf > app.dis
	$(NM) -v -l app_elf > app.map

%.o:%.c
	$(CC) -Wall -c -O2 -o $@ $<
%.o:%.S
	$(CC) -Wall -c -O2 -o $@ $<

	
clean: 
	find . \( -name '*.o' -o -name core -o -name ".*.flags" \) -type f -print \
	| grep -v lxdialog/ | xargs rm -f
	rm -f  app.dis app.bin app_elf app.map 
