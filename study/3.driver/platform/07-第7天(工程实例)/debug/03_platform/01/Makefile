#如果已定义KERNELRELEASE，说明是由内核构造系统调用的
#可以利用内建语句
ifneq ($(KERNELRELEASE),) 
	obj-m :=driver.o
	obj-m +=device.o
	#此时由内核构造系统调用
else 
	#定义并记录内核源码路径
	KERNELDIR = /home/haotian/2.6.35.7_tools/unsp210_linux_2.6.35
	#记录当前工程目录
	PWD := $(shell pwd)

default: 
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules 
	@rm -rf *.o .t* .m* .*.cmd *.mod.c *.order *.symvers *.tmp
	
endif
	
clean:
	rm -rf *.ko *.o .t* .m* .*.cmd *.mod.c *.order *.symvers
