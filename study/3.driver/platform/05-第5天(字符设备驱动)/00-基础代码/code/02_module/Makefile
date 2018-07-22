obj-m := driver.o
KERNELDIR = /home/xiuhai/work/platform/kernel/unsp210_linux_2.6.35/

default:
	$(MAKE) -C $(KERNELDIR) M=`pwd`
	rm -rf *.order *.mod.* *.o *.symvers
	
clean:
	rm -rf *.ko