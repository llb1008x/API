# If KERNELRELEASE is defined, we've been invoked from the 
# kernel build system and can use its language. 
obj-m := s5pv210-gpio.o 

#35pv210
KERNELDIR = /home/lingchun/share/sources/unsp210_linux_2.6.35

$(PWD) := $(shell pwd)

default: 
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules 
	rm -rf *.o *.mod.c *.order *.symvers
clean:
	rm -rf *.ko



