# If KERNELRELEASE is defined, we've been invoked from the 
# kernel build system and can use its language. 

#使用办法:#
#执行make命令时 加上t=文件名 eg: make t=goodix_touch_v2 #
obj-m := $(t).o  

ifneq ($(KERNELRELEASE),) 
#KERNELDIR ?= /lib/modules/$(shell uname -r)/build 
KERNELDIR = $(KERNELRELEASE)
else 
KERNELDIR = /home/lvfeng/kernel/linux-kernel-qt210-cam/linux-kernel-qt210
endif

$(PWD) := $(shell pwd)

default: 
	@echo "$(shell pwd)"
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules 
	rm -rf *.o *.mod.c *.order *.symvers
clean:
	rm -rf *.ko



