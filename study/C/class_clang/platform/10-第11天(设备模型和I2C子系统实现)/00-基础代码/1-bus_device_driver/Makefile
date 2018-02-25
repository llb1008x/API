# If KERNELRELEASE is defined, we've been invoked from the 
# kernel build system and can use its language. 
TARGET=$(t)
obj-m := $(TARGET).o

ifneq ($(KERNELRELEASE),) 
#KERNELDIR ?= /lib/modules/$(shell uname -r)/build 
KERNELDIR = $(KERNELRELEASE)
else 
KERNELDIR = /home/edu/kernel/linux-kernel-qt210
endif

$(PWD) := $(shell pwd)

default: 
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules 
	@echo $(TARGET)
	rm -rf *.o *.mod.c *.order *.symvers
clean:
	rm -rf *.ko



