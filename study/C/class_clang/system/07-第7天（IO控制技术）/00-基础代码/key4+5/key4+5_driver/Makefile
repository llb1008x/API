MODULE_NAME := key4+5

MODULES_SRC := #
MODULES_SRC += key4+5_src.c
#MODULE_NAME 不能和模块源代码中去后缀的文件名相同

#MODULE_OBJ := $(MODULES_SRC:%.c=%.o)
MODULE_OBJ := $(subst .c,.o, $(MODULES_SRC))

obj-m := $(MODULE_NAME).o 
$(MODULE_NAME)-objs := $(MODULE_OBJ)

ifneq ($(KERNELRELEASE),)
	KERNELDIR = $(KERNELRELEASE)
else 
	#KERNELDIR = /lib/modules/$(shell uname -r)/build
	KERNELDIR = /home/lingchun/work/unsp210_linux_2.6.35
endif

PWD = $(shell pwd)

default: 
	@rm -rf *.ko
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules 
	@rm -rf *.o .t* .m* .*.cmd *.mod.c *.order
clean:
	rm -rf *.ko *.o .t* .m* .*.cmd *.mod.c *.order *.symvers

