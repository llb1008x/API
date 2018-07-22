
#TOPDIR  := $(shell cd ..; pwd)
TOPDIR  := .


KERNELDIR = /home/lijun/kernel
#KERNELDIR = /opt/host/armv4l/src/linux
#KERNELDIR = /data/arm2410s/kernel-2410s
INCLUDEDIR = $(KERNELDIR)/include

CROSS_COMPILE=arm-linux-

AS      =$(CROSS_COMPILE)as
LD      =$(CROSS_COMPILE)ld
CC      =$(CROSS_COMPILE)gcc
CPP     =$(CC) -E
AR      =$(CROSS_COMPILE)ar
NM      =$(CROSS_COMPILE)nm
STRIP   =$(CROSS_COMPILE)strip
OBJCOPY =$(CROSS_COMPILE)objcopy
OBJDUMP =$(CROSS_COMPILE)objdump



CFLAGS += -I..
CFLAGS += -Wall -O -D__KERNEL__ -DMODULE -I$(INCLUDEDIR)


TARGET = s3c2410-adc.o

# TARGET = usb-skeleton2.4.o
all: $(TARGET)


s3c2410-adc.o: s3c2410-adc.c

#usb-skeleton2.4.o: usb-skeleton2.4.c
	$(CC) -c $(CFLAGS) $^ -o $@


install:
	install -d $(INSTALLDIR)
	install -c $(TARGET).o $(INSTALLDIR)

clean:
	rm -f *.o *~ core .depend
